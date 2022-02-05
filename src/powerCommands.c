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

    if(argc == 2 || argc == 3){
        // Check the bounds of num
        int num = atoi(argv[2]);

        // Create timestamp for time tagged task (we'll just schedule them right away/now)
        Calendar_t now;
        time_t t = time(NULL);
        struct tm *tm = localtime(&t);
        timeToCalendar(tm,&now);
        // Create telemetry packet
        telemetryPacket_t cmd;
        cmd.telem_id = POWER_READ_TEMP_CMD;
        cmd.timestamp = now;
        cmd.length = 1;
        cmd.data[0] = num;
        sendCommand(&cmd,POWER_CSP_ADDRESS);
    } else {
        printf("Invalid command.\n");
    }

}

void powReadSolarCurrent(int argc, char **argv){

    if(argc == 2 || argc == 3){
        // Check the bounds of num
        int num = atoi(argv[2]);

        // Create timestamp for time tagged task (we'll just schedule them right away/now)
        Calendar_t now;
        time_t t = time(NULL);
        struct tm *tm = localtime(&t);
        timeToCalendar(tm,&now);
        // Create telemetry packet
        telemetryPacket_t cmd;
        cmd.telem_id = POWER_READ_SOLAR_CURRENT_CMD;
        cmd.timestamp = now;
        cmd.length = 1;
        cmd.data[0] = num;
        sendCommand(&cmd,POWER_CSP_ADDRESS);
    } else {
        printf("Invalid command.\n");
    }

}
void powReadLoadCurrent(int argc, char **argv){

    if(argc == 2 || argc == 3){
        // Check the bounds of num
        int num = atoi(argv[2]);

        // Create timestamp for time tagged task (we'll just schedule them right away/now)
        Calendar_t now;
        time_t t = time(NULL);
        struct tm *tm = localtime(&t);
        timeToCalendar(tm,&now);
        // Create telemetry packet
        telemetryPacket_t cmd;
        cmd.telem_id = POWER_READ_LOAD_CURRENT_CMD;
        cmd.timestamp = now;
        cmd.length = 1;
        cmd.data[0] = num;
        sendCommand(&cmd,POWER_CSP_ADDRESS);
    } else {
        printf("Invalid command.\n");
    }

}
void powReadMSBVoltage(int argc, char **argv){

    if(argc == 2 || argc == 3){
        // Check the bounds of num
        int num = atoi(argv[2]);

        // Create timestamp for time tagged task (we'll just schedule them right away/now)
        Calendar_t now;
        time_t t = time(NULL);
        struct tm *tm = localtime(&t);
        timeToCalendar(tm,&now);
        // Create telemetry packet
        telemetryPacket_t cmd;
        cmd.telem_id = POWER_READ_MSB_VOLTAGE_CMD;
        cmd.timestamp = now;
        cmd.length = 1;
        cmd.data[0] = num;
        sendCommand(&cmd,POWER_CSP_ADDRESS);
    } else {
        printf("Invalid command.\n");
    }

}
void powSetMode(int argc, char **argv){

    if(argc == 2 || argc == 3){
        // Check the bounds of num
        int num = atoi(argv[2]);

        // Create timestamp for time tagged task (we'll just schedule them right away/now)
        Calendar_t now;
        time_t t = time(NULL);
        struct tm *tm = localtime(&t);
        timeToCalendar(tm,&now);
        // Create telemetry packet
        telemetryPacket_t cmd;
        cmd.telem_id = POWER_SET_POW_MODE_CMD;
        cmd.timestamp = now;
        cmd.length = 1;
        cmd.data[0] = num;
        sendCommand(&cmd,POWER_CSP_ADDRESS);
    } else {
        printf("Invalid command.\n");
    }

}