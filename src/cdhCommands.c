#include "cdhCommands.h"
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <csp/csp.h>
#include <csp/csp_types.h>
#include <stdlib.h>
#include <string.h>
#include "csp_client.h"
#include <csp/arch/csp_thread.h>
#include "csp_client.h"
#include "networkConfig.h"


void getCdhTelemetry(int argc, char **argv){}
void checkCdhTelemetry(int argc, char **argv){}
void requestCdhTelemetry(int argc, char **argv){}
void scheduleTTT(int argc, char **argv){

    uint8_t telemPacket[10+9];
    telemPacket[8] = CDH_SCHEDULE_TTT_CMD;
    telemPacket[9] = 5;
    telemPacket[10] = atoi(argv[1]);
    Calendar_t when={0};
    when.day= atoi(argv[2]);
    when.hour = atoi(argv[3]);
    when.minute = atoi(argv[4]);
    
    memcpy(&telemPacket[11],&when,8);

    csp_conn_t * conn;
	csp_packet_t * outPacket;
	conn = csp_connect(2,CDH_CSP_ADDRESS,CSP_CMD_PORT,1000,0);	//Create a connection. This tells CSP where to send the data (address and destination port).
	outPacket = csp_buffer_get(19);

	memcpy(&outPacket->data[0],telemPacket,19);
    

	outPacket->length = 19;

    int result = csp_send(conn,outPacket,1000);
}
void cancelTTT(int argc,char **argv){}
