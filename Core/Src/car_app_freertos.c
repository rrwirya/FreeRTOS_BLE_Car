
/**
  **************************************************************************************************
  * @file           : car_app_freertos.c
  * @brief          :
  * @author			:
  **************************************************************************************************
  */


/* Includes --------------------------------------------------------------------------------------*/
#include "car_app_freertos.h"
#include "FreeRTOS.h"
#include "task.h"


/* Private includes ------------------------------------------------------------------------------*/
#include "car_app_ble.h"


/* Private typedef -------------------------------------------------------------------------------*/


/* Private define --------------------------------------------------------------------------------*/


/* Private macro ---------------------------------------------------------------------------------*/


/* Exported/Global variables ---------------------------------------------------------------------*/


/* External variables ----------------------------------------------------------------------------*/


/* Private variables -----------------------------------------------------------------------------*/
	/*--- FreeRTOS Task Handles ---*/
	static TaskHandle_t h_TaskBLEConn;
	static TaskHandle_t h_TaskBLEMsg;
	static TaskHandle_t h_TaskMcuLED;

	/*--- Variables to record remaining stack size of each tasks ---*/
	static UBaseType_t Task1_RSS, Task2_RSS;

/* Private function prototypes -------------------------------------------------------------------*/
static void Task_ManageBLEConnections(void *argument);
static void Task_ParseBLEMessage(void *argument);
static void Task_BlinkLEDIndicator(void *argument);


/* Private user code -----------------------------------------------------------------------------*/

/**
  **************************************************************************************************
  * FreeRTOS Objects Initialization															       *
  **************************************************************************************************
  */

void FRTOS_Init_Mutex(void)
{



}


void FRTOS_Init_Semaphores(void)
{



}

void FRTOS_Init_SWTimers(void)
{



}

void FRTOS_Init_Queues(void)
{



}

void FRTOS_Init_Tasks(void)
{
	BaseType_t TaskCreationStatus;

	/* Create task that will maintain BLE Connection */
	TaskCreationStatus = xTaskCreate( Task_ManageBLEConnections,
										"Task1 - BLE Connection",
										TASK_STACKSIZE_DEFAULT,
										NULL,
										TASK_PRIO_BLE_CONN,
										&h_TaskBLEConn);

	/* Ensure task creation succeeds */
	assert_param(TaskCreationStatus == pdPASS);

	/* Create task that will parse BLE Message */
	TaskCreationStatus = xTaskCreate( Task_ParseBLEMessage,
										"Task1 - BLE Message",
										TASK_STACKSIZE_DEFAULT,
										NULL,
										TASK_PRIO_BLE_MSG,
										&h_TaskBLEMsg);

	/* Ensure task creation succeeds */
	assert_param(TaskCreationStatus == pdPASS);

	TaskCreationStatus = xTaskCreate( Task_BlinkLEDIndicator,
										"Task2 - MCULED",
										TASK_STACKSIZE_MIN,
										NULL,
										TASK_PRIO_MCULED,
										&h_TaskMcuLED);

	/* Ensure task creation succeeds */
	assert_param(TaskCreationStatus == pdPASS);
}



/**
  **************************************************************************************************
  * Main Application																		       *
  **************************************************************************************************
  */

/**
 * @brief
 * @param
 * @retval
 * @note
 */
static void Task_ManageBLEConnections(void *argument)
{
	/* Initialize BLE Peripheral and place in advertising mode at startup to allow establishment
	 * of connections with BLE central devices in proximity.
	 */
	BlueNRG_Init();
	BlueNRG_MakeDeviceDiscoverable();

	while(1)
	{


	}

	/* Delete tasks automatically if somehow code reached this point */
	vTaskDelete(NULL);
}

/**
 * @brief
 * @param
 * @retval
 * @note
 */
static void Task_ParseBLEMessage(void *argument)
{
	/* Initialize BLE Peripheral and place in advertising mode at startup to allow establishment
	 * of connections with BLE central devices in proximity.
	 */

	while(1)
	{
		Task1_RSS = uxTaskGetStackHighWaterMark(NULL);

		uint32_t NotificationValue = ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
		if(NotificationValue & 0xFF)
		{

		}
	}

	/* Delete tasks automatically if somehow code reached this point */
	vTaskDelete(NULL);
}


/**
 * @brief
 * @param
 * @retval
 * @note
 */
static void Task_BlinkLEDIndicator(void *argument)
{
	const TickType_t DelayFrequency = pdMS_TO_TICKS(1000);
	TickType_t LastActiveTime;

	while(1)
	{
		/* Check ammount of unused stack. If returned value is 0, stack overflow has occurred */
		Task2_RSS = uxTaskGetStackHighWaterMark(NULL);

		/* Perform accurate blocking delay */
		LastActiveTime = xTaskGetTickCount();
		vTaskDelayUntil(&LastActiveTime, DelayFrequency);

		/* Toggle LED every 1 second */
		HAL_GPIO_TogglePin(NUCLEO_LED_GPIO_Port, NUCLEO_LED_Pin);
	}

	/* Delete tasks automatically if somehow code reached this point */
	vTaskDelete(NULL);
}

/**
  **************************************************************************************************
  * BLE Processes																			       *
  **************************************************************************************************
  */



