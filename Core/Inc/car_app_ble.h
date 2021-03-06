
/**
  **************************************************************************************************
  * @file           : car_app_ble.h
  * @brief          : Header for car_app_ble.c file
  * @author         : Reggie W
  **************************************************************************************************
  */


/* Define to prevent recursive inclusion ---------------------------------------------------------*/
#ifndef __CAR_APP_BLE_H
#define __CAR_APP_BLE_H


#ifdef __cplusplus
extern "C" {
#endif


/* Includes --------------------------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include "main.h"
#include "bluenrg_conf.h"


/* Exported defines ------------------------------------------------------------------------------*/
	/*--- General BLE Layer Defines ---*/
	#define DEVICE_TYPE_GAP_PERIPHERAL
	#define TEXTSIZE											50
	#define UART_TIMEOUT										1000
 	#define MAX_DATA_EXCHANGE_BYTES								4
	#define DATA_EXCHANGE_6_BYTES								6

	/*--- BLE Motor Direction Commands ---*/
	#define BLEMOT_CMD_N							((uint8_t)0x4E)
	#define BLEMOT_CMD_N_LOWER						((uint8_t)0x6E)
	#define BLEMOT_CMD_E							((uint8_t)0x45)
	#define BLEMOT_CMD_E_LOWER						((uint8_t)0x65)
	#define BLEMOT_CMD_S							((uint8_t)0x53)
	#define BLEMOT_CMD_S_LOWER						((uint8_t)0x73)
	#define BLEMOT_CMD_W							((uint8_t)0x57)
	#define BLEMOT_CMD_W_LOWER						((uint8_t)0x77)
	#define BLEMOT_CMD_X							((uint8_t)0x58)
	#define BLEMOT_CMD_X_LOWER						((uint8_t)0x78)
	#define BLEMOT_CMD_R							((uint8_t)0x52)		/* Might be unused since 'SOUTH' is reverse direction */
	#define BLEMOT_CMD_R_LOWER						((uint8_t)0x72)		/* Might be unused since 'SOUTH' is reverse direction */

   /**
    * @brief GAP Roles
	*
	* Use Peripheral for slave devices to allow master to connect to it, and central for master
	* devices to allow connecting to slave devices. For this project, the Car BLE is the peripheral
	* devices, and the smartphone possessing BLE Scanner apps are central devices.
	*/
	#define GAP_ROLE_PERIPHERAL								((uint8_t)0x01)
	#define GAP_ROLE_BROADCASTER							((uint8_t)0x02)
	#define GAP_ROLE_CENTRAL								((uint8_t)0x04)
	#define GAP_ROLE_OBSERVER								((uint8_t)0x08)

   /**
	* @brief GAP Privacy/Security
	*
	* Use Peripheral for slave devices to allow master to connect to it, and central for master
	* devices to allow connecting to slave devices. For this project, the Car BLE is peripheral
	* devices, and the smartphone possessing BLE Scanner apps are central devices.
	*/
	#define GAP_PRIVACY_DISABLED							((uint8_t)0x00)
	#define GAP_PRIVACY_HOST_ENABLED						((uint8_t)0x01)
	#define GAP_PRIVACY_CONTROLLER_ENABLED					((uint8_t)0x02)


	/*--- FreeRTOS Task Notification Values ---*/

	/* Notification values related to BLE connectivity */
	#define FRTOS_TASK_NOTIF_BLE_DISCONNECTED				((uint16_t)0x0001)
	#define FRTOS_TASK_NOTIF_BLE_CONNECTED					((uint16_t)0x0002)

	/* Notification values related to Motor directions */
	#define FRTOS_TASK_NOTIF_DIR_NORTH						((uint16_t)0x0001)
	#define FRTOS_TASK_NOTIF_DIR_EAST						((uint16_t)0x0002)
	#define FRTOS_TASK_NOTIF_DIR_SOUTH						((uint16_t)0x0004)
	#define FRTOS_TASK_NOTIF_DIR_WEST						((uint16_t)0x0008)
	#define FRTOS_TASK_NOTIF_DIR_FORCESTOP					((uint16_t)0x8000)


/* Exported types --------------------------------------------------------------------------------*/

/* Enums that lists out all possible connection states */
typedef enum
{
	STATE_NOT_CONNECTED 		= 0x00,
	STATE_AWAITING_CONNECTION 	= 0x11,
	STATE_CONNECTED 			= 0x99,

} BLE_State_t;

/* Struct to hold BLE connection related data/parameters */
typedef struct
{
	uint8_t BLE_Client_Addr[6];
	uint8_t deviceRole;					/* deviceRole should be 0x01 when connected (indicating slave) */
	uint16_t connectionhandle;			/* Handle to connection between the GATT central and GATT peripheral */
	uint16_t BLE_ConnInterval;			/* Timing parameters of BLE */
	uint16_t BLE_ConnLatency;			/* Timing parameters of BLE */
	uint16_t BLE_SupervisionTimeout;	/* Timing parameters of BLE */
	BLE_State_t ConnectionStatus;		/* Connection status, will be used in FSM */
} ConnectionStatus_t;


/* Exported variables ----------------------------------------------------------------------------*/
extern char pText[TEXTSIZE];
extern ConnectionStatus_t Conn_Details;


/* Exported constants ----------------------------------------------------------------------------*/


/* Exported macro --------------------------------------------------------------------------------*/
	#define BLE_DATA_BYTES(x)							(x)


/* Exported Functions ----------------------------------------------------------------------------*/
/*** BLE Stack and System Init ***/
void BlueNRG_Init(void);
void BlueNRG_MakeDeviceDiscoverable(void);

/*** Custom BLE HCI Functions and Events ***/
void APP_UserEvtRx(void *pData);

/*** User Application Related Routines/Functions ***/
void BlueNRG_Loop(void);



#ifdef __cplusplus
}
#endif



#endif  /* __CAR_APP_BLE_H */


/******************************************* END OF FILE *******************************************/
