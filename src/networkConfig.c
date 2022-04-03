#include "networkConfig.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>


int nicknameToAddress(char * name){

    if(strcmp(name,CDH_NICKNAME)==0){
        return CDH_CSP_ADDRESS;
    }
    else if(strcmp(name,PAYLOAD_NICKNAME) == 0){
        return PAYLOAD_CSP_ADDRESS;
    }
    else if(strcmp(name,COMMS_NICKNAME) == 0){
        return COMMS_CSP_ADDRESS;
    }

    return -1;
}

int getAddr(char * arg){

    int addr = -1;

    if(isdigit(arg[0])){
        addr = atoi(arg);
    }
    else{
        addr = nicknameToAddress(arg);
    }

    return addr;
}

int timeToCalendar(struct tm *time, Calendar_t *cal){

    cal->year = time->tm_year-100; //C provides year since 1900, we want year since 2000...
    cal->month = time->tm_mon+1;//C provides month as 0-11, we want 1-12...
    cal->day = time->tm_mday;
    cal->hour = time->tm_hour;
    cal->minute = time->tm_min;
    cal->second = time->tm_sec;
    cal->week = 1; //Not used but must be above 1...
    cal->weekday = time->tm_wday+1; //C provides sunday = 0, we want sunday = 1... But weekday doesn't seem to be used anywhere...

}

void getCalendarNow(Calendar_t* cal){
    time_t t = time(NULL);
    struct tm *time = localtime(&t);

    cal->year = time->tm_year-100; //C provides year since 1900, we want year since 2000...
    cal->month = time->tm_mon+1;//C provides month as 0-11, we want 1-12...
    cal->day = time->tm_mday;
    cal->hour = time->tm_hour;
    cal->minute = time->tm_min;
    cal->second = time->tm_sec;
    cal->week = 1; //Not used but must be above 1...
    cal->weekday = time->tm_wday+1; //C provides sunday = 0, we want sunday = 1... But weekday doesn't seem to be used anywhere...
}

void printCalendar(Calendar_t* cal){

    char * month;
    switch (cal->month){
        case 1: month="January"; break;
        case 2: month="February"; break;
        case 3: month="March"; break;
        case 4: month="April"; break;
        case 5: month="May"; break;
        case 6: month="June"; break;
        case 7: month="July"; break;
        case 8: month="August"; break;
        case 9: month="September"; break;
        case 10: month="October"; break;
        case 11: month="November"; break;
        case 12: month="December"; break;

    }

    printf("The time is %d:%d:%d %s %d %d\n",cal->hour,cal->minute,cal->second,month,cal->day,cal->year+2000);

}