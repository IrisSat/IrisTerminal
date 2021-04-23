#include <stdio.h>
#include <windows.h>
#include <process.h>
#include <csp/csp.h>
#include <csp/drivers/usart.h>
#include "libserialport.h"

//This is an updated version of the CSP windows kiss/usart driver.
//It uses libserialport

static HANDLE portHandle = INVALID_HANDLE_VALUE;
static HANDLE rxThread = INVALID_HANDLE_VALUE;
static CRITICAL_SECTION txSection;
static LONG isListening = 0;
static usart_callback_t usart_callback = NULL;

struct sp_port* serialPort;
struct sp_event_set *event_set;

static int prvTryOpenPort(const char* intf);
static int prvTryConfigurePort(const struct usart_conf * conf);
static const char* prvParityToStr(BYTE paritySetting);
void usart_listen(void);


int prvTryOpenPort(const char* intf){

    
    int res = sp_get_port_by_name(intf,&serialPort);
    if(res != SP_OK){
        printf("Error getting serial port by nam. Code: %d\n",res);
        return res;
    }

    printf("Opening serial port.");
    res = sp_open(serialPort,SP_MODE_READ_WRITE);
    if(res != SP_OK){
        printf("Error opening serial port. Code: %d\n",res);
        return res;
    }


    return 0;
}

static int prvTryConfigurePort(const struct usart_conf * conf){

    
    sp_set_baudrate(serialPort, conf->baudrate);
    sp_set_bits(serialPort, conf->databits);
    sp_set_parity(serialPort, conf->paritysetting);
    sp_set_stopbits(serialPort, conf->stopbits);
    sp_set_flowcontrol(serialPort, SP_FLOWCONTROL_NONE);

     csp_log_info("Port: %s, Baudrate: %lu, Data bits: %d, Stop bits: %d, Parity: %s",
            conf->device, conf->baudrate, conf->databits, conf->stopbits, prvParityToStr(conf->paritysetting));
    return 0;

}
void usart_set_callback(usart_callback_t callback) {
    usart_callback = callback;
}


void usart_putc(char c) {
    EnterCriticalSection(&txSection);
    sp_blocking_write(serialPort, &c, 1, 1000);
    LeaveCriticalSection(&txSection);
}



void usart_init(struct usart_conf * conf) {

    if( prvTryOpenPort(conf->device) ) {
        
        return;
    }

    if( prvTryConfigurePort(conf) ) {
        
        return;
    }

    sp_new_event_set(&event_set);
    sp_add_port_events(event_set, serialPort, SP_EVENT_RX_READY);

    // if( prvTrySetPortTimeouts() ) {
    //     printError();
    //     return;
    // }

    InitializeCriticalSection(&txSection);

    /* Start receiver thread */
    usart_listen();

}

unsigned WINAPI prvRxTask(void* params){

    uint8_t rxBuff[1000];
    int bytesRead;
    while(isListening){

    //     sp_wait(event_set, 0);
    //    bytesWaiting =  sp_input_waiting(serialPort)
    bytesRead = sp_blocking_read_next(serialPort,rxBuff,1000,0);

        if( usart_callback != NULL ){
                       usart_callback(rxBuff, (size_t)bytesRead, NULL);
                      
        }
    }
    printf("Closing rx thread.\n");
    return 0;
}

void usart_insert(char c, void *pxTaskWoken) {
    /* redirect debug output to stdout */
    printf("%c", c);
}

void usart_listen(void) {
    InterlockedExchange(&isListening, 1);
    rxThread = (HANDLE)_beginthreadex(NULL, 0, &prvRxTask, NULL, 0, NULL);
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