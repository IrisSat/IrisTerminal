/*
 * 
 * 
 */

#include <stdio.h>
#include <csp/csp.h>
#include <csp/arch/csp_thread.h>
#include "csp_client.h"
#include <unistd.h>

#include "payloadCommands.h"
#include "cdhCommands.h"
#include "powerCommands.h"

#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif

//Local Functions
static void printHelp(int argc, char ** argv);
static void quitTerminal(int argc, char ** argv);
int setupTerminal();
char * getline_(void);
int makeargs(char *args, int *argc, char ***aa);

//Structure for the commands.
typedef struct{
    char* name; //The name the user must type to use the command.
    void (*func)(int,char**);  //A pointer to the command funciton that will be run. Takes the standard argc,argv parameters.
    char * helpString; //The initial help text shown. Put more detailed help in the command function itself.
}cmd_t;

//Commands table. Add new commands here, make sure to update the NUM_COMMANDS.
#define NUM_COMMANDS    39
cmd_t commandTable[NUM_COMMANDS]= {
    {"help", printHelp, "Prints the help message."},
    {"send_image",sendImage,"Sends an image to the payload subsystem"},
    {"ping",ping,"Pings the chosen subsystem, sending a packet of data and timing the response"},
    {"listProcess",listProcess,"Lists the tasks and their status of the chosen subsystem"},
    {"uptime",uptime,"Prints how long a subsystem has been turned on for."},
    {"memfree",memFree,"Prints the free memory of a subsystem."},
    {"buffFree",buffFree,"Prints the number of csp buffers available on a subsystem."},
    {"getPldTelem",getPayloadTelemetry,"Gets the latest payload telemetry data from CDH."},
    {"checkPldTelem",checkPayloadTelemetry,"Checks what payload telemetry is collected on CDH."},
    {"requestPldTelem",requestPayloadTelemetry,"Tells CDH to request new telemtry from payload."},
    {"getCdhTelem",getCdhTelemetry,"Gets the latest CDH telemetry data from CDH."},
    {"checkCdhTelem",checkCdhTelemetry,"Checks what CDH telemetry is collected on CDH."},
    {"requestCdhTelem",requestCdhTelemetry,"Tells CDH to generate new CDH telemetry."},
    {"setCdhTime",setCdhTime,"Updates the time on CDH."},
    {"getCdhTime",getCdhTime,"Gets the time on CDH."},
    {"downloadImage",downloadImage,"Gets an image from payload."},
    {"deleteImage",deleteImage,"Deletes a specified payload image."},
    {"scheduleTTT",scheduleTTT,"Schedules a new time tagged task."},
    {"cancelTTT",cancelTTT,"Cancels a time tagged task."},
    {"quit",quitTerminal,"Exit the terminal."},
    {"turnOnCamera1",turnoncamera1,"Turn on camera 1."},
    {"turnOffCamera1",turnoffcamera1,"Turn off camera 1."},
    {"turnOnCamera2",turnoncamera2,"Turn on camera 2."},
    {"turnOffCamera2",turnoffcamera2,"Turn off camera 2."},
    {"resetCamera1",resetcamera1,"Reset camera 1."},
    {"resetCamera2",resetcamera2,"Reset camera 2."},
    {"listofImages",listofimages,"List of images."},
    {"icc1",takeimagecamera1,"Take image with camera 1."},
    {"icc2",takeimagecamera2,"Take image with camera 2."},
    {"pldFileList",pldFileList,"Gets a list of the files stored in Payload flash."},
    {"pldMountFS",pldMountFS,"Mount the payload filesystem."},
    {"pldUnmountFS",pldUnmountFS,"Unmount the payload filesystem."},
    {"powReadTemp",powReadTemp,"Power - Read a temperature value."},
    {"powReadSC",powReadSolarCurrent,"Power - Read a solar current value."},
    {"powReadLC",powReadLoadCurrent,"Power - Read a load current value."},
    {"powReadMsbVoltage",powReadMsbVoltage,"Power - Read MSB voltage value."},
    {"powSetLoad",powSetLoad,"Power - Set load switch on/off (choose 0-6)."},
    {"powSetSolar",powSetSolar,"Power - Set solar array switch on/off (choose 0-6)."},
    {"powSetMode",powSetMode,"Power - Set power mode."},
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
    if(argc !=3){
        return 0;
    }
    else{
        startcsp(argv[1],atoi(argv[2]));
    }
    while(1) {
        //CMD line loop...

        printf("Iris>");
        cmd = getline_();
        makeargs(cmd,&argc_,&argv_);
        
        for(int i=0; i<NUM_COMMANDS; i++){
            
            //If the command entered by the user matches a command, call the command function.
            if(strcmp(commandTable[i].name,argv_[0]) ==0){
                commandTable[i].func(argc_,argv_);
                break;
            }
        }


    }

    return 0;

}

static void printHelp(int argc, char ** argv){

    printf("This terminal provides commands for interacting with the Iris satellite.\n");
    printf("Type \"command\" help to see detailed help for a specific command.\n");
    printf("Here is a list of available commands:\n");

    for(int i=0; i< NUM_COMMANDS; i++){
        //Print the name and help string of each command in the table. Using color for the command name to make it easier to read.
        printf("\t\x1b[36m %s \x1b[0m - %s\n",commandTable[i].name,commandTable[i].helpString);
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
