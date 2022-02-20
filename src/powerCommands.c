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
        cmd_data[1] = atoi(argv[1]); // Second arg is the task parameter.
        if(cmd.data[1] < 0 || cmd.data[1] > 6)
        {
            printf("Read POW temperature argument out of range.");
            return;
        }
        // Send the TTT
        sendCommand(&cmd,CDH_CSP_ADDRESS);

    } else {
        printf("Invalid command (improper number of arguments).\n");
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
        if(cmd.data[1] < 0 || cmd.data[1] > 6)
        {
            printf("Read POW solar current argument out of range.");
            return;
        }
        // Send the TTT
        sendCommand(&cmd,CDH_CSP_ADDRESS);

    } else {
        printf("Invalid command (improper number of arguments).\n");
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
        if(cmd.data[1] < 0 || cmd.data[1] > 6)
        {
            printf("Read POW load current argument out of range.");
            return;
        }
        // Send the TTT
        sendCommand(&cmd,CDH_CSP_ADDRESS);

    } else {
        printf("Invalid command (improper number of arguments).\n");
    }

}
void powReadMsbVoltage(int argc, char **argv){

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
        printf("Invalid command (improper number of arguments).\n");
    }

}


void powSetLoad(int argc, char **argv)
{
    if(argc == 3){
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
        if(strcmp(argv[2],"on"))
        {
            cmd_data[0] = TASK_POWER_SET_LOAD_ON; //First arg is the task code.
        }
        else if(strcmp(argv[2],"off"))
        {
            cmd_data[0] = TASK_POWER_SET_LOAD_OFF;
        }
        else
        {
            printf("Improper command.\n");
            return;
        }
        // Set the load switch number, check bounds
        cmd_data[1] = atoi(argv[1]);
        if(cmd.data[1] < 0 || cmd.data[1] > 6)
        {
            printf("Set POW load switch argument out of range.");
            return;
        }
        // Send the TTT
        sendCommand(&cmd,CDH_CSP_ADDRESS);

    } else {
        printf("Invalid command (improper number of arguments).\n");
    }
}
void powSetSolar(int argc, char **argv)
{
    if(argc == 3){
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
        if(strcmp(argv[2],"on"))
        {
            cmd_data[0] = TASK_POWER_SET_SOLAR_ON; //First arg is the task code.
        }
        else if(strcmp(argv[2],"off"))
        {
            cmd_data[0] = TASK_POWER_SET_SOLAR_OFF;
        }
        else
        {
            printf("Improper command.\n");
            return;
        }
        // Set the load switch number, check bounds
        cmd_data[1] = atoi(argv[1]);
        if(cmd.data[1] < 0 || cmd.data[1] > 6)
        {
            printf("Set POW solar switch argument out of range.");
            return;
        }
        // Send the TTT
        sendCommand(&cmd,CDH_CSP_ADDRESS);

    } else {
        printf("Invalid command (improper number of arguments).\n");
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
        // *** Need to fix input argument format ***
        cmd_data[1] = 0; // Task doesn't require a parameter
        // Send the TTT
        sendCommand(&cmd,CDH_CSP_ADDRESS);

    } else {
        printf("Invalid command (improper number of arguments).\n");
    }

}