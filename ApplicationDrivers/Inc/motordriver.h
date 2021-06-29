
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
	DIR_CAR_BACK
} E_Dir_Car;


/* Exported variables ----------------------------------------------------------------------------*/


/* Exported defines ------------------------------------------------------------------------------*/
#define	MOT1_SELECTION_BITMASK			((uint8_t)0x0C)
#define MOT1_SELECTION_IN1_BITMASK		((uint8_t)0x04)
#define MOT1_SELECTION_IN2_BITMASK		((uint8_t)0x08)

#define ALT_MOT1_SELECTION_BITMASK		((uint8_t)0x30)
#define ALT_MOT1_SEL_IN1_BITMASK		((uint8_t)0x20)
#define ALT_MOT1_SEL_IN2_BITMASK		((uint8_t)0x10)

#define MOT2_SELECTION_BITMASK			((uint8_t)0x00)


/* Exported constants ----------------------------------------------------------------------------*/


/* Exported macro --------------------------------------------------------------------------------*/


/* Exported Functions Prototypes -----------------------------------------------------------------*/
void __TESTMOTOR_MoveWheel(E_MotorWheel_Pos MotorWheel, E_Dir_SingleWheel WheelDirection);



#ifdef __cplusplus
}
#endif



#endif  /* __MOTORDRIVER_H */


/******************************************* END OF FILE *******************************************/

