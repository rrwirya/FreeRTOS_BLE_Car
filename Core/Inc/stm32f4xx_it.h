/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32f4xx_it.h
  * @brief   This file contains the headers of the interrupt handlers.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32F4xx_IT_H
#define __STM32F4xx_IT_H

#ifdef __cplusplus
	extern "C" {
#endif


/* Private includes ----------------------------------------------------------*/


/* Exported defines ----------------------------------------------------------*/
#define USE_MODIFIED_HARDFAULT_HANDLER						1


#define FRTOS_TASK_NOTIF_PB_PRESSED					((uint16_t)0x0001)
#define FRTOS_TASK_NOTIF_ADXL343_INT1				((uint16_t)0x0001)


/* Exported types ------------------------------------------------------------*/


/* Exported variables --------------------------------------------------------*/

#if USE_MODIFIED_HARDFAULT_HANDLER

	/*--- Variables to observe internal ARM CPU registers */
	extern unsigned long stacked_r0;
	extern unsigned long stacked_r1;
	extern unsigned long stacked_r2;
	extern unsigned long stacked_r3;
	extern unsigned long stacked_r12;
	extern unsigned long stacked_lr;
	extern unsigned long stacked_pc;
	extern unsigned long stacked_psr;
	extern unsigned long cfsr;
	extern unsigned long bus_fault_address;
	extern unsigned long memmanage_fault_address;

#endif


/* Exported constants --------------------------------------------------------*/


/* Exported macro ------------------------------------------------------------*/


/* Exported functions prototypes ---------------------------------------------*/
void NMI_Handler(void);
void HardFault_Handler(void);
void MemManage_Handler(void);
void BusFault_Handler(void);
void UsageFault_Handler(void);
void DebugMon_Handler(void);
void EXTI0_IRQHandler(void);
void TIM1_BRK_TIM9_IRQHandler(void);
void TIM2_IRQHandler(void);
void I2C1_EV_IRQHandler(void);
void EXTI15_10_IRQHandler(void);
void TIM5_IRQHandler(void);
void DMA2_Stream0_IRQHandler(void);



#ifdef __cplusplus
}
#endif

#endif /* __STM32F4xx_IT_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
