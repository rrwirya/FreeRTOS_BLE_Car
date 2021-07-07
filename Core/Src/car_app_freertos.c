
/**
  **************************************************************************************************
  * @file           : car_app_freertos.c
  * @brief          : This file contains all the FreeRTOS tasks running and controlling the STM32F411RE
  *  				  and other features pertaining the car. Also contains all FreeRTOS object
  *  				  initializations and Timer routines.
  * @author			: Reggie W
  **************************************************************************************************
  */


/* Includes --------------------------------------------------------------------------------------*/
#include "stm32f4xx_it.h"			/* Including only for FreeRTOS Task Notification Bitmask */
#include "car_app_freertos.h"
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "hci.h"
#include "hci_tl.h"


/* Private includes ------------------------------------------------------------------------------*/
#include "car_app_ble.h"
#include "motordriver.h"


/* Private typedef -------------------------------------------------------------------------------*/


/* Private define --------------------------------------------------------------------------------*/


/* Private macro ---------------------------------------------------------------------------------*/


/* Exported/Global variables ---------------------------------------------------------------------*/
	/*--- Variables related to Received BLE Message Task ---*/
	__IO uint32_t g_CountDirForward = 0;
	__IO uint32_t g_CountDirLeft = 0;
	__IO uint32_t g_CountDirRight = 0;
	__IO uint32_t g_CountDirBack = 0;
	__IO uint32_t g_CountDirForceStop = 0;

	/*--- Variables to record remaining stack size of each tasks ---*/
	UBaseType_t g_Task0_RSS, g_Task1_RSS, g_Task2_RSS, g_Task3_RSS, g_Task4_RSS;


/* External variables ----------------------------------------------------------------------------*/


/* Private variables -----------------------------------------------------------------------------*/
	/*--- FreeRTOS Timer Handles ---*/
	TimerHandle_t h_TimMotorTimeout;
	TimerHandle_t h_TimUpdateLED;

	/*--- FreeRTOS Task Handles ---*/
	TaskHandle_t h_TaskBLEConn;
	TaskHandle_t h_TaskBLEMsg;
	TaskHandle_t h_TaskPBProcessing;
	static TaskHandle_t sh_TaskMcuLED;
	static TaskHandle_t sh_TaskBLEEvents;
	static TaskHandle_t sh_TaskCarCalculations;
	static TaskHandle_t sh_TaskI2CEvents;

	/*--- Private variables related to BLE Connection Task ---*/


	/*--- Private variables related to Received BLE Message Task ---*/





/* Private function prototypes -------------------------------------------------------------------*/
	/* Task routines */
	static void Task_ManageBLEConnections(void *argument);
	static void Task_ParseBLEMessage(void *argument);
	static void Task_ProcessPushButtonIRQ(void *argument);
	static void Task_BlinkLEDIndicator(void *argument);
	static void Task_ManageBLEEvents(void *argument);
	static void Task_CarMovementCalculations(void *argument);
	static void Task_ManageI2CEvents(void *argument);

	/* FreeRTOS Timer Callback */
	static void vTimMotorTimeoutCallback(TimerHandle_t xTimer);
	static void vTimUpdateOledScreenCallback(TimerHandle_t xTimer);


/* Private user code -----------------------------------------------------------------------------*/

/**
  **************************************************************************************************
  * FreeRTOS Objects Initialization															       *
  **************************************************************************************************
  */

/**
 * @brief	Initializes all relevant FreeRTOS Mutexes
 */
void FRTOS_Init_Mutex(void)
{



}

/**
 * @brief	Initializes all relevant FreeRTOS Semaphores (binary/counting)
 */
void FRTOS_Init_Semaphores(void)
{



}

/**
 * @brief 	Initializes all relevant FreeRTOS Software Timers
 */
void FRTOS_Init_SWTimers(void)
{
	/* Create a timer that does not reload itself and triggers after 1 second */
	h_TimMotorTimeout = xTimerCreate("TIM_MotorTimeout",
										1000/portTICK_PERIOD_MS,
										pdFALSE,
										(void *)0,
										vTimMotorTimeoutCallback);

	/* Ensure SW Timer creation succeeds */
	assert_param(h_TimMotorTimeout != NULL);

	/* Create a timer that auto-reloads itself every 300ms */
	h_TimUpdateLED = xTimerCreate("TIM_UpdateOLEDScreen",
									300/portTICK_PERIOD_MS,
									pdTRUE,
									(void *)0,
									vTimUpdateOledScreenCallback);

	/* Ensure SW Timer creation succeeds */
	assert_param(h_TimUpdateLED != NULL);
}

/**
 * @brief	Initializes all relevant FreeRTOS Queues
 */
void FRTOS_Init_Queues(void)
{



}

/**
 * @brief	Initializes all relevant FreeRTOS Tasks
 */
void FRTOS_Init_Tasks(void)
{
	BaseType_t TaskCreationStatus;

	/* Create task that will maintain BLE Connection */
	TaskCreationStatus = xTaskCreate( Task_ManageBLEConnections,
										"Task0 - BLE Connection",
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

	/* Create task that will process push button interrupts */
	TaskCreationStatus = xTaskCreate( Task_ProcessPushButtonIRQ,
										"Task4 - PB",
										TASK_STACKSIZE_DEFAULT,
										NULL,
										TASK_PRIO_PB,
										&h_TaskPBProcessing);

	/* Ensure task creation succeeds */
	assert_param(TaskCreationStatus == pdPASS);

	/* Create task that will blink MCU LED periodically */
	TaskCreationStatus = xTaskCreate( Task_BlinkLEDIndicator,
										"Task2 - MCULED",
										TASK_STACKSIZE_MIN,
										NULL,
										TASK_PRIO_MCULED,
										&sh_TaskMcuLED);

	/* Ensure task creation succeeds */
	assert_param(TaskCreationStatus == pdPASS);

	/* Create task that will continuously check for pending BLE events */
	TaskCreationStatus = xTaskCreate( Task_ManageBLEEvents,
										"Task3 - BLE Events",
										TASK_STACKSIZE_DEFAULT,
										NULL,
										TASK_PRIO_BLE_EVENTS,
										&sh_TaskBLEEvents);

	/* Ensure task creation succeeds */
	assert_param(TaskCreationStatus == pdPASS);

	/*
	TaskCreationStatus = xTaskCreate( Task_CarMovementCalculations,
										"Task4 - MovementCalculations",
										TASK_STACKSIZE_DEFAULT,
										NULL,
										TASK_PRIO_CALCULATIONS,
										&sh_TaskCarCalculations);

	assert_param(TaskCreationStatus == pdPASS);

	TaskCreationStatus = xTaskCreate( Task_ManageI2CEvents,
										"Task5 - I2C Events",
										TASK_STACKSIZE_DEFAULT,
										NULL,
										TASK_PRIO_I2C_EVENTS,
										&sh_TaskI2CEvents);

	assert_param(TaskCreationStatus == pdPASS);

	*/
}



/**
  **************************************************************************************************
  * Main Application																		       *
  **************************************************************************************************
  */

/**
 * @brief	FreeRTOS Task responsible for maintaining connection with GAP Central devices. If BLE device
 *  		is not connected to any GAP Centrals, it will re-advertise itself.
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

		g_Task0_RSS = uxTaskGetStackHighWaterMark(NULL);

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
 * @brief	FreeRTOS Task responsible for performing particular sets of actions based on received
 * 			BLE messages/commands on the fourth characteristic
 * @note
 */
static void Task_ParseBLEMessage(void *argument)
{
	/* Variable declarations */
	uint32_t NotificationValue = 0;

	/* Initialize Motor */
	Motor_Init();

	while(1)
	{
		/* Block indefinitely until a notification to this task was obtained/received */
		NotificationValue = ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

		/* Check remaining stack size for this particular task */
		g_Task1_RSS = uxTaskGetStackHighWaterMark(NULL);

		if(NotificationValue & FRTOS_TASK_NOTIF_DIR_FORCESTOP)
		{
			/* Forcing car to stop moving is top priority */
			Car_ConfigDirection(DIR_CAR_BRAKES);
			g_CountDirForceStop++;
		}
		else if(NotificationValue & FRTOS_TASK_NOTIF_DIR_NORTH)
		{
			/* Move car in forward direction and begin timer that stops car in 1.5s */
			Car_ConfigDirection(DIR_CAR_FRONT);
			xTimerStart(h_TimMotorTimeout, 500/portTICK_PERIOD_MS);
			g_CountDirForward++;
		}
		else if(NotificationValue & FRTOS_TASK_NOTIF_DIR_EAST)
		{
			/* Move car in right direction and begin timer that stops car in 1s */
			Car_ConfigDirection(DIR_CAR_RIGHT);
			xTimerStart(h_TimMotorTimeout, 0);
			g_CountDirRight++;
		}
		else if(NotificationValue & FRTOS_TASK_NOTIF_DIR_SOUTH)
		{
			/* Move car in reverse direction and begin timer that stops car in 1.5s */
			Car_ConfigDirection(DIR_CAR_BACK);
			xTimerStart(h_TimMotorTimeout, 500/portTICK_PERIOD_MS);
			g_CountDirBack++;
		}
		else if(NotificationValue & FRTOS_TASK_NOTIF_DIR_WEST)
		{
			/* Move car in left direction and begin timer that stops car in 1s */
			Car_ConfigDirection(DIR_CAR_LEFT);
			xTimerStart(h_TimMotorTimeout, 0);
			g_CountDirLeft++;
		}
	}

	/* Delete tasks automatically if somehow code reached this point */
	vTaskDelete(NULL);
}

static void Task_ProcessPushButtonIRQ(void *argument)
{
	/* Variable declarations */
	__IO uint32_t PBCounter = 0;
	uint32_t NotificationValue = 0;

	while(1)
	{
		/* Block indefinitely until a notification to this task was obtained/received */
		NotificationValue = ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

		/* Check remaining stack size for this particular task */
		g_Task4_RSS = uxTaskGetStackHighWaterMark(NULL);

		if(NotificationValue & FRTOS_TASK_NOTIF_PB_PRESSED)
		{
			/* Test alternating each wheel to observe sequence */
			__TEST_MOTOR_AlternateWheel(PBCounter);
			PBCounter++;
		}
	}

	/* Delete tasks automatically if somehow code reached this point */
	vTaskDelete(NULL);
}

/**
 * @brief	FreeRTOS Task responsible for managing BLE events. hci_user_evt_proc() needs to be called
 * 			continuously to manage BLE connections and incoming/outgoing messages.
 * @note
 */
static void Task_ManageBLEEvents(void *argument)
{
	/* Configure periodic BLE updates of 15ms */
	const TickType_t DelayFrequency = pdMS_TO_TICKS(15);
	TickType_t LastActiveTime;

	while(1)
	{
		/* Check amount of unused stack. If returned value is 0, stack overflow has occurred */
		g_Task3_RSS = uxTaskGetStackHighWaterMark(NULL);

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
 * @brief	FreeRTOS Task responsible for blinking on-board microcontroller LED every second
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
		g_Task2_RSS = uxTaskGetStackHighWaterMark(NULL);

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
 * @brief	FreeRTOS Task responsible for measuring velocity and distance covered from retrieved acceleration
 * @note
 */
static void Task_CarMovementCalculations(void *argument)
{

	/* Delete tasks automatically if somehow code reached this point */
	vTaskDelete(NULL);
}

/**
 * @brief	FreeRTOS Task responsible for managing all I2C events/communication
 * @note
 */
static void Task_ManageI2CEvents(void *argument)
{

	/* Delete tasks automatically if somehow code reached this point */
	vTaskDelete(NULL);
}

/**
  **************************************************************************************************
  * Timer Callbacks																			       *
  **************************************************************************************************
  */

/**
 * @brief	FreeRTOS Timer that stops the car after 1.5 seconds of movement or 1 second of movement
 * 			depending on movement direction
 * @note
 */
static void vTimMotorTimeoutCallback(TimerHandle_t xTimer)
{
	/* Stops car movements */
	Car_ConfigDirection(DIR_CAR_BRAKES);
}

/**
 * @brief	FreeRTOS Timer that executes periodically to notify FreeRTOS task that it is
 * 			time to update OLED screen
 * @note
 */
static void vTimUpdateOledScreenCallback(TimerHandle_t xTimer)
{
	/* Fill value in <customqueue> Queue to notify <customtask> of pending actions */
}

/**
  **************************************************************************************************
  * BLE Processes																			       *
  **************************************************************************************************
  */



