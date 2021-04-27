#ifndef CDH_CMD_H_
#define CDH_CMD_H_


typedef enum {
CDH_SCHEDULE_TTT_CMD =0,
CDH_DOWNLOAD_IMAGE_CMD,
NUM_CDH_COMMANDS
} cdhCommands_t;

typedef enum {
TASK_TAKE_IMAGE =0,
NUM_CDH_TASK
} cdhTask_t;

void getCdhTelemetry(int argc, char **argv);
void checkCdhTelemetry(int argc, char **argv);
void requestCdhTelemetry(int argc, char **argv);
void scheduleTTT(int argc, char **argv);
void checkTTT(int argc,char **argv);
void cancelTTT(int argc,char **argv);


#endif