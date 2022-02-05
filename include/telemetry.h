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

// Power Telemetry
typedef enum
{
POWER_READ_TEMP_ID = 50,
POWER_READ_SOLAR_CURRENT_ID,
POWER_READ_LOAD_CURRENT_ID,
POWER_READ_MSB_VOLTAGE_ID,
POWER_MSG_ID
} powerTelemetry_t;
typedef enum
{
POWER_READ_TEMP_CMD = 0,
POWER_READ_SOLAR_CURRENT_CMD,
POWER_READ_LOAD_CURRENT_CMD,
POWER_READ_MSB_VOLTAGE_CMD,
POWER_SET_POW_MODE_CMD,
} powerCommands_t;

// Payload Telemetry.
typedef enum {
PAYLOAD_POWER_GOOD_ID = 0,
PAYLOAD_BOARD_TEMP_ID,
PAYLOAD_SAMPLE_TEMP_ID,
PAYLOAD_FULL_IMAGE_ID,
PAYLOAD_SAMPLE_LOC_ID,
PAYLOAD_CAMERA_TIME_ID,
PAYLOAD_ERROR_ID,
PAYLOAD_FULL_IMAGE_RX, //Only for debugging remove later
PAYLOAD_FILE_LIST_ID,
PAYLOAD_META_ID,
PAYLOAD_IMAGE_INFO,
PAYLOAD_ACK,
NUM_PAYLOAD_TELEMETRY,
PAYLOAD_MSG_ID
} payloadTelemetry_t;
typedef enum {
PAYLOAD_POWER_GOOD_CMD = 0,
PAYLOAD_BOARD_TEMP_CMD,
PAYLOAD_SAMPLE_TEMP_CMD,
PAYLOAD_FULL_IMAGE_CMD,
PAYLOAD_TAKE_IMAGE_CAM1_CMD,
PAYLOAD_TAKE_IMAGE_CAM2_CMD,
PAYLOAD_CAM1_ON_CMD,
PAYLOAD_CAM2_ON_CMD,
PAYLOAD_CAM1_OFF_CMD,
PAYLOAD_CAM2_OFF_CMD,
PAYLOAD_CAM1_RESET_CMD,
PAYLOAD_CAM2_RESET_CMD,
PAYLOAD_ENTER_LOW_POWER_CMD,
PAYLOAD_EXIT_LOW_POWER_CMD,
PAYLOAD_FILE_LIST_CMD,
PAYLOAD_SHUTDOWN_CMD,
PAYLOAD_TURNONCAM1_CMD,
NUM_PAYLOAD_COMMANDS,
PAYLOAD_MOUNT_FS,
PAYLOAD_UNMOUNT_FS
} payloadCommands_t;

// CDH Telemetry
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
TASK_READ_POW_ADC,
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
