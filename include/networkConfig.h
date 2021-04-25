#ifndef NETWORK_CONFIG_H_
#define NETWORK_CONFIG_H_

#define CDH_CSP_ADDR 4  
#define PAYLOAD_CSP_ADDR 5

#define CDH_NICKNAME "cdh"
#define PAYLOAD_NICKNAME "pld"

int nicknameToAddress(char * name);
int getAddr(char * arg);

#endif