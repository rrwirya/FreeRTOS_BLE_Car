
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
extern __IO uint8_t g_ShiftRegisterByteToSet;


/* Exported defines ------------------------------------------------------------------------------*/

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
 * M1A is IN1 (L293D) -> IN3/IN1 high while IN4/IN2 low yields forward direction
 * M1B is IN4 (L293D) -> IN4/IN2 high while IN3/IN1 low yields backward direction
 */
#define	MOT1_SELECTION_BITMASK			((uint8_t)0x30)
#define MOT1_SELECTION_IN1_BITMASK		((uint8_t)0x20)
#define MOT1_SELECTION_IN2_BITMASK		((uint8_t)0x10)


/* Exported constants ----------------------------------------------------------------------------*/


/* Exported macro --------------------------------------------------------------------------------*/


/* Exported Functions Prototypes -----------------------------------------------------------------*/
void Motor_Init(void);
void Motor_ApplyWheelChanges(void);

void Motor_ConfigWheelDirection(E_MotorWheel_Pos MotorWheel, E_Dir_SingleWheel WheelDirection);



#ifdef __cplusplus
}
#endif



#endif  /* __MOTORDRIVER_H */


/******************************************* END OF FILE *******************************************/

