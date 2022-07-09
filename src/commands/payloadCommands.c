#include "payloadCommands.h"
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
#include "networkConfig.h"
#include "cameraRegLists.h"
const char * TemperatureChannels[] =
{
    "board",
    "sp",
};
/*

void functionTemplate(int argc, char **argv){
    if(argc == ){
        uint8_t param = atoi(argv[]);
        if(param < 1 || param > 2){
            printf("Improper param value.\n");
            return;
        }
        telemetryPacket_t cmd;
        cmd.telem_id  = ;
        cmd.length = ;
        cmd.data[0] = param;
        printf("Sending command with param %d\n",param);
        sendCommand(&cmd,_CSP_ADDRESS);
    } else {
        printf("Improper number of arguments.\n");
    }
}
*/
void pldReadTemp(int argc, char **argv){

    if(argc == 2){
        if(strcmp(argv[1],"--help") == 0){
            printf("pldReadTemp help:\n");
            printf("-Command format: pldReadTemp [channel]\n");
            printf("-Possible arguments: 0-6\n");
            return;
        }
        // telemetryPacket_t cmd = {0};
        // sendCommand(&cmd,PAYLOAD_CSP_ADDRESS);
        
        // if(strcmp(argv[2],"board") == 0)
        // {
        //     return;
        // }
        // // Send the TTT
        // printf("Reading temperature channel %d\n",cmd_data[1]);
        // sendCommand(&cmd,CDH_CSP_ADDRESS);

    } else {
        printf("Invalid command (improper number of arguments - 1 required).\n");
        printf("Enter the following command for valid modes: powReadTemp --help\n");
    }

}

void pldTestCameraInit(int argc, char **argv){
    telemetryPacket_t cmd = {0};
    cmd.telem_id  = PAYLOAD_CAMERA_TEST_INIT;
    sendCommand(&cmd,PAYLOAD_CSP_ADDRESS);
}
void pldCameraRegListWrite(int argc, char **argv){
    if(argc == 2){
        uint8_t regListIdx = atoi(argv[1]);
        if(regListIdx < 0 || regListIdx >= NUM_REG_LISTS){
            printf("Improper register list number value.\n");
            return;
        }
        telemetryPacket_t cmd;
        s_RegList * regList;
        cmd.telem_id  = PAYLOAD_CAMERA_WRITE_REG_LIST;
        cmd.length = getRegisterListSize(regListIdx);
        // cmd.data = getRegisterList(regListIdx);
        getRegisterList(regListIdx,cmd.data);
        printf("Register list size: %d\n",cmd.length);
        // sendCommand(&cmd,PAYLOAD_CSP_ADDRESS);
    } else {
        printf("Improper number of arguments.\n");
    }
}

void pldCameraSetI2cWriteAddress(int argc, char **argv){
    if(argc == 2){
        int paramLength = strlen(argv[1]);
        if(paramLength < 0 || paramLength > 2){
            printf("Arg length error: %s\n",argv[1]);
            return;
        }
        uint8_t param = atoi(argv[1]);
        telemetryPacket_t cmd;
        cmd.telem_id  = PAYLOAD_CAMERA_SET_I2C_WRITE_ADDRESS;
        cmd.length = 1;
        cmd.data[0] = param;
        printf("Setting Payload camera I2c write address to: 0x%2X\n",param);
        sendCommand(&cmd,PAYLOAD_CSP_ADDRESS);
    } else {
        printf("Improper number of arguments.\n");
    }
}
void pldCameraSetI2cReadAddress(int argc, char **argv){
    if(argc == 2){
        int paramLength = strlen(argv[1]);
        if(paramLength < 0 || paramLength > 2){
            printf("Arg length error: %s\n",argv[1]);
            return;
        }
        uint8_t param = atoi(argv[1]);
        telemetryPacket_t cmd;
        cmd.telem_id  = PAYLOAD_CAMERA_SET_I2C_READ_ADDRESS;
        cmd.length = 1;
        cmd.data[0] = param;
        printf("Setting Payload camera I2c read address to: 0x%2X\n",param);
        sendCommand(&cmd,PAYLOAD_CSP_ADDRESS);
    } else {
        printf("Improper number of arguments.\n");
    }
}
void pldCameraI2cTransmit(int argc, char **argv){
    printf("I2C transmit: argc = %d\n",argc);
    if(argc > 2){
        uint8_t cmd_data[argc-1];
        cmd_data[0] = atoi(argv[1]);
        int i;
        for(i=2; i < argc; i++){
            printf("%d of %d\n",i,sizeof(cmd_data)/sizeof(uint8_t));
            // int paramLength = strlen(argv[i]);
            // if(paramLength < 0 || paramLength > 2){
            //     printf("Arg length error: argv[%d] = %s\n (length = %d)",i,argv[i],paramLength);
            //     return;
            // }
            // printf("Arg[%d]: 0x%s\n",i,argv[i]);
            // int value = (int) strtol(argv[i], NULL, 8);
            // cmd_data[i-1] = (uint8_t) value;
            // cmd_data[i-1] = (uint8_t) strtol(argv[i], NULL, 8);
            // printf("Value = %d\n",cmd_data[i-1]);
            cmd_data[i-1] = (uint8_t) atoi(argv[i]);
        }
        printf("Parameters set.\n");
        telemetryPacket_t cmd;
        cmd.telem_id  = PAYLOAD_CAMERA_I2C_TRANSMIT;
        cmd.length = argc-1;
        cmd.data = cmd_data;
        printf("Payload camera I2C transmit...\n");
        sendCommand(&cmd,PAYLOAD_CSP_ADDRESS);
    } else {
        printf("Improper number of arguments.\n");
    }
}
void pldCameraHandshake(int argc, char **argv){
    if(argc == 2){
        uint8_t camNum = atoi(argv[1]);
        if(camNum < 1 || camNum > 2){
            printf("Improper camera number.\n");
            return;
        }
        telemetryPacket_t cmd;
        cmd.telem_id  = PAYLOAD_CAMERA_HANDSHAKE;
        cmd.length = 1;
        cmd.data[0] = camNum;
        printf("Initializing camera %d\n",camNum);
        sendCommand(&cmd,PAYLOAD_CSP_ADDRESS);
    } else {
        printf("Improper number of arguments.\n");
    }
}
void pldCameraSensorInit(int argc, char **argv){
    if(argc == 2){
        uint8_t camNum = atoi(argv[1]);
        if(camNum < 1 || camNum > 2){
            printf("Improper camera number.\n");
            return;
        }
        telemetryPacket_t cmd;
        cmd.telem_id  = PAYLOAD_CAMERA_SENSOR_INIT;
        cmd.length = 1;
        cmd.data[0] = camNum;
        printf("Initializing camera %d\n",camNum);
        sendCommand(&cmd,PAYLOAD_CSP_ADDRESS);
    } else {
        printf("Improper number of arguments.\n");
    }
}
void pldBootCount(int argc, char **argv){

}
void pldSendImage(int argc, char **argv){
    if(argc != 2){
        printf("incorrect parameteres...");
    }
    else{
    printf("Sending image to the payload board...\n");
    const int chunkSize = 64;
    char* fileName =argv[1];
    FILE * imgFile;
    imgFile = fopen(fileName,"rb");
    if(imgFile == NULL){
        printf("Error opneing file %s\n",argv);
        goto END;
    }

    fseek(imgFile, 0, SEEK_END); // seek to end of file
    long fileSize_B= ftell(imgFile); // get current file pointer
    fseek(imgFile, 0, SEEK_SET); // seek back to beginning of file

    double numChunks_dec = fileSize_B/chunkSize;
    int numChunks = (int)ceil(numChunks_dec);

    printf("Splitting the image(%dkiB) into %d chunks of size %d B.\n",fileSize_B/1024,numChunks,chunkSize);
    // proceed with allocating memory and reading the file

    uint8_t* chunk;
    chunk = malloc(chunkSize);
    if(chunk == NULL){
        printf("Error allocating memory... Download some more RAM!\n");
    }
    //Send file...
    for(uint16_t i=0; i<numChunks;i++){

        fread(chunk,1,chunkSize,imgFile);

        uint8_t telemPacket[256];
        memcpy(&telemPacket[12],chunk,chunkSize);
        telemPacket[8] = 10;
        telemPacket[9] = 2+chunkSize;
        uint16_t * n =(uint16_t*)(&telemPacket[10]);
        *n = numChunks-1-i;
        fread(chunk,1,chunkSize,imgFile);
        uint8_t resp[12];

        int result = csp_transaction(2,5,7,5000,telemPacket,12+chunkSize,resp,12);
        if(result<=0){
            printf("csp transaction error: %d\n",result);
            break;
        }
        

            // csp_conn_t * conn;
			// csp_packet_t * outPacket;
			// conn = csp_connect(2,5,7,1000,0);	//Create a connection. This tells CSP where to send the data (address and destination port).
            // if (conn == NULL){

            //     printf("Error with connection\n;");
            //     goto END;
            // }
		       
			// outPacket = csp_buffer_get(chunkSize +12);

            // memcpy(&outPacket->data[0],telemPacket,chunkSize +12);
            // outPacket->length = chunkSize +12;

            // int good = csp_send(conn,outPacket,0);
            // printf("csp_send: %d\n",good);
			// csp_close(conn);

			// if(!good){
                
			// 	csp_buffer_free(outPacket);
			// }
            // break;
			// memcpy(&outPacket->data[0],&packet->timestamp,sizeof(Calendar_t));
			// memcpy(&outPacket->data[sizeof(Calendar_t)],&packet->telem_id,1);
			// memcpy(&outPacket->data[sizeof(Calendar_t)+1],&packet->length,1);

			// //Only do this if there is data.
			// if(packet->length){
			// 	memcpy(&outPacket->data[sizeof(Calendar_t)+2],packet->data,packet->length);
			// }

			// outPacket->length = TELEM_HEADER_SIZE + packet->length;

			// int good = csp_send(conn,outPacket,0);
			// csp_close(conn);

			// if(!good){

			// 	csp_buffer_free(outPacket);
			// }


        if(*((uint16_t*)&resp[10]) != (numChunks-1-i) || resp[8] != 11){
            printf("Error on packet %d stopping...\n",i);
            goto END;
        }

        if(i %(numChunks/100) == 0){
            printf("Progress: %f \% \n",((double)i)/numChunks*100);
        }
        // csp_sleep_ms(10);
        printf("chunk num: %d\n",i);
    }

    fclose(imgFile);
    }
    END:
        printf("error\n");
}

void pldGetTelemetry(int argc,char **argv){

    if(strcmp(argv[1],"help") ==0){
        printf("This command can be used to get recent telemtry from payload that is stored on CDH.\n");
        printf("Usage: getPldTelem <item> <num>\n");
        printf("\t <item>: The telemetry data you want: \n");
        printf("\t\t pwrGood\n\t\t boardTemp\n\t\t sampleTemp\n\t\t sampleLoc\n\t\t error\n");
        printf("\t <num>: The max number of items to get. Starting with the newest value.");
    }
    
    else if(strcmp(argv[1],"pwrGood") ==0){

    }

}

void downloadImage(int argc, char **argv){

    imageDownloadState = 1;
    snprintf(imageDownloadFile,100,"%s",argv[1]);
    if(argc == 3){
        char * args[3] = {"scheduleTTT", "0","now"};
        // scheduleTTT(3,args);

        Calendar_t now;
        time_t t = time(NULL);
        struct tm *tm = localtime(&t);
        timeToCalendar(tm,&now);
            
        telemetryPacket_t cmd;

        cmd.telem_id = CDH_SCHEDULE_TTT_CMD;
        //We need to send the task code, and when to execute.
        cmd.length = sizeof(uint8_t)*2+ sizeof(Calendar_t);

        uint8_t cmd_data[sizeof(uint8_t)*2+ sizeof(Calendar_t)] = {0};
        cmd.data = cmd_data;

        cmd_data[0] = 0;//First arg is the task code.
        cmd_data[9] = atoi(argv[2]);
        memcpy(&cmd_data[1],&now,sizeof(Calendar_t));


        sendCommand(&cmd,CDH_CSP_ADDRESS);
    }
    else if(argc == 4){
        char * args[3] = {"scheduleTTT", "0","now"};
        Calendar_t now;
        time_t t = time(NULL);
        struct tm *tm = localtime(&t);
        timeToCalendar(tm,&now);
        telemetryPacket_t cmd;
        cmd.telem_id = CDH_SCHEDULE_TTT_CMD;
        //We need to send the task code, and when to execute.
        cmd.length = sizeof(uint8_t)*3+ sizeof(Calendar_t);

        uint8_t cmd_data[sizeof(uint8_t)*2+ sizeof(Calendar_t)] = {0};
        cmd.data = cmd_data;

        cmd_data[0] = 0;//First arg is the task code.
        cmd_data[1] = atoi(argv[2]);
        // cmd_data[9] = atoi(argv[2]);
        // cmd_data[10] = atoi(argv[3]);
        memcpy(&cmd_data[5],&now,sizeof(Calendar_t));


        sendCommand(&cmd,CDH_CSP_ADDRESS);
    }
    else if (argc == 8){
        Calendar_t when;
        when.day= atoi(argv[2]);
        when.hour = atoi(argv[3]);
        when.minute = atoi(argv[4]);
        when.month = atoi(argv[5]);
        when.year = atoi(argv[6]);
        when.weekday = 0; //This really isn't ever used and is just a pain, set to 0...
        when.weekday =1; //Not used but must be 1.
        scheduleTTT_raw(0,when);
    }
    else if(argc == 3){
        Calendar_t cal;
        int mins = atoi(argv[2]);
        getCalendarNow(&cal);

        if(mins +cal.minute > 59){
            cal.hour = (cal.hour+1)%24;
            cal.minute = mins-(60-cal.minute);
        }
        else{
            cal.minute = cal.minute+mins;
        }
    }

}

void pldCheckTelemetry(int argc,char **argv){}
void pldRequestTelemetry(int argc,char **argv){
    
    printf("Issuing the following commands:\n");
    printf("- PAYLOAD_POWER_GOOD_CMD\n");
    printf("- PAYLOAD_BOARD_TEMP_CMD\n");
    printf("- PAYLOAD_SAMPLE_TEMP_CMD\n");
    telemetryPacket_t cmd = {0};
    cmd.telem_id = PAYLOAD_POWER_GOOD_CMD;
    cmd.length = 0;
    sendCommand(&cmd,PAYLOAD_CSP_ADDRESS);
    cmd.telem_id = PAYLOAD_BOARD_TEMP_CMD;
    sendCommand(&cmd,PAYLOAD_CSP_ADDRESS);
    cmd.telem_id = PAYLOAD_SAMPLE_TEMP_CMD;
    sendCommand(&cmd,PAYLOAD_CSP_ADDRESS);
}

//Added by Ali{

void turnoncamera1(int argc, char **argv){ 
    
    telemetryPacket_t cmd;

    //Set command timestamp to now.
    Calendar_t now;
    getCalendarNow(&now);
    cmd.timestamp = now;

    cmd.telem_id = PAYLOAD_CAM1_ON_CMD;

    cmd.length = 0; //We send an updated time.
sendCommand(&cmd,PAYLOAD_CSP_ADDRESS);
}


void turnoffcamera1(int argc, char **argv){telemetryPacket_t cmd;

    //Set command timestamp to now.
    Calendar_t now;
    getCalendarNow(&now);
    cmd.timestamp = now;

    cmd.telem_id = PAYLOAD_CAM1_OFF_CMD;

    cmd.length = 0; //We send an updated time.
sendCommand(&cmd,PAYLOAD_CSP_ADDRESS);
}


void turnoncamera2(int argc, char **argv){telemetryPacket_t cmd;

    //Set command timestamp to now.
    Calendar_t now;
    getCalendarNow(&now);
    cmd.timestamp = now;

    cmd.telem_id = PAYLOAD_CAM2_ON_CMD;

    cmd.length = 0; //We send an updated time.
sendCommand(&cmd,PAYLOAD_CSP_ADDRESS);
}

void turnoffcamera2(int argc, char **argv){telemetryPacket_t cmd;

    //Set command timestamp to now.
    Calendar_t now;
    getCalendarNow(&now);
    cmd.timestamp = now;

    cmd.telem_id = PAYLOAD_CAM2_OFF_CMD;

    cmd.length = 0; //We send an updated time.
sendCommand(&cmd,PAYLOAD_CSP_ADDRESS);
}


void resetcamera1(int argc, char **argv){telemetryPacket_t cmd;

    //Set command timestamp to now.
    Calendar_t now;
    getCalendarNow(&now);
    cmd.timestamp = now;

    cmd.telem_id = PAYLOAD_CAM1_RESET_CMD;

    cmd.length = 0; //We send an updated time.
sendCommand(&cmd,PAYLOAD_CSP_ADDRESS);
}


void resetcamera2(int argc, char **argv){telemetryPacket_t cmd;

    //Set command timestamp to now.
    Calendar_t now;
    getCalendarNow(&now);
    cmd.timestamp = now;

    cmd.telem_id = PAYLOAD_CAM2_RESET_CMD;

    cmd.length = 0; //We send an updated time.
sendCommand(&cmd,PAYLOAD_CSP_ADDRESS);
}


void listofimages(int argc, char **argv){telemetryPacket_t cmd;

    //Set command timestamp to now.
    Calendar_t now;
    getCalendarNow(&now);
    cmd.timestamp = now;

    cmd.telem_id = PAYLOAD_FILE_LIST_CMD;

    cmd.length = 0; //We send an updated time.
sendCommand(&cmd,PAYLOAD_CSP_ADDRESS);
}


void takeimagecamera1(int argc, char **argv){telemetryPacket_t cmd;

    //Set command timestamp to now.
    Calendar_t now;
    getCalendarNow(&now);
    cmd.timestamp = now;

    cmd.telem_id = PAYLOAD_TAKE_IMAGE_CAM1_CMD;

    cmd.length = 0; //We send an updated time.
sendCommand(&cmd,PAYLOAD_CSP_ADDRESS);
}


void takeimagecamera2(int argc, char **argv){telemetryPacket_t cmd;

    //Set command timestamp to now.
    Calendar_t now;
    getCalendarNow(&now);
    cmd.timestamp = now;

    cmd.telem_id = PAYLOAD_TAKE_IMAGE_CAM2_CMD;

    cmd.length = 0; //We send an updated time.
sendCommand(&cmd,PAYLOAD_CSP_ADDRESS);
}

void pldFileList(int argc, char **argv){
    
    telemetryPacket_t cmd;
    //Set command timestamp to now.
    Calendar_t now;
    getCalendarNow(&now);
    cmd.timestamp = now;
    cmd.telem_id = PAYLOAD_FILE_LIST_CMD;
    cmd.length = 0; //We send an updated time.
    sendCommand(&cmd,PAYLOAD_CSP_ADDRESS);
}

void deleteImage(int argc, char **argv){
    
    telemetryPacket_t cmd;
    //Set command timestamp to now.
    Calendar_t now;
    getCalendarNow(&now);
    cmd.timestamp = now;
    cmd.telem_id = PAYLOAD_FILE_LIST_CMD;
    cmd.length = 1; //We send an updated time.
    cmd.data[0] = atoi(argv[1]);
    sendCommand(&cmd,PAYLOAD_CSP_ADDRESS);
}

void pldMountFS(int argc, char **argv){
    telemetryPacket_t cmd;
    Calendar_t now;
    getCalendarNow(&now);
    cmd.timestamp = now;
    cmd.telem_id = PAYLOAD_MOUNT_FS;
    cmd.length = 1;
    sendCommand(&cmd,PAYLOAD_CSP_ADDRESS);
}

void pldUnmountFS(int argc, char **argv){
    telemetryPacket_t cmd;
    Calendar_t now;
    getCalendarNow(&now);
    cmd.timestamp = now;
    cmd.telem_id = PAYLOAD_UNMOUNT_FS;
    cmd.length = 1;
    sendCommand(&cmd,PAYLOAD_CSP_ADDRESS);
}
void pldRestartFS(int argc, char **argv){
    telemetryPacket_t cmd;
    Calendar_t now;
    getCalendarNow(&now);
    cmd.timestamp = now;
    cmd.telem_id = PAYLOAD_RESTART_FS;
    cmd.length = 1;
    sendCommand(&cmd,PAYLOAD_CSP_ADDRESS);
}
