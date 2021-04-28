/*
 * telemetry.h
 *	Note telemetry and command is used interchangably here.
 *  Created on: Mar. 17, 2021
 *      Author: Joseph Howarth
 */

#ifndef INC_TELEMETRY_H_
#define INC_TELEMETRY_H_

#include <stdint.h>
#include <csp/csp.h>


/***********************************************************/
//Put the commands/telemtry here.

//Payload telemetry.
typedef enum {
PAYLOAD_POWER_GOOD_ID,
PAYLOAD_BOARD_TEMP_ID,
PAYLOAD_SAMPLE_TEMP_ID,
PAYLOAD_FULL_IMAGE_ID,
PAYLOAD_R_REFLECTANCE_ID,
PAYLOAD_G_REFLECTANCE_ID,
PAYLOAD_B_REFLECTANCE_ID,
PAYLOAD_SAMPLE_LOC_ID,
PAYLOAD_CAMERA_TIME_ID,
PAYLOAD_ERROR_ID,
PAYLOAD_FULL_IMAGE_RX, //Only for debugging remove later
PAYLOAD_ACK,
NUM_PAYLOAD_TELEMETRY
} payloadTelemetry_t;

typedef enum {
PAYLOAD_POWER_GOOD_CMD = 0,
PAYLOAD_BOARD_TEMP_CMD,
PAYLOAD_SAMPLE_TEMP_CMD,
PAYLOAD_FULL_IMAGE_CMD,
PAYLOAD_R_REFLECTANCE_CMD,
PAYLOAD_G_REFLECTANCE_CMD,
PAYLOAD_B_REFLECTANCE_CMD,
PAYLOAD_SAMPLE_LOC_CMD,
PAYLOAD_CAMERA_TIME_CMD,
PAYLOAD_ERROR_CMD,
NUM_PAYLOAD_COMMANDS
} payloadCommands_t;


typedef enum {
CDH_TIME =0,
CDH_BOARD_TEMP,
CDH_TTT_LIST,
NUM_CDH_TELEMETRY
} cdhTelemetry_t;

typedef enum {
CDH_SCHEDULE_TTT_CMD =0,
CDH_SET_TIME_CMD,
CDH_GET_TIME_CMD,
CDH_DOWNLOAD_IMAGE_CMD,
NUM_CDH_COMMANDS
} cdhCommands_t;

typedef enum {
TASK_TAKE_IMAGE =0,
NUM_CDH_TASK
} cdhTask_t;

/**********************************************************/

typedef struct{
	uint8_t second;
	uint8_t minute;
	uint8_t hour;
	uint8_t day;
	uint8_t month;
	uint8_t year; //years since 2000!
	uint8_t weekday;
	uint8_t week;

}Calendar_t;


typedef struct{

	Calendar_t timestamp;
	uint8_t telem_id;		//Make sure there is less than 255 commands/telemetry ids for any subsystem. Or change to uint16_t.
	uint8_t length;
	uint8_t* data;

} telemetryPacket_t;
#define TELEM_HEADER_SIZE	(sizeof(Calendar_t)+2)

/**********************************************************/
void unpackTelemetry(uint8_t * data, telemetryPacket_t* output);//Unpacks the telemetry into the telemetry packet struct.
void sendTelemetry(telemetryPacket_t * packet);//Sends telemetry to CDH.
void sendTelemetry_direct(telemetryPacket_t * packet,csp_conn_t * conn); //For directly responding to a message.
void sendCommand(telemetryPacket_t * packet,uint8_t addr);//Sends a cmd packet to the cmd port of the subsytem at address addr.
void sendTelemetryAddr(telemetryPacket_t * packet,uint8_t addr); //Sends telemetry directly to a subsystem.
/**********************************************************/

#endif /* INC_TELEMETRY_H_ */
