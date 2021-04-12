#include <stdio.h>
#include <csp/csp.h>
#include <csp/interfaces/csp_if_kiss.h>

#include <csp/drivers/usart.h>
#include <csp/arch/csp_thread.h>

#include "csp_client.h"

#define PORT 10
#define MY_ADDRESS 1
#define CDH_ADDRESS 0

#define SERVER_TIDX 0
#define CLIENT_TIDX 1
#define USART_HANDLE 0

static csp_iface_t csp_if_kiss;
	/* Setup callback from USART RX to KISS RS */
	void my_usart_rx(uint8_t * buf, int len, void * pxTaskWoken) {
		csp_kiss_rx(&csp_if_kiss, buf, len, pxTaskWoken);
	}

int startcsp(char * comPort,int baudRate){
    csp_debug_toggle_level(CSP_PACKET);
    csp_debug_toggle_level(CSP_INFO);

    int res = csp_buffer_init(10, 300);
    if(res != CSP_ERR_NONE){
        printf("ERROR");
        return -1;
    }

    res = csp_init(MY_ADDRESS);
    if(res != CSP_ERR_NONE){
        printf("ERROR");
        return -1;
    }

    struct usart_conf conf;

    #if defined(CSP_WINDOWS)
        conf.device = comPort;
        conf.baudrate = baudRate;
        conf.databits = 8;
        conf.paritysetting = NOPARITY;
        conf.stopbits = ONESTOPBIT;
        conf.checkparity = FALSE;
    #elif defined(CSP_POSIX)
        conf.device = argc != 2 ? "/dev/ttyUSB0" : argv[1];
        conf.baudrate = 500000;
    #elif defined(CSP_MACOSX)
        conf.device = argc != 2 ? "/dev/tty.usbserial-FTSM9EGE" : argv[1];
        conf.baudrate = 115200;
    #endif

	/* Run USART init */
	usart_init(&conf);

    /* Setup CSP interface */
	
	static csp_kiss_handle_t csp_kiss_driver;
	csp_kiss_init(&csp_if_kiss, &csp_kiss_driver, usart_putc, usart_insert, "KISS");
		

	usart_set_callback(my_usart_rx);

    res = csp_route_set(0, &csp_if_kiss, CSP_NODE_MAC);
        if(res != CSP_ERR_NONE){
        printf("ERROR");
        return -1;
    }
    res = csp_route_start_task(0, 0);
        if(res != CSP_ERR_NONE){
        printf("ERROR");
        return -1;
    }
    
    return 0;
}

CSP_DEFINE_TASK(task_client) {

    char outbuf = 'q';
    char inbuf[3] = {0};
    int pingResult;

    for(int i = 50; i <= 200; i+= 50) {
        pingResult = csp_ping(CDH_ADDRESS, 360000, 100, CSP_O_NONE);
        printf("Ping with payload of %d bytes, took %d ms\n", i, pingResult);
        csp_sleep_ms(1000);
    }
    csp_ps(CDH_ADDRESS, 1000);
    csp_sleep_ms(1000);
    csp_memfree(CDH_ADDRESS, 1000);
    csp_sleep_ms(1000);
    csp_buf_free(CDH_ADDRESS, 1000);
    csp_sleep_ms(1000);
    csp_uptime(CDH_ADDRESS, 1000);
    csp_sleep_ms(1000);

    // csp_transaction(0, CDH_ADDRESS, PORT, 1000, &outbuf, 1, inbuf, 2);
    // printf("Quit response from server: %s\n", inbuf);
    printf("Done task\n");

    return CSP_TASK_RETURN;
}

CSP_DEFINE_TASK(task_ping){
        int* pingAddr= (int*)param;
        int pingResult;
        pingResult = csp_ping(*pingAddr, 60000, 100, CSP_O_NONE);
        printf("Ping with payload of %d bytes, took %d ms\n", 100, pingResult);
        return CSP_TASK_RETURN;
}

void ping(int argc,char **argv){

    if(argc != 2){
        printf("Not enough inputs\n");

    }
    else{
        int pingAddr = atoi(argv[1]);
    // csp_thread_handle_t handle_client;
    // csp_thread_create(task_client, "PING", 1000, (void*)&pingAddr, 0, &handle_client);
        int pingResult;
        pingResult = csp_ping(pingAddr, 60000, 100, CSP_O_NONE);
        printf("Ping with payload of %d bytes, took %d ms\n", 100, pingResult);
    }



}
void runDiagnostics(int argc,char **argv){}
void getNetworkInfo(int argc,char **argv){}