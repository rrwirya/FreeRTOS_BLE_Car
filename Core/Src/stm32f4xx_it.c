/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32f4xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include <stdlib.h>
#include "main.h"
#include "stm32f4xx_it.h"
#include "FreeRTOS.h"
#include "task.h"


/* Private includes ----------------------------------------------------------*/


/* Private typedef -----------------------------------------------------------*/


/* Private define ------------------------------------------------------------*/


/* Private macro -------------------------------------------------------------*/


/* Exported/Global variables -------------------------------------------------*/

#if USE_MODIFIED_HARDFAULT_HANDLER

	/*--- Variables to observe ARM CPU registers */
	unsigned long stacked_r0;
	unsigned long stacked_r1;
	unsigned long stacked_r2;
	unsigned long stacked_r3;
	unsigned long stacked_r12;
	unsigned long stacked_lr;
	unsigned long stacked_pc;
	unsigned long stacked_psr;
	unsigned long cfsr;
	unsigned long bus_fault_address;
	unsigned long memmanage_fault_address;

#endif


/* Private variables ---------------------------------------------------------*/


/* Private function prototypes -----------------------------------------------*/


/* Private user code ---------------------------------------------------------*/


/* External variables --------------------------------------------------------*/
	/*--- Microcontroller Peripheral Handles ---*/
	extern DMA_HandleTypeDef hdma_adc1;
	extern I2C_HandleTypeDef hi2c1;
	extern TIM_HandleTypeDef htim5;
	extern TIM_HandleTypeDef htim9;
	extern TIM_HandleTypeDef htim2;

	/*--- FreeRTOS Task Handles ---*/
	extern TaskHandle_t h_TaskPBProcessing;


/******************************************************************************/
/*           Cortex-M4 Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */
  while (1)
  {
  }
  /* USER CODE END NonMaskableInt_IRQn 1 */
}

#if USE_MODIFIED_HARDFAULT_HANDLER

/**
 * Code provided by Arm through: https://www.keil.com/appnotes/docs/apnt_209.asp
 * App Note 209 that has a detailed description of HardFault_Handler can be found in the link
 */

/* Private Function Prototype */
void HardFault_Handler_C(unsigned long * svc_args, unsigned int lr_value);


/**
 * HardFault handler wrapper in assembly language.
 * It extracts the location of stack frame and passes it to the handler written
 * in C as a pointer. We also extract the LR value as second parameter.
 */
void HardFault_Handler(void)
{
	asm(
		"TST    LR, #4\n\t"
		"ITE    EQ\n\t"
		"MRSEQ  R0, MSP\n\t"
		"MRSNE  R0, PSP\n\t"
		"MOV    R1, LR\n\t"
		"B      (HardFault_Handler_C)\n\t"
	);
}

/**
 * HardFault handler in C, with stack frame location and LR value extracted
 * from the assembly wrapper as input parameters
 */
void HardFault_Handler_C(unsigned long * hardfault_args, unsigned int lr_value)
{
	/* Relevant registers that display code/memory address that caused hardfault */
	bus_fault_address       = SCB->BFAR;
	memmanage_fault_address = SCB->MMFAR;
	cfsr                    = SCB->CFSR;

	/* Relevant ARM registers */
	stacked_r0  = ((unsigned long) hardfault_args[0]);
	stacked_r1  = ((unsigned long) hardfault_args[1]);
	stacked_r2  = ((unsigned long) hardfault_args[2]);
	stacked_r3  = ((unsigned long) hardfault_args[3]);
	stacked_r12 = ((unsigned long) hardfault_args[4]);
	stacked_lr  = ((unsigned long) hardfault_args[5]);
	stacked_pc  = ((unsigned long) hardfault_args[6]);
	stacked_psr = ((unsigned long) hardfault_args[7]);

	/* Need to enable ITM/SWD on CubeIDE and MDK-ARM to observer printf() statements */
	printf ("[HardFault]\n");
	printf ("- Stack frame:\n");
	printf (" R0  = %lx\n", stacked_r0);
	printf (" R1  = %lx\n", stacked_r1);
	printf (" R2  = %lx\n", stacked_r2);
	printf (" R3  = %lx\n", stacked_r3);
	printf (" R12 = %lx\n", stacked_r12);
	printf (" LR  = %lx\n", stacked_lr);
	printf (" PC  = %lx\n", stacked_pc);
	printf (" PSR = %lx\n", stacked_psr);
	printf ("- FSR/FAR:\n");
	printf (" CFSR = %lx\n", cfsr);
	printf (" HFSR = 0x%08X\n", (unsigned int)(SCB->HFSR));
	printf (" DFSR = 0x%08X\n", (unsigned int)(SCB->DFSR));
	printf (" AFSR = 0x%08X\n", (unsigned int)(SCB->AFSR));
	if (cfsr & 0x0080) printf (" MMFAR = %lx\n", memmanage_fault_address);
	if (cfsr & 0x8000) printf (" BFAR = %lx\n", bus_fault_address);
	printf ("- Misc\n");
	printf (" LR/EXC_RETURN= %x\n", lr_value);

	/* Infinite loop - halt program/software */
	while(1);
}

#else

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
}

#endif

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void)
{
  /* USER CODE BEGIN MemoryManagement_IRQn 0 */

  /* USER CODE END MemoryManagement_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
    /* USER CODE END W1_MemoryManagement_IRQn 0 */
  }
}

/**
  * @brief This function handles Pre-fetch fault, memory access fault.
  */
void BusFault_Handler(void)
{
  /* USER CODE BEGIN BusFault_IRQn 0 */

  /* USER CODE END BusFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_BusFault_IRQn 0 */
    /* USER CODE END W1_BusFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void)
{
  /* USER CODE BEGIN UsageFault_IRQn 0 */

  /* USER CODE END UsageFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_UsageFault_IRQn 0 */
    /* USER CODE END W1_UsageFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void)
{
  /* USER CODE BEGIN DebugMonitor_IRQn 0 */

  /* USER CODE END DebugMonitor_IRQn 0 */
  /* USER CODE BEGIN DebugMonitor_IRQn 1 */

  /* USER CODE END DebugMonitor_IRQn 1 */
}

/******************************************************************************/
/* STM32F4xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f4xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles EXTI line0 interrupt.
  */
void EXTI0_IRQHandler(void)
{
  HAL_EXTI_IRQHandler(&H_EXTI_0);
}

/**
  * @brief This function handles TIM1 break interrupt and TIM9 global interrupt.
  */
void TIM1_BRK_TIM9_IRQHandler(void)
{
  HAL_TIM_IRQHandler(&htim9);
}

/**
  * @brief This function handles TIM2 global interrupt.
  */
void TIM2_IRQHandler(void)
{
  HAL_TIM_IRQHandler(&htim2);
}

/**
  * @brief This function handles I2C1 event interrupt.
  */
void I2C1_EV_IRQHandler(void)
{
  HAL_I2C_EV_IRQHandler(&hi2c1);
}

/**
  * @brief This function handles EXTI line[15:10] interrupts.
  */
void EXTI15_10_IRQHandler(void)
{
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_13);
}

/**
  * @brief This function handles TIM5 global interrupt.
  */
void TIM5_IRQHandler(void)
{
  HAL_TIM_IRQHandler(&htim5);
}

/**
  * @brief This function handles DMA2 stream0 global interrupt.
  */
void DMA2_Stream0_IRQHandler(void)
{
  HAL_DMA_IRQHandler(&hdma_adc1);
}

/******************************************************************************/
/* STM32F4xx Peripheral Interrupt Callback Functions		                  */
/******************************************************************************/

/**
 * @brief  Period elapsed callback in non blocking mode
 * @note   This function is called  when TIM2 interrupt took place, inside
 * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
 * a global variable "uwTick" used as application time base.
 * @param  htim : TIM handle
 * @retval None
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == TIM2)
	{
		HAL_IncTick();
	}
	else if(htim->Instance == TIM5)
	{

	}
	else if(htim->Instance == TIM9)
	{

	}
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if(GPIO_Pin == NUCLEO_PB_Pin)
	{
		/* 10ms debounce button */
		HAL_Delay(10);

		/* This value becomes pdTRUE if giving the notification caused a task to unblock, and the unblocked task has a
		   higher priority than the currently running task, in which a context switch should occur */
		BaseType_t xHigherPriorityTaskWoken = pdFALSE;

		/* Notify task that manages BLE connections that a connection was successfully created */
		xTaskNotifyFromISR(h_TaskPBProcessing, FRTOS_TASK_NOTIF_PB_PRESSED, eSetBits, &xHigherPriorityTaskWoken);

		/* Force context switch if xHigherPriorityTaskWoken == pdTRUE. This does nothing if xHigherPriorityTaskWoken
		   is pdFALSE */
		portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
	}
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
