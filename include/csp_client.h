#ifndef CSP_CLIENT_H_
#define CSP_CLIENT_H_

typedef struct{
	uint8_t second;
	uint8_t minute;
	uint8_t hour;
	uint8_t day;
	uint8_t month;
	uint8_t year;
	uint8_t weekday;
	uint8_t week;

}Calendar_t;

int startcsp(char * comPort,int baudRate);
int csp_transaction_2port(uint8_t prio, uint8_t dest, uint8_t port, uint8_t rx_port, uint32_t timeout, void * outbuf, int outlen, void * inbuf, int inlen) ;

void ping(int argc,char **argv);
void listProcess(int argc,char **argv);
void uptime(int argc,char **argv);
void runDiagnostics(int argc,char **argv);
void getNetworkInfo(int argc,char **argv);

#endif