
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
 * Front Right Motor Wheel connected to Motor Driver Shield M3
 * Front Left Motor Wheel connected to Motor Driver Shield M4
 *
 * 		Motor Wheels PWM Speed Configurations:
 *
 * Rear Left Motor Wheel's speed controlled by PWM2A, connected to PB4 (TIM3_CH1)
 * Rear Right Motor Wheel's speed controlled by PWM2B, connected to PC7 (TIM3_CH2)
 * Front Right Motor Wheel's speed controlled by PWM0B, connected to PA9 (TIM1_CH3)
 * Front Left Motor Wheel's speed controlled by PWM0A, connected to PA10 (TIM1_CH2)
 *
 */

/* Enumerations for all the motor wheels in the car */
typedef enum
{
	MOTWHEEL_REARLEFT		= ((uint8_t)0x11),
	MOTWHEEL_REARRIGHT		= ((uint8_t)0x22),
	MOTWHEEL_FRONTRIGHT		= ((uint8_t)0x33),
	MOTWHEEL_FRONTLEFT		= ((uint8_t)0x44)
} E_MotorWheel_Pos;


/* Exported variables ----------------------------------------------------------------------------*/


/* Exported defines ------------------------------------------------------------------------------*/
	/*--- Timer PWM Parameters ---*/
	#define	TIM_PWM_PRESCALER_VALUE				9
	#define TIM_PWM_PERIOD_VALUE				999
	#define TIM_PWM_MAX_CCR_VALUE				(TIM_PWM_PERIOD_VALUE + 1)
	#define MAX_PERCENTAGE						100


/* Exported constants ----------------------------------------------------------------------------*/


/* Exported macro --------------------------------------------------------------------------------*/


/* Exported Functions Prototypes -----------------------------------------------------------------*/
void __MOTOR_ConfigureSpeed(E_MotorWheel_Pos MotorWheel, uint8_t Percentage);



#ifdef __cplusplus
}
#endif



#endif  /* __MOTORDRIVER_IO_H */


/******************************************* END OF FILE *******************************************/
