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

const char * LoadCurrentChannelStrings[] =
{
    "minco",
    "ant-dpl",
    "coms",
    "adcs",
    "datec",
    "pld",
    "cdh"
};

const char * LoadSwitchStrings[] = 
{
    "datec",
    "adcs",
    "coms",
    "cdh",
    "pld",
    "dpl-a",
    "dpl-s"
};

const char * PowerModeStrings[] = 
{
    "detumble",
    "critical",
    "survival",
    "low-power",
    "idle",
    "normal",
    "sun-pointing",
    "science"
};

void powReadTempChannel(int argc, char **argv){

    if(argc == 2){
        if(strcmp(argv[1],"--help") == 0){
            printf("powReadTemp help:\n");
            printf("-Command format: powReadTemp [arg]\n");
            printf("-Possible arguments: 0-6\n");
            return;
        }
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
            printf("Read POW temperature argument out of range.\n");
            printf("Enter the following command for valid modes: powReadTemp --help\n");
            return;
        }
        // Send the TTT
        printf("Reading temperature channel %d\n",cmd_data[1]);
        //sendCommand(&cmd,CDH_CSP_ADDRESS);

    } else {
        printf("Invalid command (improper number of arguments - 1 required).\n");
        printf("Enter the following command for valid modes: powReadTemp --help\n");
    }

}

void powReadSolarCurrent(int argc, char **argv){

    if(argc == 2){
        if(strcmp(argv[1],"--help") == 0){
            printf("powReadSC help:\n");
            printf("-Command format: powReadSC [arg]\n");
            printf("-Possible arguments: 0-6\n");
            return;
        }
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
        cmd_data[1] = atoi(argv[1]); // Second arg is the task parameter.
        if(cmd.data[1] < 0 || cmd.data[1] > 6)
        {
            printf("Read POW solar current argument out of range.\n");
            printf("Enter the following command for valid modes: powReadSC --help\n");
            return;
        }
        // Send the TTT
        printf("Reading solar current %d\n",cmd_data[1]);
        //sendCommand(&cmd,CDH_CSP_ADDRESS);

    } else {
        printf("Invalid command (improper number of arguments - 1 required).\n");
        printf("Enter the following command for valid modes: powReadSC --help\n");
    }

}
void powReadLoadCurrent(int argc, char **argv){

    if(argc == 2){
        if(strcmp(argv[1],"--help") == 0){
            printf("powReadLC help:\n");
            printf("-Command format: powReadLC [arg]\n");
            printf("-Possible arguments:\n");
            int i;
            for(i=0; i < NUM_LOAD_CURRENT_CHANNELS; i++)
                printf(" - %s\n",LoadCurrentChannelStrings[i]);
            return;
        }
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
        uint8_t loadCurentChannel = decodeLoadCurrentChannel(argv[1]);
        cmd_data[1] = loadCurentChannel; // Second arg is the task parameter.
        if(cmd.data[1] < 0 || cmd.data[1] > 6)
        {
            printf("Invalid load current channel.\n");
            printf("Enter the following command for valid modes: powReadLC --help\n");
            return;
        }
        // Send the TTT
        printf("Reading load current %s\n",LoadCurrentChannelStrings[loadCurentChannel]);
        //sendCommand(&cmd,CDH_CSP_ADDRESS);

    } else {
        printf("Invalid command (improper number of arguments - 1 required).\n");
        printf("Enter the following command for valid modes: powReadLC --help\n");
    }

}
void powReadMsbVoltage(int argc, char **argv){
    if(argc == 2){
        if(strcmp(argv[1],"--help") == 0){
            printf("powReadMsbVoltage help:\n");
            printf("-Command format: powSetLoadSwitch\n");
            printf("-No arguments required:\n");
        }
    }
    else if(argc == 1){
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
        printf("Getting MSB voltage\n");
        //sendCommand(&cmd,CDH_CSP_ADDRESS);

    } else {
        printf("Invalid command (improper number of arguments - none required).\n");
        printf("Enter the following command for valid modes: powReadMsbVoltage --help\n");
    }

}


void powSetLoadSwitch(int argc, char **argv)
{
    if(argc == 2){
        if(strcmp(argv[1],"--help") == 0){
            printf("powSetLoadSwitch help:\n");
            printf("-Command format: powSetLoadSwitch [arg] [on|off]\n");
            printf("-Possible arguments:\n");
            int i;
            for(i=0; i < NUM_LOAD_SWITCHES; i++)
                printf(" - %s\n",LoadSwitchStrings[i]);
        }
    }
    else if(argc == 3){
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
        if(strcmp(argv[2],"on") == 0)
        {
            cmd_data[0] = TASK_POWER_SET_LOAD_ON; //First arg is the task code.
        }
        else if(strcmp(argv[2],"off") == 0)
        {
            cmd_data[0] = TASK_POWER_SET_LOAD_OFF;
        }
        else
        {
            printf("Improper command argument (on/off).\n");
            printf("Enter the following command for valid modes: powSetLoadSwitch --help\n");
            return;
        }
        // Set the load switch number, check bounds
        uint8_t loadSwitchNum = decodeLoadSwitchNumber(argv[1]);
        cmd_data[1] = loadSwitchNum;
        if(cmd.data[1] < 0 || cmd.data[1] > 6)
        {
            printf("Invalid power load switch argument - 2 required.\n");
            printf("Enter the following command for valid modes: powSetLoadSwitch --help\n");
            return;
        }
        // Send the TTT
        printf("Setting power load switch %s %s\n",LoadSwitchStrings[loadSwitchNum],argv[2]);
        //sendCommand(&cmd,CDH_CSP_ADDRESS);

    } else {
        printf("Invalid command (improper number of arguments).\n");
        printf("Enter the following command for valid modes: powSetLoadSwitch --help\n");
    }
}
void powSetSolar(int argc, char **argv)
{
    if(argc == 2){
        if(strcmp(argv[1],"--help") == 0){
            printf("powSetSolar help:\n");
            printf("-Command format: powSetSolar [arg] [on|off]\n");
            printf("-Possible arguments: 0-6\n");
        }
    }
    else if(argc == 3){
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
        if(strcmp(argv[2],"on") == 0)
        {
            cmd_data[0] = TASK_POWER_SET_SOLAR_ON; //First arg is the task code.
        }
        else if(strcmp(argv[2],"off") == 0)
        {
            cmd_data[0] = TASK_POWER_SET_SOLAR_OFF;
        }
        else
        {
            printf("Improper command argument (on/off).\n");
            printf("Enter the following command for valid modes: powSetSolar --help\n");
            return;
        }
        // Set the load switch number, check bounds
        cmd_data[1] = atoi(argv[1]);
        if(cmd.data[1] < 0 || cmd.data[1] > 6)
        {
            printf("Set POW solar switch argument out of range.\n");
            printf("Enter the following command for valid modes: powSetSolar --help\n");
            return;
        }
        // Send the TTT
        printf("Setting power solar array %d %s\n",cmd_data[1],argv[2]);
        //sendCommand(&cmd,CDH_CSP_ADDRESS);

    } else {
        printf("Invalid command (improper number of arguments - 2 required).\n");
        printf("Enter the following command for valid modes: powSetSolar --help\n");
    }
}

void powSetMode(int argc, char **argv){

    if(argc == 2){
        if(strcmp(argv[1],"--help") == 0){
            printf("powSetMode help:\n");
            printf("-Command format: powSetMode [arg]\n");
            printf("-Possible arguments:\n");
            int i;
            for(i=0; i < NUM_POWER_MODES; i++)
                printf(" - %s\n",PowerModeStrings[i]);
            return;
        }
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
        uint8_t powerMode = decodePowerModes(argv[1]);
        if(powerMode < 0 || powerMode >= NUM_POWER_MODES){
            printf("Invalid power mode.\n");
            printf("Enter the following command for valid modes: powSetMode --help\n");
            return;
        }
        cmd_data[1] = powerMode;
        // Send the TTT
        printf("Setting power mode: %s\n",PowerModeStrings[powerMode]);
        //sendCommand(&cmd,CDH_CSP_ADDRESS);

    } else {
        printf("Invalid command (improper number of arguments - 1 required).");
        printf("Enter the following command for valid modes: powSetMode --help\n");
    }

}


int decodeLoadCurrentChannel(char * arg)
{
    int i;
    for(i=0; i < NUM_LOAD_CURRENT_CHANNELS; i++)
    {
        if(strcmp(arg,LoadCurrentChannelStrings[i]) == 0)
            return i;
    }
    return -1;
}

int decodeLoadSwitchNumber(char * arg)
{
    int i;
    for(i=0; i < NUM_LOAD_SWITCHES; i++)
    {
        if(strcmp(arg,LoadSwitchStrings[i]) == 0)
            return i;
    }
    return -1;
}

int decodePowerModes(char * arg)
{
    int i;
    for(i=0; i < NUM_POWER_MODES; i++)
    {
        if(strcmp(arg,PowerModeStrings[i]) == 0)
            return i;
    }
    return -1;
}