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
#include <checksum.h>


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

typedef struct{
    uint8_t fileIndex; //Valid options are 0(GOLDEN image) or 1 (UPDATE image)
    uint32_t filesize;//Filesize in bytes. For use in receiving and as extra check.
    uint32_t checksum;//crc-32 full file checksum.

}Fw_metadata_t;

void checksum_file(uint32_t * out, FILE * in);//Out must be pointer to 32 bit int where result will go. In must be opened file, seek will be set back to start on function return.
void cdhGetTelemetry(int argc, char **argv){}
void cdhCheckTelemetry(int argc, char **argv){}
void cdhRequestTelemetry(int argc, char **argv){}

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

void cdhSetTime(int argc, char **argv){

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

void cdhGetTime(int argc, char **argv){

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

void cdhFileList(int argc, char **argv){

    telemetryPacket_t cmd;
    //Set command timestamp to now.
    Calendar_t now;
    getCalendarNow(&now);
    cmd.timestamp = now;
    cmd.telem_id = CDH_LIST_FILES_CMD;
    cmd.length = 0; //We send an updated time.
    sendCommand(&cmd,CDH_CSP_ADDRESS);
}

int cmdRead(char *cmd,char *output, int outLen)
{
    FILE *p = popen(cmd, "r");
    if (p == NULL) return 0;
    char buf[256]={0};
    int left = outLen;
    while(fgets(buf,256,p) != NULL){

        strncpy(output,buf,left);
        left= left-strlen(buf);
    }
    pclose(p);
    
}



void cdhUploadFw(int argc, char **argv){

    if(argc != 3){
        printf("incorrect parameteres...");
    }
    else{

        printf("Sending fw to the cdh board...\n");

        const int chunkSize = 150; //CAN MTU is 255? 
        char* fileName = argv[1];
        FILE * fwFile;
        fwFile = fopen(fileName,"rb");
        if(fwFile == NULL){
            printf("Error opneing file %s\n",argv);
            goto END;
        }

        fseek(fwFile, 0, SEEK_END); // seek to end of file
        long fileSize_B= ftell(fwFile); // get current file pointer
        fseek(fwFile, 0, SEEK_SET); // seek back to beginning of file

        double numChunks_dec = (double)fileSize_B/(double)chunkSize;
        int numChunks = (int)ceil(numChunks_dec);

        printf("Splitting the fw(%dkiB) into %d (%f) chunks of size %d B.\n",fileSize_B/1024,numChunks,numChunks_dec,chunkSize);
        // proceed with allocating memory and reading the file

        uint8_t* chunk;
        chunk = malloc(chunkSize);
        if(chunk == NULL){
            printf("Error allocating memory... Download some more RAM!\n");
        }

        //Start by telling cdh fw_update_mgr to enter the "receiveing update" state, along with basic info...
        Fw_metadata_t fw_info;
        fw_info.fileIndex =atoi(argv[2]);
        fw_info.filesize = fileSize_B;
        
        //Get checksum for file...
        uint32_t checksum;
        char cmd_str[128];
        checksum_file(&checksum,fwFile);
        printf("fw checksum: %0x\n",checksum);
        fw_info.checksum = checksum;

        printf("Uploading file %s (%db) with checksum %0x to slot %d\n",fileName,fw_info.filesize,fw_info.checksum,fw_info.fileIndex);
        
        telemetryPacket_t cmd;
        //Set command timestamp to now.
        Calendar_t now;
        getCalendarNow(&now);
        cmd.timestamp = now;
        cmd.telem_id = CDH_FW_RX_FW_CMD;
        cmd.length = sizeof(Fw_metadata_t);; //We send an updated time.
        cmd.data = (uint8_t *)&fw_info;
        sendCommand(&cmd,CDH_CSP_ADDRESS);
        csp_sleep_ms(1000);
        
        //Now we upload the file
        for(int i=0; i< numChunks; i++){

            int actual = fread(chunk,1,chunkSize,fwFile);

            telemetryPacket_t cmd;
            //Set command timestamp to now.
            Calendar_t now;
            getCalendarNow(&now);
            cmd.timestamp = now;
            cmd.telem_id = CDH_FW_PUT_DATA_CMD;
            cmd.length = sizeof(uint8_t)*chunkSize; //We send an updated time.
            cmd.data = chunk;
            sendCommand(&cmd,CDH_CSP_ADDRESS);
            if(i%500 ==0){
                printf("Done chunk %d of %d\n",i,numChunks);
                printf("Estimated time remainging: %f minutes \n",((double)numChunks-i)*.1/60);
            }
            csp_sleep_ms(100);
        }


    }

    END: 
        printf("done\n");
}


void cdhGetFwState(int argc, char **argv){

    telemetryPacket_t cmd;
    //Set command timestamp to now.
    Calendar_t now;
    getCalendarNow(&now);
    cmd.timestamp = now;
    cmd.telem_id = CDH_FW_GET_STATE_CMD;
    cmd.length = 0; //We send an updated time.
    sendCommand(&cmd,CDH_CSP_ADDRESS);

}

void cdhListFw(int argc, char **argv){
    
    telemetryPacket_t cmd;
    //Set command timestamp to now.
    Calendar_t now;
    getCalendarNow(&now);
    cmd.timestamp = now;
    cmd.telem_id = CDH_LIST_FW_CMD;
    cmd.length = 0; //We send an updated time.
    sendCommand(&cmd,CDH_CSP_ADDRESS);
}

void cdhSetFwState(int argc, char **argv){

    if(strcmp(argv[1],"help") ==0){
        printf("This command can be used to set state of the fw update manager on CDH. \n");
        printf("Valid Arguments: IDLE,RX_FW,PRE_VER,ARM,EXECUTE,EXECUTE_CONFIRM,POST_VER \n");
        printf("It is recommended to check success using the cdhFwGetState after running this command. \n");
        return;
    }

    if(argc != 2){
        printf("Wrong number of arguments.\n");
        return;
    }

    uint8_t state_cmd;

    if(!strcmp(argv[1],"IDLE")){
        state_cmd = CDH_FW_IDLE_CMD;
    }
    else if(!strcmp(argv[1],"RX_FW")){
        state_cmd = CDH_FW_RX_FW_CMD;
    }    
    else if(!strcmp(argv[1],"PRE_VER")){
        state_cmd = CDH_FW_PRE_VER_CMD;
    }
    else if(!strcmp(argv[1],"ARM")){
        state_cmd = CDH_FW_ARM_CMD;
    }
    else if(!strcmp(argv[1],"EXECUTE")){
        state_cmd = CDH_FW_EXECUTE_CMD;
    }
    else if(!strcmp(argv[1],"EXECUTE_CONFIRM")){
        state_cmd = CDH_FW_EXECUTE_CONFIRM_CMD;
    }
    else if(!strcmp(argv[1],"POST_VER")){
        state_cmd = CDH_FW_POST_VER_CMD;
    }
    else{
        printf("Invalid state: %s \n",argv[1]);
        return;
    }

    telemetryPacket_t cmd;
    //Set command timestamp to now.
    Calendar_t now;
    getCalendarNow(&now);
    cmd.timestamp = now;
    cmd.telem_id = state_cmd;
    cmd.length = 0; //We send an updated time.
    sendCommand(&cmd,CDH_CSP_ADDRESS);
}

void cdhChecksumFile(int argc, char **argv){

    if(argc != 2){
        printf("Wrong number arguments...\n");
        return;
    }

    char name[64] = {0};
    strcpy(name,argv[1]);

    telemetryPacket_t cmd;
    //Set command timestamp to now.
    Calendar_t now;
    getCalendarNow(&now);
    cmd.timestamp = now;
    cmd.telem_id = CDH_CHECKSUM_FILE_CMD;
    cmd.length = strlen(name)+1; //We send an updated time.
    cmd.data = name;
    sendCommand(&cmd,CDH_CSP_ADDRESS);


}


//Out must be pointer to 32 bit int where result will go. In must be opened file, seek will be set back to start on function return.
void checksum_file(uint32_t * out, FILE * in){
    //Based on the libcrc example/test program.
    uint32_t crc_32_val = 0xffffffffL;;
    char ch;
    unsigned char prev_byte;
    fseek(in, 0, SEEK_SET);
        if ( in != NULL ) {
            int count =0;
            while( ( fread(&ch,1,1, in ) ) >0 ) {

                
                crc_32_val = update_crc_32(     crc_32_val,         (unsigned char) ch            );
                count ++;
                prev_byte = (unsigned char) ch;
            }

            fseek(in, 0, SEEK_SET);
        }
    

    crc_32_val        ^= 0xffffffffL;

    *out = crc_32_val;

}

void checksumFile(int argc, char ** argv){

    char* fileName = argv[1];
    FILE * File;
    File = fopen(fileName,"rb");
    if(File == NULL){
        printf("Error opneing file %s\n",argv);
        return;
    }
    //Get checksum for file...
    uint32_t checksum;
    char cmd_str[128];
    checksum_file(&checksum,File);
    printf("checksum: %0x\n",checksum);

}

void cdhMvFile(int argc, char **argv){

    if(argc != 3){
        printf("Wrong number arguments...\n");
        return;
    }

    char oldPath[64] = {0};
    char newPath[64] = {0};
    strcpy(oldPath,argv[1]);
    strcpy(newPath,argv[2]);

    uint8_t databuf[130] = {0}; //Max 2 64b paths plus 2 bytes giving the length of each path.
    databuf[0] = strlen(oldPath)+1;
    databuf[1] = strlen(newPath)+1;
    strncpy(&databuf[2],oldPath,64);
    strncpy(&databuf[2+databuf[0]],newPath,64);


    telemetryPacket_t cmd;
    //Set command timestamp to now.
    Calendar_t now;
    getCalendarNow(&now);
    cmd.timestamp = now;
    cmd.telem_id = CDH_MV_FILE_CMD;
    cmd.length = 2 + strlen(oldPath)+1 + strlen(newPath)+1; //We send an updated time.
    cmd.data = databuf;
    sendCommand(&cmd,CDH_CSP_ADDRESS);

}
void cdhRmFile(int argc, char **argv){

    if(argc != 2){
        printf("Wrong number arguments...\n");
        return;
    }

    char name[64] = {0};
    strcpy(name,argv[1]);

    telemetryPacket_t cmd;
    //Set command timestamp to now.
    Calendar_t now;
    getCalendarNow(&now);
    cmd.timestamp = now;
    cmd.telem_id = CDH_RM_FILE_CMD;
    cmd.length = strlen(name)+1; //We send an updated time.
    cmd.data = name;
    sendCommand(&cmd,CDH_CSP_ADDRESS);
}
void cdhCpFile(int argc, char **argv){

    if(argc != 3){
        printf("Wrong number arguments...\n");
        return;
    }

    char oldPath[64] = {0};
    char newPath[64] = {0};
    strcpy(oldPath,argv[1]);
    strcpy(newPath,argv[2]);

    uint8_t databuf[130] = {0}; //Max 2 64b paths plus 2 bytes giving the length of each path.
    databuf[0] = strlen(oldPath)+1;
    databuf[1] = strlen(newPath)+1;
    strncpy(&databuf[2],oldPath,64);
    strncpy(&databuf[2+databuf[0]],newPath,64);


    telemetryPacket_t cmd;
    //Set command timestamp to now.
    Calendar_t now;
    getCalendarNow(&now);
    cmd.timestamp = now;
    cmd.telem_id = CDH_CP_FILE_CMD;
    cmd.length = 2 + strlen(oldPath)+1 + strlen(newPath)+1; //We send an updated time.
    cmd.data = databuf;
    sendCommand(&cmd,CDH_CSP_ADDRESS);
}