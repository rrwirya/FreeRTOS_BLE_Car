
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
#include "hci.h"
#include "hci_tl.h"


/* Private includes ------------------------------------------------------------------------------*/
#include "car_app_ble.h"


/* Private typedef -------------------------------------------------------------------------------*/


/* Private define --------------------------------------------------------------------------------*/


/* Private macro ---------------------------------------------------------------------------------*/


/* Exported/Global variables ---------------------------------------------------------------------*/


/* External variables ----------------------------------------------------------------------------*/


/* Private variables -----------------------------------------------------------------------------*/
	/*--- FreeRTOS Task Handles ---*/
	TaskHandle_t h_TaskBLEConn;
	static TaskHandle_t h_TaskBLEMsg;
	static TaskHandle_t h_TaskMcuLED;
	static TaskHandle_t h_TaskBLEEvents;

	/*--- Private variables related to BLE Connection Task ---*/


	/*--- Variables to record remaining stack size of each tasks ---*/
	UBaseType_t Task0_RSS, Task1_RSS, Task2_RSS, Task3_RSS;


/* Private function prototypes -------------------------------------------------------------------*/
static void Task_ManageBLEConnections(void *argument);
static void Task_ParseBLEMessage(void *argument);
static void Task_BlinkLEDIndicator(void *argument);
static void Task_ManageBLEEvents(void *argument);


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

	/* Create task that will blink MCU LED periodically */
	TaskCreationStatus = xTaskCreate( Task_BlinkLEDIndicator,
										"Task2 - MCULED",
										TASK_STACKSIZE_MIN,
										NULL,
										TASK_PRIO_MCULED,
										&h_TaskMcuLED);

	/* Ensure task creation succeeds */
	assert_param(TaskCreationStatus == pdPASS);

	/* Create task that will continuously check for pending BLE events */
	TaskCreationStatus = xTaskCreate( Task_ManageBLEEvents,
										"Task3 - BLE Events",
										TASK_STACKSIZE_DEFAULT,
										NULL,
										TASK_PRIO_BLE_EVENTS,
										&h_TaskBLEEvents);

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
	/* Variable declarations */
	uint32_t NotificationValue = 0;

	/* Initialize BLE Peripheral and place in advertising mode at startup to allow establishment
	 * of connections with BLE central devices in proximity.
	 */
	BlueNRG_Init();
	BlueNRG_MakeDeviceDiscoverable();

	while(1)
	{
		/* Block indefinitely until a notification to this task was obtained/received */
		NotificationValue = ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

		Task0_RSS = uxTaskGetStackHighWaterMark(NULL);

		if(NotificationValue & FRTOS_TASK_NOTIF_BLE_CONNECTED)
		{
			/* Resume or start the task that parses BLE messages */
			vTaskResume(h_TaskBLEMsg);
		}
		else if(NotificationValue & FRTOS_TASK_NOTIF_BLE_DISCONNECTED)
		{
			/* Suspend the task that parses BLE messages */
			vTaskSuspend(h_TaskBLEMsg);

			/* Place BLE module in advertising mode to allow new connections */
			BlueNRG_MakeDeviceDiscoverable();
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
static void Task_ParseBLEMessage(void *argument)
{
	/* Initialize BLE Peripheral and place in advertising mode at startup to allow establishment
	 * of connections with BLE central devices in proximity.
	 */

	while(1)
	{
		uint32_t NotificationValue = ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

		/* Check remaining stack size for this particular task */
		Task1_RSS = uxTaskGetStackHighWaterMark(NULL);

		if(NotificationValue & 0xFF)
		{

		}
	}

	/* Delete tasks automatically if somehow code reached this point */
	vTaskDelete(NULL);
}


static void Task_ManageBLEEvents(void *argument)
{
	/* Configure periodic BLE updates of 15ms */
	const TickType_t DelayFrequency = pdMS_TO_TICKS(15);
	TickType_t LastActiveTime;

	while(1)
	{
		/* Check amount of unused stack. If returned value is 0, stack overflow has occurred */
		Task3_RSS = uxTaskGetStackHighWaterMark(NULL);

		/* Perform accurate blocking delay */
		LastActiveTime = xTaskGetTickCount();
		vTaskDelayUntil(&LastActiveTime, DelayFrequency);

		/* Need to continuously call this function to process BLE events and connections */
		hci_user_evt_proc();
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
	/* Configure periodic delay */
	const TickType_t DelayFrequency = pdMS_TO_TICKS(1000);
	TickType_t LastActiveTime;

	while(1)
	{
		/* Check amount of unused stack. If returned value is 0, stack overflow has occurred */
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



