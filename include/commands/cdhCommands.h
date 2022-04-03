#ifndef CDH_CMD_H_
#define CDH_CMD_H_
#include "telemetry.h"

void downloadLogFile(int argc, char **argv);
void getCdhTelemetry(int argc, char **argv);
void checkCdhTelemetry(int argc, char **argv);
void requestCdhTelemetry(int argc, char **argv);
void scheduleTTT(int argc, char **argv);
void scheduleTTT_raw(int code,Calendar_t when);
void checkTTT(int argc,char **argv);
void cancelTTT(int argc,char **argv);
void setCdhTime(int argc, char **argv);
void getCdhTime(int argc, char **argv);

uint8_t decodeLogFileNames(char * arg);

#endif