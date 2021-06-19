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
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define NUCLEO_PB_Pin GPIO_PIN_13
#define NUCLEO_PB_GPIO_Port GPIOC
#define NUCLEO_PB_EXTI_IRQn EXTI15_10_IRQn
#define HCI_TL_SPI_EXTI_Pin GPIO_PIN_0
#define HCI_TL_SPI_EXTI_GPIO_Port GPIOA
#define HCI_TL_SPI_EXTI_EXTI_IRQn EXTI0_IRQn
#define BLE_CS_Pin GPIO_PIN_1
#define BLE_CS_GPIO_Port GPIOA
#define NUCLEO_LED_Pin GPIO_PIN_5
#define NUCLEO_LED_GPIO_Port GPIOA
#define BLE_MISO_Pin GPIO_PIN_6
#define BLE_MISO_GPIO_Port GPIOA
#define BLE_MOSI_Pin GPIO_PIN_7
#define BLE_MOSI_GPIO_Port GPIOA
#define HCI_TL_RST_Pin GPIO_PIN_8
#define HCI_TL_RST_GPIO_Port GPIOA
#define BLE_SCK_Pin GPIO_PIN_3
#define BLE_SCK_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
