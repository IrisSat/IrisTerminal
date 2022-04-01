#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "commandIO.h"
#include "commsCommands.h"
#include "payloadCommands.h"
#include "cdhCommands.h"
#include "powerCommands.h"

int outputToFile = 0;
FILE *outputFilePtr;

void printfToOutput(const char *printValue, ...)
{
	va_list argp;
	va_start(argp, printValue);
	
	if (outputToFile == 1)
	{
		vfprintf(outputFilePtr, printValue, argp);
	} else {
		vprintf(printValue, argp);
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

void runCommandFile(int argc, char ** argv){

   int num;
   FILE *commandsFilePtr;
   char * line = NULL;
   size_t len = 0;
   
   if (argc != 3) {
	   return;
   }

   commandsFilePtr = fopen(argv[1],"r");
   
   if(commandsFilePtr == NULL)
   {
      printf("Error: Cannot open file: %s", argv[1]);   
      return;          
   }
   
   outputFilePtr = fopen(argv[2],"w");
   
   if(outputFilePtr == NULL)
   {
      printf("Error: Cannot open file: %s", argv[2]);   
      return;          
   }

   while (getline(&line, &len, fp) != -1) {
        printf("Executing command:\n");
        printf("%s\n", line);
		executeCommand(line);
    }

   printf("All commands completed and printed to: %s\n", argv[2]);
   fclose(outputFilePtr);
}

void executeCommand(char * cmd) {
	int argc_;
    char ** argv_;
   
	makeargs(cmd,&argc_,&argv_);
	for(int i=0; i<NUM_COMMANDS; i++){
		
		//If the command entered by the user matches a command, call the command function.
		if(strcmp(commandTable[i].name,argv_[0])==0){
			commandTable[i].func(argc_,argv_);
			break;
		}
	}
}

void printHelp(int argc, char ** argv){

    printf("This terminal provides commands for interacting with the Iris satellite.\n");
    printf("Type \"command\" help to see detailed help for a specific command.\n");
    printf("Here is a list of available commands:\n");

    for(int i=0; i< NUM_COMMANDS; i++){
        //Print the name and help string of each command in the table. Using color for the command name to make it easier to read.
        printf("\t\x1b[36m %s \x1b[0m - %s\n",commandTable[i].name,commandTable[i].helpString);
    }
}

void quitTerminal(int argc, char ** argv){
    //Just quit the program, add any other clean up if necessary.
    exit(0);
}
