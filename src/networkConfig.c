#include "networkConfig.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

int nicknameToAddress(char * name){

    if(strcmp(name,CDH_NICKNAME)==0){
        return CDH_CSP_ADDRESS;
    }
    else if(strcmp(name,PAYLOAD_NICKNAME) == 0){
        return PAYLOAD_CSP_ADDRESS;
    }

    return -1;
}

int getAddr(char * arg){

    int addr = -1;

    if(isdigit(arg[0])){
        addr = atoi(arg);
    }
    else{
        addr = nicknameToAddress(arg);
    }

    return addr;
}