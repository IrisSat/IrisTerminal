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


void powReadTemp(int argc, char **argv){

    if(argc == 2){
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
        cmd_data[0] = TASK_POWER_READ_TEMP; //First arg is the task code.
        cmd_data[1] = atoi(argv[2]); // Second arg is the task parameter.
        // Send the TTT
        sendCommand(&cmd,CDH_CSP_ADDRESS);

    } else {
        printf("Invalid command.\n");
    }

}

void powReadSolarCurrent(int argc, char **argv){

    if(argc == 2){
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
        cmd_data[0] = TASK_POWER_READ_SOLAR_CURRENT; //First arg is the task code.
        cmd_data[1] = atoi(argv[2]); // Second arg is the task parameter.
        // Send the TTT
        sendCommand(&cmd,CDH_CSP_ADDRESS);

    } else {
        printf("Invalid command.\n");
    }

}
void powReadLoadCurrent(int argc, char **argv){

    if(argc == 2){
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
        cmd_data[0] = TASK_POWER_READ_LOAD_CURRENT; //First arg is the task code.
        cmd_data[1] = atoi(argv[2]); // Second arg is the task parameter.
        // Send the TTT
        sendCommand(&cmd,CDH_CSP_ADDRESS);

    } else {
        printf("Invalid command.\n");
    }

}
void powReadMSBVoltage(int argc, char **argv){

    if(argc == 2){
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
        cmd_data[0] = TASK_POWER_READ_MSB_VOLTAGE; //First arg is the task code.
        cmd_data[1] = 0; // Task doesn't require a parameter
        // Send the TTT
        sendCommand(&cmd,CDH_CSP_ADDRESS);

    } else {
        printf("Invalid command.\n");
    }

}
void powSetMode(int argc, char **argv){

    if(argc == 2){
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
        cmd_data[0] = TASK_POWER_SET_MODE; //First arg is the task code.
        cmd_data[1] = 0; // Task doesn't require a parameter
        // Send the TTT
        sendCommand(&cmd,CDH_CSP_ADDRESS);

    } else {
        printf("Invalid command.\n");
    }

}