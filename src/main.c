/**
 * Build this example on linux with:
 * ./waf configure --enable-examples --enable-if-kiss --with-driver-usart=linux --enable-crc32 clean build
 */

#include <stdio.h>
#include <csp/csp.h>
#include <csp/arch/csp_thread.h>
#include "csp_client.h"
#include "argtable3.h"

#include "payloadCommands.h"

/* global arg_xxx structs */
struct arg_lit *verb, *help, *version;
struct arg_int *level;
struct arg_file *o, *file;
struct arg_end *end;

static void printHelp(int argc, char ** argv);

typedef struct{
    char* name;
    void (*func)(int,char**);
}cmd_t;

#define NUM_COMMANDS    3
cmd_t commandTable[NUM_COMMANDS]= {
    {"help", printHelp},
    {"send_image",sendImage},
    {"ping",ping}
};

int makeargs(char *args, int *argc, char ***aa) {
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

int main(int argc, char **argv) {
   
   char *cmd;
   int n;
   int argc_;
   char ** argv_;

    //    void *argtable[] = {
    //     help    = arg_litn(NULL, "help", 0, 1, "display this help and exit"),
    //     version = arg_litn(NULL, "version", 0, 1, "display version info and exit"),
    //     level   = arg_intn(NULL, "level", "<n>", 0, 1, "foo value"),
    //     verb    = arg_litn("v", "verbose", 0, 1, "verbose output"),
    //     o       = arg_filen("o", NULL, "myfile", 0, 1, "output file"),
    //     file    = arg_filen(NULL, NULL, "<file>", 1, 100, "input files"),
    //     end     = arg_end(20),
    // };

   //startcsp();
    /* Wait for program to terminate (ctrl + c) */
    if(argc !=3){
        return 0;
    }
    else{
        startcsp(argv[1],atoi(argv[2]));
    }
    while(1) {

        printf("Iris>");
        cmd = getline_();
        makeargs(cmd,&argc_,&argv_);
        // int nerrors;
        // nerrors = arg_parse(argc_,argv_,argtable);

        for(int i=0; i<NUM_COMMANDS; i++){

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
    printf("Type \"command\" help to see help for a specific command.\n");
    printf("Here is a list of available commands:\n");

    for(int i=0; i< NUM_COMMANDS; i++){

        printf("\t%s\n",commandTable[i].name);
    }
}