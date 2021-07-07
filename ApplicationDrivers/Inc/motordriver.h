
/**
  **************************************************************************************************
  * @file           : motordriver.h
  * @brief          : Header for motordriver.c file
  * @author         : Reggie W
  **************************************************************************************************
  */


/* Define to prevent recursive inclusion ---------------------------------------------------------*/
#ifndef __MOTORDRIVER_H
#define __MOTORDRIVER_H


#ifdef __cplusplus
extern "C" {
#endif


/* Includes --------------------------------------------------------------------------------------*/
#include "motordriver_io.h"


/* Exported types --------------------------------------------------------------------------------*/

/* Enumerations for all the directions one wheel can do, compared to pins of a L293D driver */
typedef enum
{
	DIR_WHEEL_OFF		= ((uint8_t)0x00),		/* IN1 low, IN2 low */
	DIR_WHEEL_BACKWARD 	= ((uint8_t)0x01),		/* IN1 low, IN2 high */
	DIR_WHEEL_FORWARD 	= ((uint8_t)0x10)		/* IN1 high, IN2 low */
} E_Dir_SingleWheel;

/* Enumerations for all the directions the car can move */
typedef enum
{
	DIR_CAR_LEFT,
	DIR_CAR_RIGHT,
	DIR_CAR_FRONT,
	DIR_CAR_BACK,
	DIR_CAR_BRAKES
} E_Dir_Car;

/* Enumerations for car speed configurations (4 PWM values represented currently) */
typedef enum
{
	SPEED_CAR_VERYFAST	= ((uint8_t)0x44),
	SPEED_CAR_FAST		= ((uint8_t)0x33),
	SPEED_CAR_NORMAL	= ((uint8_t)0x22),
	SPEED_CAR_SLOW		= ((uint8_t)0x11),
	SPEED_CAR_OFF		= ((uint8_t)0x00)
} E_Speed_Car;


/* Exported variables ----------------------------------------------------------------------------*/
extern __IO uint8_t g_ShiftRegisterByteToSet;


/* Exported defines ------------------------------------------------------------------------------*/
	/*--- Default speeds ---*/
	#define SPEED_CAR_VERYFAST_PERCENTAGE				100
	#define SPEED_CAR_FAST_PERCENTAGE					90
	#define SPEED_CAR_NORMAL_PERCENTAGE					80
	#define SPEED_CAR_SLOW_PERCENTAGE					65


/* Exported constants ----------------------------------------------------------------------------*/


/* Exported macro --------------------------------------------------------------------------------*/


/* Exported Functions Prototypes -----------------------------------------------------------------*/
	/*--- Basic/general motor functions ---*/
	void Motor_Init(void);
	void Motor_ApplyWheelChanges(void);

	/*--- Motor Direction related functions ---*/
	void Motor_ConfigWheelDirection(E_MotorWheel_Pos MotorWheel, E_Dir_SingleWheel WheelDirection);
	void Car_ConfigDirection(E_Dir_Car CarDirection);

	/*--- Motor Speed related functions ---*/
	void Car_ConfigSpeed(E_Speed_Car CarSpeed);

	/*--- Misc. functions ---*/
	void __TEST_MOTOR_AlternateWheel(uint32_t Counter);




#ifdef __cplusplus
}
#endif



#endif  /* __MOTORDRIVER_H */


/******************************************* END OF FILE *******************************************/

