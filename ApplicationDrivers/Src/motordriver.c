
/**
  **************************************************************************************************
  * @file           : <TEMPLATE_IO>.c
  * @brief          : This file contains HW IO level functions for interacting
  *                   with the <DEVICE AND PART NUMBER> via <HW PERIPHERALS>
  * @author			: Reggie W
  **************************************************************************************************
  */


/* Includes --------------------------------------------------------------------------------------*/
#include "motordriver.h"


/* Private includes ------------------------------------------------------------------------------*/


/* Private typedef -------------------------------------------------------------------------------*/


/* Private define --------------------------------------------------------------------------------*/


/* Private macro ---------------------------------------------------------------------------------*/


/* Exported/Global variables ---------------------------------------------------------------------*/


/* External variables ----------------------------------------------------------------------------*/


/* Private variables -----------------------------------------------------------------------------*/


/* Private function prototypes -------------------------------------------------------------------*/


/* Private user code -----------------------------------------------------------------------------*/

/**
  **************************************************************************************************
  * Motor Wheel movement/selection related code												       *
  **************************************************************************************************
  */

/**
 * @brief
 * @param
 * @retval
 * @note
 */


/**
  **************************************************************************************************
  * Motor Wheel Speed related code															       *
  **************************************************************************************************
  */

/**
 * @brief
 * @param
 * @retval
 * @note
 */


/**
  **************************************************************************************************
  * Test code																				       *
  **************************************************************************************************
  */

/**
 * @brief	Test function to move motor wheels
 * @param
 * @note	SR output pin QA might represent LSbit, while QH might represent MSbit
 */
void __TESTMOTOR_MoveWheel(E_MotorWheel_Pos MotorWheel, E_Dir_SingleWheel WheelDirection)
{
	uint8_t ShiftRegValue = 0;

	switch(MotorWheel)
	{
		case MOTWHEEL_REARLEFT:
		{
			/* Speed controlled by PWM2A */
			__MOTOR_ConfigureSpeed(MOTWHEEL_REARLEFT, 50);

			/* Selection and direction controlled by pins M1A and M1B */
			switch(WheelDirection)
			{
				case DIR_WHEEL_OFF:
				{
					ShiftRegValue = g_RecentShiftRegisterByte & ~ALT_MOT1_SELECTION_BITMASK;
					__MOTOR_SetShiftRegister(ShiftRegValue);
					break;
				}
				case DIR_WHEEL_BACKWARD:
				{
					ShiftRegValue = g_RecentShiftRegisterByte & ~ALT_MOT1_SELECTION_BITMASK;
					ShiftRegValue |= ALT_MOT1_SEL_IN2_BITMASK;
					__MOTOR_SetShiftRegister(ShiftRegValue);
					break;
				}
				case DIR_WHEEL_FORWARD:
				{
					ShiftRegValue = g_RecentShiftRegisterByte & ~ALT_MOT1_SELECTION_BITMASK;
					ShiftRegValue |= ALT_MOT1_SEL_IN1_BITMASK;
					__MOTOR_SetShiftRegister(ShiftRegValue);
					break;
				}
			}

			break;
		}
		case MOTWHEEL_REARRIGHT:
		{
			break;
		}
		case MOTWHEEL_FRONTRIGHT:
		{
			break;
		}
		case MOTWHEEL_FRONTLEFT:
		{
			break;
		}
		default: break;
	}
}

/******************************************* END OF FILE *******************************************/

