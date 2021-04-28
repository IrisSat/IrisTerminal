#ifndef NETWORK_CONFIG_H_
#define NETWORK_CONFIG_H_

#include "telemetry.h"
#include <time.h>

//Define the csp address of all devices in the network.
#define POWER_CSP_ADDRESS	2
#define COMMS_CSP_ADDRESS	3
#define CDH_CSP_ADDRESS		4
#define PAYLOAD_CSP_ADDRESS	5
#define GROUND_CSP_ADDRESS	9

//Define the ports we're using
#define CSP_CMD_PORT	7
#define CSP_TELEM_PORT	8
#define CSP_UPDATE_PORT	9


#define CDH_NICKNAME "cdh"
#define PAYLOAD_NICKNAME "pld"

int nicknameToAddress(char * name);
int getAddr(char * arg);
int timeToCalendar(struct tm *time, Calendar_t *cal);
void getCalendarNow(Calendar_t* cal);
void printCalendar(Calendar_t* cal);

#endif