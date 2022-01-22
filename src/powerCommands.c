#include "powerCommands.h"
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

void powReadADC(int argc, char **argv){

    if(argc == 3){

        Calendar_t now;
        time_t t = time(NULL);
        struct tm *tm = localtime(&t);
        timeToCalendar(tm,&now);
            
        telemetryPacket_t cmd;

        if(argv[2]=='A'){
            cmd.telem_id = POW_READ_ADC_A_CMD;
        } else if (argv[2]=='B'){
            cmd.telem_id = POW_READ_ADC_B_CMD;
        } else {
            printf("No ADC bank given.");
            return;
        }

        /*
        // At TTT
        //We need to send the task code, and when to execute.
        cmd.length = sizeof(uint8_t)*2+ sizeof(Calendar_t);
        uint8_t cmd_data[sizeof(uint8_t)*2+ sizeof(Calendar_t)] = {0};
        // unsigned int dl = sizeof(uint8_t)*2+ sizeof(Calendar_t);
        // uint8_t cmd_data[dl] = {0};
        cmd.data = cmd_data;
        cmd_data[0] = 0;//First arg is the task code.
        cmd_data[9] = atoi(argv[2]);
        memcpy(&cmd_data[1],&now,sizeof(Calendar_t));
        sendCommand(&cmd,CDH_CSP_ADDRESS);
        */
        cmd.length = 1;
        cmd.data[0] = atoi(argv[3]);
        sendCommand(&cmd,POWER_CSP_ADDRESS);
    }
    

}