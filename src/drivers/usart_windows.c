#include <stdio.h>
#include <Windows.h>
#include <process.h>

#include <csp/csp.h>
#include <csp/drivers/usart.h>

static HANDLE portHandle = INVALID_HANDLE_VALUE;
static HANDLE rxThread = INVALID_HANDLE_VALUE;
static CRITICAL_SECTION txSection;
static LONG isListening = 0;
static usart_callback_t usart_callback = NULL;

static void prvSendData(char *buf, int bufsz);
static int prvTryOpenPort(const char* intf);
static int prvTryConfigurePort(const struct usart_conf*);
static int prvTrySetPortTimeouts(void);
static const char* prvParityToStr(BYTE paritySetting);

#ifdef CSP_DEBUG
static void prvPrintError(void) {
    char *messageBuffer = NULL;
    DWORD errorCode = GetLastError();
    DWORD formatMessageRet;
    formatMessageRet = FormatMessageA(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM,
        NULL,
        errorCode,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (char*)&messageBuffer,
        0,
        NULL);

    if( !formatMessageRet ) {
        csp_log_error("FormatMessage error, code: %lu", GetLastError());
        return;
    }
    csp_log_error("%s", messageBuffer);
    LocalFree(messageBuffer);
}
#endif

#ifdef CSP_DEBUG
#define printError() prvPrintError()
#else
#define printError() do {} while(0)
#endif

static int prvTryOpenPort(const char *intf) {
    //Apparently we must use \\.\COMx for com ports above 9, but works for all.
    char * fullName = (char*) malloc(strlen(intf)+5);
    snprintf(fullName,strlen(intf)+5,"\\\\.\\%s",intf);

    portHandle = CreateFileA(
        fullName, 
        GENERIC_READ|GENERIC_WRITE,
        0,
        0,
        OPEN_EXISTING,
        FILE_FLAG_OVERLAPPED,
        0);

    if( portHandle == INVALID_HANDLE_VALUE ) {
        DWORD errorCode = GetLastError();
        if( errorCode == ERROR_FILE_NOT_FOUND ) {
            csp_log_error("Could not open serial port, because it didn't exist!");
        }
        else
            csp_log_error("Failure opening serial port! Code: %lu", errorCode);
        return 1;
    }
    return 0;
}

static int prvTryConfigurePort(const struct usart_conf * conf) {
    DCB portSettings = {0};
    portSettings.DCBlength = sizeof(DCB);
    if(!GetCommState(portHandle, &portSettings) ) {
        csp_log_error("Could not get default settings for open COM port! Code: %lu", GetLastError());
        return -1;
    }
    portSettings.BaudRate = conf->baudrate;
    portSettings.Parity = conf->paritysetting;
    portSettings.StopBits = conf->stopbits;
    portSettings.fParity = conf->checkparity;
    portSettings.fBinary = TRUE;
    portSettings.ByteSize = conf->databits;
    if( !SetCommState(portHandle, &portSettings) ) {
        csp_log_error("Error when setting COM port settings! Code:%lu", GetLastError());
        return 1;
    }

    GetCommState(portHandle, &portSettings);

    csp_log_info("Port: %s, Baudrate: %lu, Data bits: %d, Stop bits: %d, Parity: %s",
            conf->device, conf->baudrate, conf->databits, conf->stopbits, prvParityToStr(conf->paritysetting));
    return 0;
}

static const char* prvParityToStr(BYTE paritySetting) {
    static const char *parityStr[] = {
        "None",
        "Odd",
        "Even",
        "N/A"
    };
    char const *resultStr = NULL;

    switch(paritySetting) {
        case NOPARITY:
            resultStr = parityStr[0];
            break;
        case ODDPARITY:
            resultStr = parityStr[1];
            break;
        case EVENPARITY:
            resultStr = parityStr[2];
            break;
        default:
            resultStr = parityStr[3];
    };
    return resultStr;
}

static int prvTrySetPortTimeouts(void) {
    COMMTIMEOUTS timeouts = {0};

    if( !GetCommTimeouts(portHandle, &timeouts) ) {
        csp_log_error("Error gettings current timeout settings");
        return 1;
    }

    timeouts.ReadIntervalTimeout = 5;
    timeouts.ReadTotalTimeoutMultiplier = 1;
    timeouts.ReadTotalTimeoutConstant = 5;
    timeouts.WriteTotalTimeoutMultiplier = 1;
    timeouts.WriteTotalTimeoutConstant = 5;

    if(!SetCommTimeouts(portHandle, &timeouts)) {
        csp_log_error("Error setting timeouts!");
        return 1;
    }

    return 0;
}


unsigned WINAPI prvRxTask(void* params) {
    
	if (!SetCommMask(portHandle, EV_RXCHAR))
	{
		printf("Failed to Set Comm Mask. Reason: %d", GetLastError());
		return 0;
	}

	// Create an event to use for notifications from the com port
	HANDLE serialEvent = CreateEvent(
		0,			// Event Attributes
		true,		// Manual Reset
		FALSE,		// Initial State
		0);			// Name

	// Create a separate event to wait for reads to finish
	HANDLE readEvent = CreateEvent(
		0,			// Event Attributes
		true,		// Manual Reset
		FALSE,		// Initial State
		0);			// Name

	// Create the overlapped structure to wait for events from the serial port
	OVERLAPPED ov;
	memset(&ov, 0, sizeof(ov));
	ov.hEvent = serialEvent;

	// Set the two events that this thread will wait for
	HANDLE arHandles[1];
	arHandles[0] = ov.hEvent;

	// Reserve space in a buffer to receive the incoming data.  Size is arbitrary
	uint8_t incomingDataBuffer[1024];
    uint16_t bufferIdx =0;

	bool keepRunning = true;

	while (keepRunning)
	{
		DWORD comEventMask = 0;	// NOTE: this is different from the value passed to SecCommMask

		BOOL abRet = WaitCommEvent(portHandle, &comEventMask, &ov);
		if (!abRet)
		{
			DWORD errorCode = GetLastError();
			if (errorCode != ERROR_IO_PENDING)
			{
				printf("Error occurred reading from COM port: %d", errorCode);
				continue;
			}
		}

		DWORD waitResult = WaitForSingleObject( ov.hEvent, INFINITE);
		switch (waitResult)
		{
			// If the thread exit event is the one that was triggered...
			case WAIT_OBJECT_0:
			{
				// Allow the thread to end by returning from this function
				keepRunning = false;
				continue;
			}
			break;

			// If the comm event is the one that was triggered...
			case WAIT_OBJECT_0 + 1:
			{
				// Read all the available data from the comm port into a buffer

				// Clear the buffer that accumulates the data
				// incomingDataBuffer.Clear();
                memset(incomingDataBuffer,0,1024);

				OVERLAPPED ovRead;
				memset(&ovRead, 0, sizeof(ovRead));
				ovRead.hEvent = readEvent;

				byte szTmp[1024];
				int iSize = sizeof(szTmp);
				memset(szTmp, 0, iSize);

				DWORD bytesRead = 0;

				do
				{
					memset(szTmp, 0, iSize);
					bytesRead = 0;

					if (!ReadFile(portHandle, szTmp, iSize, NULL, &ovRead))
					{
						DWORD errorCode = GetLastError();
						if (errorCode == ERROR_IO_PENDING)
						{
							WaitForSingleObject(ovRead.hEvent, INFINITE);

							if (!GetOverlappedResult(portHandle, &ovRead, &bytesRead, false))
							{
								printf("Error waiting for read from serial port");
								break;
							}
						}
						else
						{
							printf("Error reading from serial port");
							break;
						}
					}

					if (bytesRead > 0)
					{
                        memcpy(&incomingDataBuffer[bufferIdx],szTmp,bytesRead);
                        bufferIdx += bytesRead;
						//incomingDataBuffer.Append(szTmp, bytesRead);
					}

				} while (bytesRead > 0);

				// // Pass the buffered data to any listeners
				// if (incomingDataBuffer.GetSize() > 0)
				// {
				// 	if (m_readCallback.IsBound())
				// 	{
				// 		m_readCallback.Call(incomingDataBuffer);
				// 	}
				// }
                  if( usart_callback != NULL ){
                       usart_callback(incomingDataBuffer, (size_t)bufferIdx, NULL);
                       bufferIdx =0;
                  }
			}
			break;
		}
	}

	// Close the events before the thread exits
	CloseHandle(serialEvent);
	CloseHandle(readEvent);
}

static void prvSendData(char *buf, int bufsz) {
    // DWORD bytesTotal = 0;
    // DWORD bytesActual;
    // if( !WriteFile(portHandle, buf, bufsz-bytesTotal, &bytesActual, NULL) ) {
    //     csp_log_error("Could not write data. Code: %lu", GetLastError());
    //     return;
    // }
    // if( !FlushFileBuffers(portHandle) ) {
    //     csp_log_warn("Could not flush write buffer. Code: %lu", GetLastError());
    // }

   OVERLAPPED osWrite = {0};
   DWORD dwWritten;
   DWORD dwRes;
   BOOL fRes;

   // Create this write operation's OVERLAPPED structure's hEvent.
   osWrite.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
   //if (osWrite.hEvent == NULL)return FALSE;

   // Issue write.
   if (!WriteFile(portHandle, buf, bufsz, &dwWritten, &osWrite)) {
      if (GetLastError() != ERROR_IO_PENDING) { 
         // WriteFile failed, but isn't delayed. Report error and abort.
         fRes = FALSE;
      }
      else
         // Write is pending.
         dwRes = WaitForSingleObject(osWrite.hEvent, INFINITE);
         switch(dwRes)
         {
            // OVERLAPPED structure's event has been signaled. 
            case WAIT_OBJECT_0:
                 if (!GetOverlappedResult(portHandle, &osWrite, &dwWritten, FALSE))
                       fRes = FALSE;
                 else
                  // Write operation completed successfully.
                  fRes = TRUE;
                 break;
            
            default:
                 // An error has occurred in WaitForSingleObject.
                 // This usually indicates a problem with the
                // OVERLAPPED structure's event handle.
                 fRes = FALSE;
                 break;
         }
      }
   else  fRes = TRUE;
   
   CloseHandle(osWrite.hEvent);
  // return fRes;

}

void usart_shutdown(void) {
    InterlockedExchange(&isListening, 0);
    CloseHandle(portHandle);
    portHandle = INVALID_HANDLE_VALUE;
    if( rxThread != INVALID_HANDLE_VALUE ) {
        WaitForSingleObject(rxThread, INFINITE);
        rxThread = INVALID_HANDLE_VALUE;
    }
    DeleteCriticalSection(&txSection);  
}

void usart_listen(void) {
    InterlockedExchange(&isListening, 1);
    rxThread = (HANDLE)_beginthreadex(NULL, 0, &prvRxTask, NULL, 0, NULL);
}

void usart_putstr(char* buf, int bufsz) {
    EnterCriticalSection(&txSection);
    prvSendData(buf, bufsz);
    LeaveCriticalSection(&txSection);
}

void usart_putc(char c) {
    EnterCriticalSection(&txSection);
    prvSendData(&c, 1);
    LeaveCriticalSection(&txSection);
}

void usart_insert(char c, void *pxTaskWoken) {
    /* redirect debug output to stdout */
    printf("%c", c);
}

void usart_set_callback(usart_callback_t callback) {
    usart_callback = callback;
}

void usart_init(struct usart_conf * conf) {
    if( prvTryOpenPort(conf->device) ) {
        printError();
        return;
    }

    if( prvTryConfigurePort(conf) ) {
        printError();
        return;
    }

    if( prvTrySetPortTimeouts() ) {
        printError();
        return;
    }

    InitializeCriticalSection(&txSection);

    /* Start receiver thread */
    usart_listen();
}


