
/**
  **************************************************************************************************
  * @file           : motordriver_io.h
  * @brief          : Header for motordriver_io.c file.
  * @author         : Reggie W
  **************************************************************************************************
  */


/* Define to prevent recursive inclusion ---------------------------------------------------------*/
#ifndef __MOTORDRIVER_IO_H
#define __MOTORDRIVER_IO_H


#ifdef __cplusplus
extern "C" {
#endif


/* Includes --------------------------------------------------------------------------------------*/
#include "main.h"


/* Exported types --------------------------------------------------------------------------------*/

/**
 * 		Motor Wheels Positions/Configurations (Pinout):
 *
 * Rear Left Motor Wheel connected to Motor Driver Shield M1
 * Rear Right Motor Wheel connected to Motor Driver Shield M2
 * Front Left Motor Wheel connected to Motor Driver Shield M3
 * Front Right Motor Wheel connected to Motor Driver Shield M4
 *
 * 		Motor Wheels PWM Speed Configurations:
 *
 * Rear Left Motor Wheel's speed controlled by PWM2A, connected to PB4 (TIM3_CH1)
 * Rear Right Motor Wheel's speed controlled by PWM2B, connected to PC7 (TIM3_CH2)
 * Front Right Motor Wheel's speed controlled by PWM0A, connected to PA9 (TIM1_CH2)
 * Front Left Motor Wheel's speed controlled by PWM0B, connected to PA10 (TIM1_CH3)
 *
 */

/* Enumerations for all the motor wheels in the car */
typedef enum
{
	MOTWHEEL_REARLEFT		= ((uint8_t)0x11),
	MOTWHEEL_REARRIGHT		= ((uint8_t)0x22),
	MOTWHEEL_FRONTRIGHT		= ((uint8_t)0x33),
	MOTWHEEL_FRONTLEFT		= ((uint8_t)0x44),
	MOTWHEEL_FRONTTIRES		= ((uint8_t)0x55),
	MOTWHEEL_REARTIRES		= ((uint8_t)0x66)
} E_MotorWheel_Pos;


/* Exported variables ----------------------------------------------------------------------------*/
extern __IO uint8_t g_RecentShiftRegisterByte;


/* Exported defines ------------------------------------------------------------------------------*/
	#define	ENABLE_SPEED_CONTROL						0
	#define WHEEL_SPEED_DEFAULT_PERCENTAGE				100
	#define WHEEL_SPEED_DEFAULT_STARTING_PERCENTAGE		0

	/*--- Timer PWM Parameters ---*/
	#define	TIM_PWM_PRESCALER_VALUE				9
	#define TIM_PWM_PERIOD_VALUE				999
	#define TIM_PWM_MAX_CCR_VALUE				(TIM_PWM_PERIOD_VALUE + 1)
	#define MAX_PERCENTAGE						100

	/*--- Shift Register Pins ---*/
	/* Reference: https://lastminuteengineers.com/74hc595-shift-register-arduino-tutorial/ */
	#define DIR_LATCH_Pin						GPIO_PIN_6
	#define DIR_LATCH_GPIO_Port					GPIOB
	#define DIR_SER_Pin							GPIO_PIN_10
	#define DIR_SER_GPIO_Port					GPIOB
	#define DIR_EN_Pin							GPIO_PIN_5
	#define DIR_EN_GPIO_Port					GPIOB
	#define DIR_CLK_Pin							GPIO_PIN_0
	#define DIR_CLK_GPIO_Port					GPIOC

	/**
	 * @brief
	 *
	 * Motor2 (Motor Wheel Rear Right) is controlled by M2A (0x40 bitmask) and M2B (0x08 bitmask)
	 *
	 * M2B is IN3 (L293D) -> IN4/IN1 high while IN3/IN2 low yields forward direction
	 * M2A is IN4 (L293D) -> IN3/IN2 high while IN4/IN1 low yields backward direction
	 */
	#define	MOT2_SELECTION_BITMASK			((uint8_t)0x48)
	#define MOT2_SELECTION_IN3_BITMASK		((uint8_t)0x40)
	#define MOT2_SELECTION_IN4_BITMASK		((uint8_t)0x08)

	/**
	 * @brief
	 *
	 * Motor1 (Motor Wheel Rear Left) is controlled by M1A (0x20 bitmask) and M1B (0x10 bitmask)
	 *
	 * M1A is IN1 (L293D) -> IN4/IN1 high while IN3/IN2 low yields forward direction
	 * M1B is IN2 (L293D) -> IN3/IN2 high while IN4/IN1 low yields backward direction
	 */
	#define	MOT1_SELECTION_BITMASK			((uint8_t)0x30)
	#define MOT1_SELECTION_IN1_BITMASK		((uint8_t)0x20)
	#define MOT1_SELECTION_IN2_BITMASK		((uint8_t)0x10)

	/**
	 * @brief
	 *
	 * Motor3 (Motor Wheel Front Left) is controlled by M3A (0x80 bitmask) and M3B (0x02 bitmask)
	 *
	 * M3A is IN1 (L293D) -> IN4/IN1 high while IN3/IN2 low yields forward direction
	 * M3B is IN2 (L293D) -> IN3/IN2 high while IN4/IN1 low yields backward direction
	 */
	#define	MOT3_SELECTION_BITMASK			((uint8_t)0x82)
	#define MOT3_SELECTION_IN1_BITMASK		((uint8_t)0x80)
	#define MOT3_SELECTION_IN2_BITMASK		((uint8_t)0x02)

	/**
	 * @brief
	 *
	 * Motor4 (Motor Wheel Front Right) is controlled by M4A (0x04 bitmask) and M4B (0x01 bitmask)
	 *
	 * M4B is IN3 (L293D) -> IN4/IN1 high while IN3/IN2 low yields forward direction
	 * M4A is IN4 (L293D) -> IN3/IN2 high while IN4/IN1 low yields backward direction
	 *
	 * @warning	The pinout configuration (in physical HW) might be mixed up and direction
	 * 			might be reversed
	 */
	#define MOT4_SELECTION_BITMASK			((uint8_t)0x05)
	#define MOT4_SELECTION_IN3_BITMASK		((uint8_t)0x01)
	#define MOT4_SELECTION_IN4_BITMASK		((uint8_t)0x04)


/* Exported constants ----------------------------------------------------------------------------*/


/* Exported macro --------------------------------------------------------------------------------*/


/* Exported Functions Prototypes -----------------------------------------------------------------*/
void __MOTOR_HWInit(void);
void __MOTOR_SetShiftRegister(uint8_t cByte);
void __MOTOR_ConfigureSpeed(E_MotorWheel_Pos MotorWheel, uint8_t Percentage);
void __MOTOR_ConfigureAllWheelSpeed(uint8_t Percentage);


#ifdef __cplusplus
}
#endif



#endif  /* __MOTORDRIVER_IO_H */


/******************************************* END OF FILE *******************************************/

