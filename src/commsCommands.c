#include "commsCommands.h"
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

void commsSendCSPMessage(int argc, char **argv){

    if(argc == 1){
        // if(strcmp(argv[1],"--help") == 0){
        //     printf("powReadSC help:\n");
        //     printf("-Command format: powReadSC [arg]\n");
        //     printf("-Possible arguments: 0-6\n");
        //     return;
        // }
        // Create a telemtry packet
        telemetryPacket_t cmd;
        //Set command timestamp to now.
        Calendar_t now;
        time_t t = time(NULL);
        struct tm *tm = localtime(&t);
        timeToCalendar(tm,&now);
        cmd.timestamp = now;
        // TTT ID 
        cmd.telem_id = CDH_SCHEDULE_TTT_CMD;
        cmd.length = 2*sizeof(uint8_t)+ sizeof(Calendar_t); //We need to send the task code, and when to execute.
        // Format Data
        uint8_t cmd_data[2*sizeof(uint8_t)+ sizeof(Calendar_t)] = {0};
        cmd.data = cmd_data;
        cmd_data[0] = TASK_COMMS_SEND_MESSAGE; //First arg is the task code.
        // cmd_data[1] = atoi(argv[1]); // Second arg is the task parameter.
        // if(cmd.data[1] < 0 || cmd.data[1] > 6)
        // {
        //     printf("Invalid read power solar current arguments.\n");
        //     printf("Enter the following command for valid modes: powReadSC --help\n");
        //     return;
        // }
        // Send the TTT
        printf("sending COMMS message\n");
        sendCommand(&cmd,CDH_CSP_ADDRESS);

    } else {
        printf("Invalid command (improper number of arguments - 1 required).\n");
        printf("Enter the following command for valid modes: powReadSC --help\n");
    }

}
