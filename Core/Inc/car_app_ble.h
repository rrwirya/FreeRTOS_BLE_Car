
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
#define DEVICE_TYPE_GAP_PERIPHERAL
#define TEXTSIZE											50
#define UART_TIMEOUT										1000

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


/* Exported variables ----------------------------------------------------------------------------*/
extern char pText[TEXTSIZE];


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
} connectionStatus_t;


/* Exported constants ----------------------------------------------------------------------------*/


/* Exported macro --------------------------------------------------------------------------------*/


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
