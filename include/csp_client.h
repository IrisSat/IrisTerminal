#ifndef CSP_CLIENT_H_
#define CSP_CLIENT_H_


int startcsp(char * comPort,int baudRate);

void ping(int argc,char **argv);
void runDiagnostics(int argc,char **argv);
void getNetworkInfo(int argc,char **argv);

#endif