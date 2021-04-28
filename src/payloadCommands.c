#include "payloadCommands.h"
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <csp/csp.h>
#include <csp/csp_types.h>
#include <stdlib.h>
#include <string.h>
#include "csp_client.h"
#include <csp/arch/csp_thread.h>

void sendImage(int argc, char **argv){

    
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

void getPayloadTelemetry(int argc,char **argv){

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

}

void checkPayloadTelemetry(int argc,char **argv){}
void requestPayloadTelemetry(int argc,char **argv){}