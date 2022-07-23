/*
 * 
 * 
 */

#include <csp/csp.h>
#include <csp/arch/csp_thread.h>
#include "csp_client.h"

#include "main.h"
#include "commsCommands.h"
#include "settingsUtilities.h"
#include "payloadCommands.h"
#include "cdhCommands.h"
#include "powerCommands.h"



#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif


//Help groups/filter, 0 is all.
#define GENERAL_GRP 1
#define CDH_GRP     2
#define PLD_GRP     3
#define PWR_GRP     4
#define COMS_GRP    5

//Local Functions
static void printHelp(int argc, char ** argv);
static void quitTerminal(int argc, char ** argv);
static void setDebugLevel(int argc, char** argv);
int setupTerminal();
char * getline_(void);
int makeargs(char *args, int *argc, char ***aa);

//Structure for the commands.
typedef struct{
    int   group; //Commands grouped by system, so we can print help for one system only, since the full list is too long.
    char* name; //The name the user must type to use the command.
    void (*func)(int,char**);  //A pointer to the command funciton that will be run. Takes the standard argc,argv parameters.
    char * helpString; //The initial help text shown. Put more detailed help in the command function itself.
}cmd_t;

//Commands table. Add new commands here, make sure to update the NUM_COMMANDS.

#define NUM_COMMANDS    83

cmd_t commandTable[NUM_COMMANDS]= {
    // General Commands
    {GENERAL_GRP,"help", printHelp, "Prints the help message."},
    {GENERAL_GRP,"quit",quitTerminal,"Exit the terminal."},
    {GENERAL_GRP,"setDebug",setDebugLevel,"Set CSP debug level"},
    {GENERAL_GRP, "checksum", checksumFile,"Calculate the crc32 checksum for a file."},
    //Common for CSP
    {GENERAL_GRP,"ping",ping,"Pings the chosen subsystem, sending a packet of data and timing the response"},
    {GENERAL_GRP,"listProcess",listProcess,"Lists the tasks and their status of the chosen subsystem"},
    {GENERAL_GRP,"uptime",uptime,"Prints how long a subsystem has been turned on for."},
    {GENERAL_GRP,"memfree",memFree,"Prints the free memory of a subsystem."},
    {GENERAL_GRP,"buffFree",buffFree,"Prints the number of csp buffers available on a subsystem."},
    // CDH Commands
    {CDH_GRP,"scheduleTTT",scheduleTTT,"Schedules a new time tagged task."}, //10
    {CDH_GRP,"cancelTTT",cancelTTT,"Cancels a time tagged task."},
    {CDH_GRP,"downloadLogFile",downloadLogFile,"Download a log file from CDH."},
    {CDH_GRP,"cdhCheckTelemetry",cdhCheckTelemetry,"Checks what CDH telemetry is collected on CDH."},
    {CDH_GRP,"cdhGetTelemetry",cdhGetTelemetry,"Gets the latest CDH telemetry data from CDH."},
    {CDH_GRP,"cdhRequestTelemetry",cdhRequestTelemetry,"Tells CDH to generate new CDH telemetry."},
    {CDH_GRP,"cdhSetTime",cdhSetTime,"Updates the time on CDH."},
    {CDH_GRP,"cdhGetTime",cdhGetTime,"Gets the time on CDH."},
    {CDH_GRP,"cdhFileList",cdhFileList,"Lists the files in CDH data memory."},
    {CDH_GRP,"cdhUploadFw",cdhUploadFw,"Upload a firmware file to CDH."},
    {CDH_GRP,"cdhGetFwState", cdhGetFwState, "Get the current state of the CDH firmware update manager."}, //20
    {CDH_GRP,"cdhSetFwState", cdhSetFwState, "Request to set the CDH firmware update manager to the desired state."},
    {CDH_GRP,"cdhListFw",cdhListFw, "List the firmware files and their state."},
    {CDH_GRP,"cdhChecksumFile", cdhChecksumFile,"Calculate the checksum for a file on the cdh data memory."},
    {CDH_GRP,"cdhMvFile", cdhMvFile, "Move a file on the cdh data memory." },
    {CDH_GRP,"cdhRmFile", cdhRmFile, "Delete a file on the cdh data memory." },
    {CDH_GRP,"cdhCpFile", cdhCpFile, "Copy a file on the cdh data memory." },
    {CDH_GRP,"cdhChecksumFlash",cdhChecksumFlash,"Run checksum on flash memory area."},
    {CDH_GRP,"cdhCopyToFlash", cdhCopyToProgFlash, "Copy file to program flash."},
    {CDH_GRP,"cdhGetSwVersion", cdhGetSwVersion, "Get the software version from CDH."},
    {CDH_GRP,"cdhGetDesignVersion", cdhGetDesignVersion, "Get the design version from CDH."},
    {CDH_GRP,"cdhGetFwSpiDir", cdhGetFwSpiDir, "Get the firmware update(IAP) spi dir."},
    {CDH_GRP,"cdhGetFsFreeSpace",cdhFsGetFreeSpace, "Get remaining space in filesystem, in bytes."},
    {CDH_GRP,"cdhFwUpdateSpiDir", cdhFwUpdateSpiDir, "Update the fw design version in the spi dir."},
    {CDH_GRP,"cdhFwCreateSpiDir", cdhFwCreateSpiDir, "Create the spi dir for fw update, specify whole file."},
    {CDH_GRP,"cdhWriteProgFlash", cdhWriteProgFlash, "Write a file to the cdh program flash."},
    {CDH_GRP,"cdhEraseProgFlash", cdhEraseProgFlash,  "Erase blocks from the program flash."},
    {CDH_GRP,"cdhArmFw", cdhArmFw, "Arm the system for a firmware upgrade."},
    {CDH_GRP,"cdhUpgradeFw", cdhUpgradeFw, "Start the firmware upgrade."},
    {CDH_GRP,"cdhConfirmFw", cdhConfirmFw, "Confirm and finally do the firmware upgrade."},
    {CDH_GRP,"cdhResetFwMgr", cdhRestFwMgr, "Reset the Firmware manager."},
    {CDH_GRP,"cdhSetFwChecksum", cdhSetFwChecksum, "Set the checksum for the firmware files."},
    {CDH_GRP,"cdhSetFwDesignVer", cdhSetFwDesVer, "Set the design version for the firmware files."},
    {CDH_GRP,"cdhFormatFs", cdhFormatFs, "Format(erase) the file system."},
    {CDH_GRP,"cdhSoftReset", cdhSoftReset, "Request software reset of cdh system."},
    // Payload Commands
    {PLD_GRP,"pldCheckTelemetry",pldCheckTelemetry,"Checks what payload telemetry is collected on PLD."},
    {PLD_GRP,"pldGetTelemetry",pldGetTelemetry,"Gets the latest payload telemetry data from PLD."},
    {PLD_GRP,"pldRequestTelemetry",pldRequestTelemetry,"Tells CDH to request new telemtry from payload."},
    {PLD_GRP,"pldSendImage",pldSendImage,"Sends an image to the payload subsystem."},//30
    {PLD_GRP,"downloadImage",downloadImage,"Gets an image from payload."},
    {PLD_GRP,"deleteImage",deleteImage,"Deletes a specified payload image."},
    {PLD_GRP,"turnOnCamera1",turnoncamera1,"Turn on camera 1."},
    {PLD_GRP,"turnOffCamera1",turnoffcamera1,"Turn off camera 1."},
    {PLD_GRP,"turnOnCamera2",turnoncamera2,"Turn on camera 2."},
    {PLD_GRP,"turnOffCamera2",turnoffcamera2,"Turn off camera 2."},
    {PLD_GRP,"resetCamera1",resetcamera1,"Reset camera 1."},
    {PLD_GRP,"resetCamera2",resetcamera2,"Reset camera 2."},
    {PLD_GRP,"pldCamHs",pldCameraHandshake,"Do a handshake with one of the two cameras."},
    {PLD_GRP,"pldCamInit",pldCameraSensorInit,"Initialize one of the two cameras."},//40
    {PLD_GRP,"pldCamTxAddr",pldCameraSetI2cWriteAddress,"Sets Payload camera I2C write address."},
    {PLD_GRP,"pldCamRxAddr",pldCameraSetI2cReadAddress,"Sets Payload camera I2C read address."},
    {PLD_GRP,"pldCamTxData",pldCameraI2cTransmit,"Transmit I2C data to one of the two cameras."},
    {PLD_GRP,"pldCamRegListWrite",pldCameraRegListWrite,"Write a list of Payload camera registers."},
    {PLD_GRP,"pldCamTest",pldTestCameraInit,"Test Payload camera initialization."},
    // pldCameraRegListWrite
    {PLD_GRP,"listofImages",listofimages,"List of images."},
    {PLD_GRP,"icc1",takeimagecamera1,"Take image with camera 1."},
    {PLD_GRP,"icc2",takeimagecamera2,"Take image with camera 2."},
    {PLD_GRP,"pldFileList",pldFileList,"Gets a list of the files stored in Payload flash."},
    {PLD_GRP,"pldMountFs",pldMountFS,"Mount Payload filesystem."},//50
    {PLD_GRP,"pldUnmountFs",pldUnmountFS,"Unmount Payload filesystem."},
    {PLD_GRP,"pldRestartFs",pldRestartFS,"Restart Payload filesystem."},
    // Power Commands
    {PWR_GRP,"powReadTemp",powReadTempChannel,"Power - Read a temperature channel."},
    {PWR_GRP,"powReadSAC",powReadSolarArrayCurrent,"Power - Read a solar array current value."},
    {PWR_GRP,"powReadLC",powReadLoadCurrent,"Power - Read a load current value."},
    {PWR_GRP,"powReadMsbVoltage",powReadMsbVoltage,"Power - Read MSB voltage value."},
    {PWR_GRP,"powSetLoadSwitch",powSetLoadSwitch,"Power - Set load switch on/off."},
    {PWR_GRP,"powSetSolarSwitch",powReadSolarArrayCurrent,"Power - Set solar array switch on/off."},
    {PWR_GRP,"powSetMode",powSetMode,"Power - Set power mode."},
    {PWR_GRP,"powGetSoc",powGetBatterySoc,"Power - Get battery state of charge."}, //60
    {PWR_GRP,"powGetEclipse",powGetEclipse,"Power - Get state of eclipse."},
    {PWR_GRP,"powGetBootCount",powGetBootCount,"Power - Get boot count."},
    {PWR_GRP,"powSetSoc",powAitSetBatterySoc,"Power AIT - Set battery state of charge."},
    {PWR_GRP,"powSetEclipse",powAitSetEclipse,"Power AIT - Set state of eclipse."},

    // Comms Commands
    {COMS_GRP,"sendCommsMessage",commsSendCSPMessage,"Send COMMS a message."},
};


int main(int argc, char **argv) {

    char *cmd;
    int n;
    int argc_;
    char ** argv_;

    setupTerminal();
    char cwd[255];
    getcwd(cwd,255);
    printf("Current directory: %s\n",cwd);

    //User must specify 2 parameters(COM port and baud rate), so argc == 3
    if(argc == 1)
    {
        InitializeSettings();
        return 0;
    }
    if(argc != 3)
    {
        return 0;
    }
    else
    {
        startcsp(argv[1],atoi(argv[2]));
    }
    while(1) {
        //CMD line loop...

        
        printf("Iris>");
        cmd = getline_();


        makeargs(cmd,&argc_,&argv_);
        int found = 0;
        for(int i=0; i<NUM_COMMANDS; i++){
            
            //If the command entered by the user matches a command, call the command function.
            if(strcmp(commandTable[i].name,argv_[0]) ==0){
                commandTable[i].func(argc_,argv_);
                found =1;
                break;
            }
        }
        if(!found && strcmp(argv_[0],"")){
            printf("Could not find command: %s\n",argv_[0]);
        }

    }

    return 0;

}

static void printHelp(int argc, char ** argv){

    printf("This terminal provides commands for interacting with the Iris satellite.\n");
    printf("Type \"command\" help to see detailed help for a specific command.\n");
    printf("Type help <system> to see commands only for <system>. Valid systems are: general,cdh,pld,pwr,comms\n");
    printf("Here is a list of available commands:\n");

    int filter=0;
    if(argc>1){

        if(!strcmp(argv[1],"general")){
            filter = GENERAL_GRP;
        }
        else if(!strcmp(argv[1],"cdh")){
            filter = CDH_GRP;
        }
        else if(!strcmp(argv[1],"pld")){
            filter = PLD_GRP;
        }
        else if(!strcmp(argv[1],"pwr")){
            filter = PWR_GRP;
        }
        else if(!strcmp(argv[1],"comms")){
            filter = COMS_GRP;
        }


    }

    for(int i=0; i< NUM_COMMANDS; i++){
        //Print the name and help string of each command in the table. Using color for the command name to make it easier to read.
        if(filter == 0 || commandTable[i].group == filter){
            printf("\t\x1b[36m %s \x1b[0m - %s\n",commandTable[i].name,commandTable[i].helpString);
        }
    }
}

static void quitTerminal(int argc, char ** argv){
    //Just quit the program, add any other clean up if necessary.
    exit(0);
}

int setupTerminal(){
    //This configures the powershell terminal to support color.
    //https://docs.microsoft.com/en-us/windows/console/console-virtual-terminal-sequences#samples
    //https://stackoverflow.com/questions/51680709/colored-text-output-in-powershell-console-using-ansi-vt100-codes
    
    // Set output mode to handle virtual terminal sequences
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE)
    {
        return GetLastError();
    }

    DWORD dwMode = 0;
    if (!GetConsoleMode(hOut, &dwMode))
    {
        return GetLastError();
    }

    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    if (!SetConsoleMode(hOut, dwMode))
    {
        return GetLastError();
    }
}


int makeargs(char *args, int *argc, char ***aa) {
    //This takes a string and parses it into argc(number of arguments) and argv(array of arguments).
    //From Andi on stackopverflow: https://stackoverflow.com/a/24229245

    char *buf = strdup(args);
    int c = 1;
    char *delim;
    char **argv = calloc(c, sizeof (char *));

    argv[0] = buf;

    while (delim = strchr(argv[c - 1], ' ')) {
        argv = realloc(argv, (c + 1) * sizeof (char *));
        argv[c] = delim + 1;
        *delim = 0x00;
        c++;
    }

    *argc = c;
    *aa = argv;

    return c;
}

char * getline_(void) {
    //Reads a line from stdin.
    //From Johannes Schaub: https://stackoverflow.com/a/314422

    char * line = malloc(1000), * linep = line;
    size_t lenmax = 1000, len = lenmax;
    int c;

    if(line == NULL)
        return NULL;

    for(;;) {
        c = fgetc(stdin);
        if(c == EOF)
            break;

        if(--len == 0) {
            len = lenmax;
            char * linen = realloc(linep, lenmax *= 2);

            if(linen == NULL) {
                free(linep);
                return NULL;
            }
            line = linen + (line - linep);
            linep = linen;
        }

        if((*line++ = c) == '\n'){
            *--line =0;
            break;
        }
    }
    *line = '\0';
    return linep;
}

void setDebugLevel(int argc, char** argv){

    //  switch(atoi(argv[0])){

    //      case 0
    //  }
     csp_debug_toggle_level(atoi(argv[1]));
}