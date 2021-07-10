/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif


/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include "stm32f4xx_hal.h"

#include "hci_tl_interface.h"


/* Private includes ----------------------------------------------------------*/


/* Exported types ------------------------------------------------------------*/


/* Exported constants --------------------------------------------------------*/


/* Exported macro ------------------------------------------------------------*/


/* Exported functions prototypes ---------------------------------------------*/
int _write(int file, char *ptr, int len);
void Error_Handler(void);


/* Private defines -----------------------------------------------------------*/
	/*--- STM32F411RE Nucleo-64 Board Pins ---*/
	#define NUCLEO_PB_Pin 					GPIO_PIN_13
	#define NUCLEO_PB_GPIO_Port 			GPIOC
	#define NUCLEO_PB_EXTI_IRQn 			EXTI15_10_IRQn
	#define NUCLEO_LED_Pin 					GPIO_PIN_5
	#define NUCLEO_LED_GPIO_Port 			GPIOA

	/*--- X-NUCLEO-BNRG2A1 SPI and IRQ Pins ---*/
	#define BLE_SCK_Pin 					GPIO_PIN_3
	#define BLE_SCK_GPIO_Port 				GPIOB
	#define BLE_MISO_Pin 					GPIO_PIN_6
	#define BLE_MISO_GPIO_Port 				GPIOA
	#define BLE_MOSI_Pin 					GPIO_PIN_7
	#define BLE_MOSI_GPIO_Port 				GPIOA
	#define BLE_CS_Pin 						GPIO_PIN_1
	#define BLE_CS_GPIO_Port 				GPIOA
	#define HCI_TL_RST_Pin 					GPIO_PIN_8
	#define HCI_TL_RST_GPIO_Port 			GPIOA
	#define HCI_TL_SPI_EXTI_Pin 			GPIO_PIN_0
	#define HCI_TL_SPI_EXTI_GPIO_Port 		GPIOA
	#define HCI_TL_SPI_EXTI_EXTI_IRQn 		EXTI0_IRQn

	/*--- PWM Pins (definitions below unused, only for user reference) ---*/
	#define PWM0A_GPIO_Port					GPIOA			/* PWM0A controls Front Right Motor Wheels */
	#define PWM0A_Pin						GPIO_PIN_9
	#define PWM0B_GPIO_Port					GPIOA			/* PWM0B controls Front Left Motor Wheels */
	#define PWM0B_Pin						GPIO_PIN_10
	#define PWM2A_GPIO_Port					GPIOB			/* PWM2A controls Rear Left Motor Wheels */
	#define PWM2A_Pin						GPIO_PIN_4
	#define PWM2B_GPIO_Port					GPIOC			/* PWM2B controls Rear Right Motor Wheels */
	#define PWM2B_Pin						GPIO_PIN_7

	/*--- ADXL343 HW Interface Pins ---*/
	#define ACCELEROMETER_INT1_GPIO_Port	GPIOC
	#define ACCELEROMETER_INT1_Pin			GPIO_PIN_4



#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
