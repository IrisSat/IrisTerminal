#ifndef POWER_CMD_H_
#define POWER_CMD_H_

typedef enum
{
    LCC_MINCO,
    LCC_ANT_DPL,
    LCC_COMS,
    LCC_ADCS,
    LCC_DATEC,
    LCC_PLD,
    LCC_CDH,
    NUM_LOAD_CURRENT_CHANNELS
} LoadCurrentChannels_t;

typedef enum
{
    LS_DATEC,
    LS_ADCS,
    LS_COMS,
    LS_CDH,
    LS_PLD,
    LS_DPL_A,
    LS_DPL_S,
    NUM_LOAD_SWITCHES
} LoadSwitch_t;

typedef enum
{
    DETUMBLE_MODE,
    CRITICAL_HOLD_MODE,
    SURVIVAL_MODE,
    LOW_POWER_MODE,
    IDLE_MODE,
    NORMAL_MODE,
    SUN_POINTING_MODE,
    SCIENCE_MODE,
    NUM_POWER_MODES
} PowerMode_t;


// Get methods
void powReadTempChannel(int argc, char **argv);
void powReadSolarArrayCurrent(int argc, char **argv);
void powReadLoadCurrent(int argc, char **argv);
void powReadMsbVoltage(int argc, char **argv);
void powGetBatterySoc(int argc, char **argv);
void powGetEclipse(int argc, char **argv);
// Set methods
void powSetLoadSwitch(int argc, char **argv);
void powSetSolarSwitch(int argc, char **argv);
void powSetMode(int argc, char **argv);
void powAitSetBatterySoc(int argc, char **argv);
void powAitSetEclipse(int argc, char **argv);
// Helper methods
int decodeLoadCurrentChannel(char * arg);
int decodeLoadSwitchNumber(char * arg);
int decodePowerModes(char * arg);

#endif