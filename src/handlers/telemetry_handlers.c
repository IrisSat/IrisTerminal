
#include "telemetry_handlers.h"

int imageDownloadState=0;
char imageDownloadFile[100] = "";
FILE * imageFileHandle;
int imageChunksLeft =0;
int imageChunksRcvd = 0;
int prevDownloadPercentage = 100;
int MAX_IMAGE_CHUNKS = 2700;
int IMAGE_PERCENT_DELTA = 2;

void CdhTelemetryHandler(csp_conn_t * conn, csp_packet_t * packet)
{
    telemetryPacket_t tm_pkt;
    unpackTelemetry(packet->data,&tm_pkt);
    switch(tm_pkt.telem_id){
        case CDH_MSG_ID:{
            uint8_t msg[tm_pkt.length];
            snprintf(msg,tm_pkt.length,"%s",tm_pkt.data);
            printf("CDH_MSG_ID: %s\n",msg);
            break;
        }
        default:{
            csp_service_handler(conn, packet);
            break;
        }
    }
}

void PayloadTelemetryHandler(csp_conn_t * conn, csp_packet_t * packet)
{
    // printf("Received telemetry packet from Payload.\n");
    telemetryPacket_t tm_pkt;
    unpackTelemetry(packet->data,&tm_pkt);
    
    switch(tm_pkt.telem_id){

        case PAYLOAD_ACK:{
            if(imageDownloadState == 1){

                uint32_t size = *((uint32_t*)&tm_pkt.data[0]);
                uint32_t numChunks = *((uint32_t*)&tm_pkt.data[sizeof(uint32_t)]);
                imageChunksLeft = numChunks;
                printf("Getting ready to receive image of %d bytes, in %d chunks.\n",size,numChunks);
                imageChunksRcvd = numChunks;

                if(imageFileHandle != NULL) fclose(imageFileHandle);

                imageFileHandle = fopen(imageDownloadFile,"wb");
                if(imageFileHandle == NULL){
                    printf("Could not open file %s\n",imageDownloadFile);
                }
            }
            break;
        }

        case PAYLOAD_FULL_IMAGE_ID:{

            uint8_t buff[256];
            imageChunksLeft --;
            memcpy(buff,&tm_pkt.data[4],tm_pkt.length-4); //Copy actual data
            fwrite(buff,1,tm_pkt.length-4,imageFileHandle);

            printf("chunks left:%d \n",imageChunksLeft);
            // float downloadPercentage = ((float)imageChunksLeft/(float)imageChunksRcvd*100.00);
            // printf("Remaining: %d\nRcvd: %d\n, Percentage: %.2f\nInt Percentage: %d\n,Int Percentage mod 10: %d\n",imageChunksLeft,imageChunksRcvd,downloadPercentage,(int)downloadPercentage,(int)downloadPercentage%10);
            // if((int)downloadPercentage % 10 == 0 && prevDownloadPercentage != (int)downloadPercentage){
            //     printf("Download Percentage: %d\n",100-(int)downloadPercentage);
            // }
            // prevDownloadPercentage = (int)downloadPercentage;

            if(imageChunksLeft == 0 ){
                fclose(imageFileHandle);
                imageChunksLeft = 0;
                imageDownloadState =0;
                printf("Done receiving image.\n");
            }
            break;
        }

        case PAYLOAD_FILE_LIST_ID:{
            uint8_t buf[tm_pkt.length];
            snprintf(buf,tm_pkt.length,"%s",tm_pkt.data);
            printf("PAYLOAD_FILE_LIST_ID: %s\n",buf);
            break;
        }

        case PAYLOAD_ERROR_ID:{
            uint8_t errorMsg[tm_pkt.length];
            snprintf(errorMsg,tm_pkt.length,"%s",tm_pkt.data);
            printf("PAYLOAD_ERROR_ID: %s\n",errorMsg);
            break;
        }

        case PAYLOAD_META_ID:{
            uint8_t errorMsg[tm_pkt.length];
            snprintf(errorMsg,tm_pkt.length,"%s",tm_pkt.data);
            printf("PAYLOAD_META_ID: %s\n",errorMsg);
            break;
        }

        case PAYLOAD_MSG_ID:{
            uint8_t msg[tm_pkt.length];
            snprintf(msg,tm_pkt.length,"%s",tm_pkt.data);
            printf("PAYLOAD_MSG_ID: %s\n",msg);
            break;
        }

        case POWER_READ_TEMP_ID:{
            float temp;
            memcpy(&temp,tm_pkt.data,4);
            printf("POWER_READ_TEMP_ID: %.3f\n",temp);
            // for(int i=0; i < 4; i++){
            //     printf("[%d] - 0x%2X\n",i,telem.data[i]);
            // }
            break;
        }
        case POWER_READ_SOLAR_CURRENT_ID:{
            float temp;
            memcpy(&temp,tm_pkt.data,4);
            printf("POWER_READ_SOLAR_CURRENT_ID: %.3f\n",temp);
            break;
        }
        case POWER_READ_LOAD_CURRENT_ID:{
            float temp;
            memcpy(&temp,tm_pkt.data,4);
            printf("POWER_READ_LOAD_CURRENT_ID: %.3f\n",temp);
            break;
        }
        case POWER_READ_MSB_VOLTAGE_ID:{
            float temp;
            memcpy(&temp,tm_pkt.data,4);
            printf("POWER_READ_MSB_VOLTAGE_ID: %.3f\n",temp);
            break;
        }
        case POWER_GET_BATTERY_SOC_ID:{
            float temp;
            memcpy(&temp,tm_pkt.data,4);
            printf("POWER_GET_BATTERY_SOC_ID: %.3f\n",temp);
            break;
        }
        case POWER_GET_ECLIPSE_ID:{
            printf("POWER_GET_ECLIPSE_ID: %d\n",tm_pkt.data[0]);
            break;
        }
        case POWER_GET_BOOT_COUNT_ID:{
            printf("POWER_GET_BOOT_COUNT_ID: %d\n",tm_pkt.data[0]);
            break;
        }
        default:{
            csp_service_handler(conn, packet);
            break;
        }
    }
}
