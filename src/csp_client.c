#include <stdio.h>
#include <csp/csp.h>
#include <csp/interfaces/csp_if_kiss.h>

#include <csp/drivers/usart.h>
#include <csp/arch/csp_thread.h>

#include <math.h>

#include "csp_client.h"
#include "networkConfig.h"
#include "telemetry.h"

#include "telemetry_handlers.h"


#define PORT 10
#define MY_ADDRESS 9
#define CDH_ADDRESS 0

#define SERVER_TIDX 0
#define CLIENT_TIDX 1
#define USART_HANDLE 0

static csp_iface_t csp_if_kiss;
/* Setup callback from USART RX to KISS RS */
void my_usart_rx(uint8_t * buf, int len, void * pxTaskWoken) {
    csp_kiss_rx(&csp_if_kiss, buf, len, pxTaskWoken);
}

// int imageDownloadState=0;
// char imageDownloadFile[100] = "";
// FILE * imageFileHandle;
// int imageChunksLeft =0;
// int imageChunksRcvd = 0;
// int prevDownloadPercentage = 100;
// int MAX_IMAGE_CHUNKS = 2700;
// int IMAGE_PERCENT_DELTA = 2;

CSP_DEFINE_TASK(task_server) {

    int running = 1;
    csp_socket_t *socket = csp_socket(CSP_SO_NONE);
    csp_conn_t *conn;
    csp_packet_t *packet;
    csp_packet_t *response;

    csp_bind(socket, CSP_ANY);
    csp_listen(socket, 10);

    printf("\nServer task started\r\n");

    while(running) {
        if( (conn = csp_accept(socket, 10000)) == NULL ) {
            continue;
        }

        while( (packet = csp_read(conn, 100)) != NULL ) {

            switch( csp_conn_dport(conn) ) {
                case CSP_TELEM_PORT:{
                    
                    int src = csp_conn_src(conn);

                    switch(src) {
                        case CDH_CSP_ADDRESS:{
                            CdhTelemetryHandler(conn,packet);
                            break;
                        }
                        case PAYLOAD_CSP_ADDRESS:{
                            PayloadTelemetryHandler(conn,packet);
                            break;
                        }
                        default:{
                            printf("CSP message received from %d.\n",src);
                            break;
                        }
                    }
                    printf("Iris>");
                } // case: CSP_TELEM_PORT
                // default:
                //     csp_service_handler(conn, packet);
                //     break;
            } // switch: csp_conn_dport(conn)
            csp_buffer_free(packet);
        } // while: csp_read

        csp_close(conn);
    } // while: running

    csp_buffer_free(response);

    return CSP_TASK_RETURN;
}
// CSP_DEFINE_TASK(task_client) {

//     char outbuf = 'q';
//     char inbuf[3] = {0};
//     int pingResult;

//     for(int i = 50; i <= 200; i+= 50) {
//         pingResult = csp_ping(CDH_ADDRESS, 360000, 100, CSP_O_NONE);
//         printf("Ping with payload of %d bytes, took %d ms\n", i, pingResult);
//         csp_sleep_ms(1000);
//     }
//     csp_ps(CDH_ADDRESS, 1000);
//     csp_sleep_ms(1000);
//     csp_memfree(CDH_ADDRESS, 1000);
//     csp_sleep_ms(1000);
//     csp_buf_free(CDH_ADDRESS, 1000);
//     csp_sleep_ms(1000);
//     csp_uptime(CDH_ADDRESS, 1000);
//     csp_sleep_ms(1000);

//     // csp_transaction(0, CDH_ADDRESS, PORT, 1000, &outbuf, 1, inbuf, 2);
//     // printf("Quit response from server: %s\n", inbuf);
//     printf("Done task\n");

//     return CSP_TASK_RETURN;
// }

CSP_DEFINE_TASK(task_ping){
        int* pingAddr= (int*)param;
        int pingResult;
        pingResult = csp_ping(*pingAddr, 60000, 200, CSP_O_NONE);
        //printf("Ping with payload of %d bytes, took %d ms\n", 100, pingResult);
        return CSP_TASK_RETURN;
}

void ping(int argc,char **argv){

    if(argc < 2){
        printf("Not enough inputs\n");

    }
    else{

        //Get the address from the user input
        int pingAddr = getAddr(argv[1]);
        if(pingAddr<0){ 
            printf("Invalid addresss or subsystem...\n");
            return;
        }

        //Check if the user specified the ping payload size.
        int len = 100;
        if(argc == 3){
            len = atoi(argv[2]);
        }

        int pingResult;
        pingResult = csp_ping(pingAddr, 5000, len, CSP_O_NONE);
        printf("Ping with payload of %d bytes, took %d ms\n", len, pingResult);
    }
}

void listProcess(int argc,char **argv){

    if(argc != 2){
        printf("Not enough inputs\n");

    }
    else{

        int addr = getAddr(argv[1]);
        if(addr<0){ 
            printf("Invalid addresss or subsystem...\n");
            return;
        }

        csp_ps(addr,5000);
    }
}


void memFree(int argc,char **argv){

    if(argc != 2){
        printf("Not enough inputs\n");

    }
    else{

        int addr = getAddr(argv[1]);
        if(addr<0){ 
            printf("Invalid addresss or subsystem...\n");
            return;
        }

        csp_memfree(addr,5000);
    }
}

void buffFree(int argc,char **argv){

    if(argc != 2){
        printf("Not enough inputs\n");

    }
    else{

        int addr = getAddr(argv[1]);
        if(addr<0){ 
            printf("Invalid addresss or subsystem...\n");
            return;
        }

        csp_buf_free(addr,5000);
    }
}

void uptime(int argc,char **argv){

    if(argc != 2){
        printf("Not enough inputs\n");

    }
    else{

        int addr = getAddr(argv[1]);
        if(addr<0){ 
            printf("Invalid addresss or subsystem...\n");
            return;
        }

        csp_uptime(addr,5000);
    }
}

void runDiagnostics(int argc,char **argv){}
void getNetworkInfo(int argc,char **argv){}


int csp_transaction_2port(uint8_t prio, uint8_t dest, uint8_t port, uint8_t rx_port, uint32_t timeout, void * outbuf, int outlen, void * inbuf, int inlen) {

    // csp_conn_t * conn = csp_connect(prio, dest, port, 0, CSP_CONNECTION_SO);

	// int size = (inlen > outlen) ? inlen : outlen;
	// csp_packet_t * packet = csp_buffer_get(size);
	// if (packet == NULL){
    //     csp_close(conn);
	// 	return 0;
    // }

	// /* Copy the request */
	// if (outlen > 0 && outbuf != NULL)
	// 	memcpy(packet->data, outbuf, outlen);
	// packet->length = outlen;

	// if (!csp_send(conn, packet, timeout)) {
	// 	csp_buffer_free(packet);
    //     csp_close(conn);
	// 	return 0;
	// }

    // // csp_close(conn);
	// /* If no reply is expected, return now */
	// if (inlen == 0)
	// 	return 1;

    // // conn = csp_connect(prio, dest, rx_port, 0, CSP_CONNECTION_SO);
	// packet = csp_read(conn, timeout);
	// if (packet == NULL){
    //     csp_close(conn);
	// 	return 0;
    //     }

	// if ((inlen != -1) && ((int)packet->length != inlen)) {
	// 	csp_log_error("Reply length %u expected %u", packet->length, inlen);
	// 	csp_buffer_free(packet);
    //     csp_close(conn);
	// 	return 0;
	// }

	// memcpy(inbuf, packet->data, packet->length);
	// int length = packet->length;
	// csp_buffer_free(packet);
    // csp_close(conn);
	// return length;

}

int startcsp(char * comPort,int baudRate){
    // csp_debug_toggle_level(CSP_PACKET);
    csp_debug_toggle_level(CSP_INFO);

    int res = csp_buffer_init(100, 500);
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

    res = csp_route_set(COMMS_CSP_ADDRESS, &csp_if_kiss, CSP_NODE_MAC);
    
        if(res != CSP_ERR_NONE){
        printf("ERROR");
        return -1;
    }

    res = csp_route_set(CDH_CSP_ADDRESS, &csp_if_kiss, CSP_NODE_MAC);
    
        if(res != CSP_ERR_NONE){
        printf("ERROR");
        return -1;
    }
	
    res = csp_route_set(PAYLOAD_CSP_ADDRESS, &csp_if_kiss, CSP_NODE_MAC);
    
        if(res != CSP_ERR_NONE){
        printf("ERROR");
        return -1;
    }

    res = csp_route_set(GROUND_CSP_ADDRESS, &csp_if_kiss, CSP_NODE_MAC);
    
        if(res != CSP_ERR_NONE){
        printf("ERROR");
        return -1;
    }
	
    // send all messages to KISS by default
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

    csp_thread_handle_t handle_server;
    csp_thread_create(task_server, "SERVER", 1000, NULL, 0, &handle_server);

    csp_conn_print_table();
    csp_route_print_table();
    csp_route_print_interfaces();
    
    return 0;
}
