#ifndef CDH_CMD_H_
#define CDH_CMD_H_
#include "telemetry.h"

void downloadLogFile(int argc, char **argv);
void cdhGetTelemetry(int argc, char **argv);
void cdhCheckTelemetry(int argc, char **argv);
void cdhRequestTelemetry(int argc, char **argv);
void scheduleTTT(int argc, char **argv);
void scheduleTTT_raw(int code,Calendar_t when);
void checkTTT(int argc,char **argv);
void cancelTTT(int argc,char **argv);
void cdhSetTime(int argc, char **argv);
void cdhGetTime(int argc, char **argv);
void cdhFileList(int argc, char **argv);
void cdhUploadFw(int argc, char **argv);
void cdhGetFwState(int argc, char **argv);
void cdhListFw(int argc, char **argv);
void cdhSetFwState(int argc, char **argv);
void cdhChecksumFile(int argc, char **argv);
void checksumFile(int argc, char **argv);
void cdhMvFile(int argc, char **argv);
void cdhRmFile(int argc, char **argv);
void cdhCpFile(int argc, char **argv);


uint8_t decodeLogFileNames(char * arg);

#endif