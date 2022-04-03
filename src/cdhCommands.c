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
#include "telemetry.h"
#include <time.h>

typedef enum
{
    TASK_LOG,
    CDH_TELEMETRY_LOG,
    CDH_ERROR_LOG,
    NUM_LOGS
} LogFile_t;

const char * LogFileList[] =
{
    "cdhTasks",
    "cdhTelem",
    "cdhError"
};

void getCdhTelemetry(int argc, char **argv){}
void checkCdhTelemetry(int argc, char **argv){}
void requestCdhTelemetry(int argc, char **argv){}

uint8_t decodeLogFileNames(char * arg)
{
    uint8_t i;
    for(i=0; i < NUM_LOGS; i++)
    {
        if(strcmp(arg,LogFileList[i]) == 0)
            return i;
    }
    return -1;
}

void downloadLogFile(int argc, char **argv){
    
    if(argc == 2)
    {
        if(strcmp(argv[1],"--help") == 0){
            printf("downloadLogFile help:\n");
            printf("-Command format: downloadLogFile [arg]\n");
            printf("-Possible arguments:\n");
            int i;
            for(i=0; i < NUM_LOGS; i++)
                printf(" - %s\n",LogFileList[i]);
            return;
        }
        // Get log file name
        int log_file_id = decodeLogFileNames(argv[1]);
        if(log_file_id < 0){
            printf("Invalid argument.\n");
            printf("Enter the following command for valid arguments: downloadLogFile --help\n");
            return;
        }
        // Create and send packet
        telemetryPacket_t cmd;
        cmd.telem_id = CDH_DOWNLOAD_LOG_FILE_CMD;
        cmd.length = 1 * sizeof(uint8_t);
        cmd.data[0] = log_file_id;
        printf("Downloading log file: %s\n",LogFileList[log_file_id]);
        sendCommand(&cmd,CDH_CSP_ADDRESS);
    }
    else
    {
        printf("Invalid command (improper number of arguments - 1 required).\n");
        printf("Enter the following command for valid log files: downloadLogFile --help\n");
    }
}

void scheduleTTT(int argc, char **argv){

    telemetryPacket_t cmd;

    //Set command timestamp to now.
    Calendar_t now;
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    timeToCalendar(tm,&now);
    cmd.timestamp = now;

    cmd.telem_id = CDH_SCHEDULE_TTT_CMD;
    cmd.length = 2*sizeof(uint8_t)+ sizeof(Calendar_t); //We need to send the task code, and when to execute.

    uint8_t cmd_data[2*sizeof(uint8_t)+ sizeof(Calendar_t)] = {0};
    cmd.data = cmd_data;

    cmd_data[0] = atoi(argv[1]); //First arg is the task code.
    cmd_data[1] = atoi(argv[2]); // Second arg is the task parameter.
    // char * test = argv[2];
    if(argc > 4){
        Calendar_t when = {0};
        when.day = atoi(argv[2]);
        when.hour = atoi(argv[3]);
        when.minute = atoi(argv[4]);
        when.month = atoi(argv[5]);
        when.year = atoi(argv[6]);
        when.weekday = 0; //This really isn't ever used and is just a pain, set to 0...
        when.weekday = 1; //Not used but must be 1.
        
        memcpy(&cmd_data[1],&when,sizeof(Calendar_t));
    }
    else if(strcmp(argv[3],"now")==0){
        memcpy(&cmd_data[2],&now,sizeof(Calendar_t)); //Just copy the current time.
    }
    else{
        printf("Wrong number of args... try scheduleTTT help for instructions on how to use this function.\n");
        return;
    }

    sendCommand(&cmd,CDH_CSP_ADDRESS);
    // csp_conn_t * conn;
	// csp_packet_t * outPacket;
	// conn = csp_connect(2,CDH_CSP_ADDRESS,CSP_CMD_PORT,1000,0);	//Create a connection. This tells CSP where to send the data (address and destination port).
	// outPacket = csp_buffer_get(19);

	// memcpy(&outPacket->data[0],telemPacket,19);
    

	// outPacket->length = 19;

    // int result = csp_send(conn,outPacket,1000);
}


void scheduleTTT_raw(int code,Calendar_t when){

    telemetryPacket_t cmd;

    cmd.telem_id = CDH_SCHEDULE_TTT_CMD;
    cmd.length = sizeof(uint8_t)+ sizeof(Calendar_t); //We need to send the task code, and when to execute.

    uint8_t cmd_data[sizeof(uint8_t)+ sizeof(Calendar_t)] = {0};
    cmd.data = cmd_data;

    cmd_data[0] = code;//First arg is the task code.
    
    memcpy(&cmd_data[1],&when,sizeof(Calendar_t));


    sendCommand(&cmd,CDH_CSP_ADDRESS);
}

void cancelTTT(int argc,char **argv){}

void setCdhTime(int argc, char **argv){

    if(strcmp(argv[1],"help") ==0){
        printf("This command can be used to set the time on CDH. \n");
        printf("Should only be needed for testing or inital setup, since the external RTC can keep time with battery.\n");
        printf("Usage: setCdhTime now\n");
        printf("Usage: setCdhTime <s> <min> <hr> <day> <month> <year>\n");
        printf("Time Format: 24 hour, month 1-12, year since 2000\n");
        printf("Example: So to set the time to June 4 2021 at 6:30:00 pm\n");
        printf("\t setCdhTime 0 30 18 1 6 21");

        return;
    }

    telemetryPacket_t cmd;

    //Set command timestamp to now.
    Calendar_t now;
    getCalendarNow(&now);
    cmd.timestamp = now;

    cmd.telem_id = CDH_SET_TIME_CMD;
    cmd.length = sizeof(Calendar_t); //We send an updated time.

    uint8_t cmd_data[sizeof(Calendar_t)] = {0};
    cmd.data = cmd_data;

    char * test = argv[1];
    if(argc >2){
        Calendar_t when={0};
        when.second= atoi(argv[2]);
        when.minute = atoi(argv[3]);
        when.hour = atoi(argv[4]);
        when.day = atoi(argv[5]);
        when.month = atoi(argv[6]);
        when.year = atoi(argv[7]);
        when.weekday = 0; //This really isn't ever used and is just a pain, set to 0...
        when.weekday =1; //Not used but must be 1.
        
        memcpy(cmd_data,&when,sizeof(Calendar_t));
    }
    else if(strcmp(argv[1],"now")==0){
        memcpy(cmd_data,&now,sizeof(Calendar_t)); //Just copy the current time.
    }
    else{
        printf("Wrong number of args... try setCdhTime help for instructions on how to use this function.\n");
        return;
    }

    sendCommand(&cmd,CDH_CSP_ADDRESS);
}

void getCdhTime(int argc, char **argv){

    telemetryPacket_t request;

    request.telem_id = CDH_GET_TIME_CMD;
    Calendar_t now;
    getCalendarNow(&now);
    request.timestamp = now;
    request.length=0;
    request.data=NULL;

    telemetryPacket_t response;

    int result = csp_transaction(2,CDH_CSP_ADDRESS,CSP_CMD_PORT,5000,&request,TELEM_HEADER_SIZE,&response,TELEM_HEADER_SIZE);
    if(result<=0){
        printf("csp transaction error: %d\n",result);
    }
    else{
        printCalendar(&response.timestamp);

    }
}

