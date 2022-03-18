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
        telemetryPacket_t cmd;
        cmd.length = 2;
        uint8_t cmd_data[2] = {0};
        cmd.data = cmd_data;
        cmd_data[0] = 0xAA;
        cmd_data[1] = 0xAA;
        printf("sending COMMS message\n");
        sendCommand(&cmd,COMMS_CSP_ADDRESS);

    } else {
        printf("Invalid command (improper number of arguments - 1 required).\n");
    }

}
