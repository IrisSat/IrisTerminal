/*
 * 
 * 
 */
#include <stdio.h>
#include <stdlib.h>

#include <csp/csp.h>
#include <csp/arch/csp_thread.h>
#include "csp_client.h"

#include "main.h"
#include "commsCommands.h"
#include "settingsUtilities.h"
#include "payloadCommands.h"
#include "cdhCommands.h"
#include "powerCommands.h"

#include "commands.h"

#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif

//Local Functions
int setupTerminal();
char * getline_(void);
int makeargs(char *args, int *argc, char ***aa);

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

