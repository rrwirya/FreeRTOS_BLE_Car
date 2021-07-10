
/**
  **************************************************************************************************
  * @file           : car_app_freertos.h
  * @brief          : Header for car_app_freertos.c file.
  *
  * @author         : Reggie W
  **************************************************************************************************
  */


/* Define to prevent recursive inclusion ---------------------------------------------------------*/
#ifndef __CAR_APP_FREERTOS_H
#define __CAR_APP_FREERTOS_H


#ifdef __cplusplus
extern "C" {
#endif


/* Includes --------------------------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"


/* Exported types --------------------------------------------------------------------------------*/


/* Exported variables ----------------------------------------------------------------------------*/
	/*--- Variables related to Received BLE Message Task ---*/
	extern __IO uint32_t g_CountDirForward;
	extern __IO uint32_t g_CountDirLeft;
	extern __IO uint32_t g_CountDirRight;
	extern __IO uint32_t g_CountDirBack;
	extern __IO uint32_t g_CountDirForceStop;
	extern __IO uint32_t g_CarTotalDistanceCovered;


/* Exported defines ------------------------------------------------------------------------------*/
	/*--- Measurement/Calculation Parameters ---*/
	#define FREQUENCY_MS_CALCULATION			((uint8_t)25)
	#define FREQUENCY_S_CALCULATION				((float)FREQUENCY_MS_CALCULATION/1000.0f)

	/*--- Task Stack Sizes ---*/
	#define TASK_STACKSIZE_MIN					(64 * 4)			/* 256 words = 512 bytes */
	#define TASK_STACKSIZE_DEFAULT				(256 * 4)			/* 1024 words = 2048 bytes */
	#define TASK_STACKSIZE_MAX					(1024 * 4)			/* 4096 words = 8192 bytes */

	/*--- Task Priorities ---*/
	#define TASK_PRIO_BLE_CONN					osPriorityHigh6
	#define TASK_PRIO_BLE_MSG					osPriorityHigh4
	#define TASK_PRIO_CALCULATIONS				osPriorityHigh1
	#define TASK_PRIO_PB						osPriorityAboveNormal7
	#define TASK_PRIO_MCULED					osPriorityAboveNormal4
	#define TASK_PRIO_BLE_EVENTS				osPriorityNormal1


/* Exported constants ----------------------------------------------------------------------------*/


/* Exported macro --------------------------------------------------------------------------------*/


/* Exported Functions Prototypes -----------------------------------------------------------------*/
void FRTOS_Init_Mutex(void);
void FRTOS_Init_Semaphores(void);
void FRTOS_Init_SWTimers(void);
void FRTOS_Init_Queues(void);
void FRTOS_Init_Tasks(void);




#ifdef __cplusplus
}
#endif



#endif  /* __CAR_APP_FREERTOS_H */


/******************************************* END OF FILE *******************************************/

