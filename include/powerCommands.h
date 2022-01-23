#ifndef POWER_CMD_H_
#define POWER_CMD_H_


void powReadTemp(int argc, char **argv);
void powReadSolarCurrent(int argc, char **argv);
void powReadLoadCurrent(int argc, char **argv);
void powReadMSBVoltage(int argc, char **argv);
void powSetMode(int argc, char **argv);

#endif