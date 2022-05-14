#include "handlers/cdhHandler.h"

#include <stdio.h>

char * cdhFwStateTable[6] ={
    "IDLE",
    "RX_FW",
    "PRE_VERIFY",
    "ARMED",
    "UPDATE",
    "POST_VERIFY",
};

void cdhTelemetryHandler(telemetryPacket_t* telem){

    switch(telem->telem_id){

        case CDH_MSG_ID:{
                uint8_t msg[telem->length];
                snprintf(msg,telem->length,"%s",telem->data);
                printf("CDH_MSG_ID: %s\n",msg);
                break;
            }
        case CDH_FW_STATE_ID:{

            uint8_t state = telem->data[0];
            char * stateName = cdhFwStateTable[state];
            printf("CDH_FW_STATE: %d (%s)\n",state,stateName);
            break;
        }
    }

}