
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
__IO uint8_t g_ShiftRegisterByteToSet = 0x00;


/* External variables ----------------------------------------------------------------------------*/


/* Private variables -----------------------------------------------------------------------------*/


/* Private function prototypes -------------------------------------------------------------------*/


/* Private user code -----------------------------------------------------------------------------*/

/**
 * @brief	Initialize Motor
 * @note
 */
void Motor_Init(void)
{
	/* Initialize hardware layer (motor shield driver) */
	__MOTOR_HWInit();
}

/**
 * @brief	Updates shift register with new wheel directions and selections
 * @note	Update g_ShiftRegisterByteToSet value through Motor_ConfigWheelDirection() prior to calling
 * 			this function
 */
void Motor_ApplyWheelChanges(void)
{
	__MOTOR_SetShiftRegister(g_ShiftRegisterByteToSet);
}

/**
  **************************************************************************************************
  * Motor Wheel movement/selection related code												       *
  **************************************************************************************************
  */

/**
 * @brief	Test function to move motor wheels
 * @param
 * @note	SR output pin QA might represent LSbit, while QH might represent MSbit
 */
void Motor_ConfigWheelDirection(E_MotorWheel_Pos MotorWheel, E_Dir_SingleWheel WheelDirection)
{
	switch(MotorWheel)
	{
		case MOTWHEEL_REARRIGHT:
		{
			/* Rear Right Wheel (M2) Selection and direction controlled by pins M2A and M2B */
			switch(WheelDirection)
			{
				case DIR_WHEEL_OFF:
				{
					/* Clear all bits related to this motor wheel */
					g_ShiftRegisterByteToSet &= ~MOT2_SELECTION_BITMASK;
					break;
				}
				case DIR_WHEEL_FORWARD:
				{
					/* Clear bits related to this motor and update its bits appropriately */
					g_ShiftRegisterByteToSet &= ~MOT2_SELECTION_BITMASK;
					g_ShiftRegisterByteToSet |= MOT2_SELECTION_IN4_BITMASK;
					break;
				}
				case DIR_WHEEL_BACKWARD:
				{
					/* Clear bits related to this motor and update its bits appropriately */
					g_ShiftRegisterByteToSet &= ~MOT2_SELECTION_BITMASK;
					g_ShiftRegisterByteToSet |= MOT2_SELECTION_IN3_BITMASK;
					break;
				}
			}

			break;
		}
		case MOTWHEEL_REARLEFT:
		{
			/* Rear Left Wheel (M1) Selection and direction controlled by pins M1A and M1B */
			switch(WheelDirection)
			{
				case DIR_WHEEL_OFF:
				{
					/* Clear all bits related to this motor wheel */
					g_ShiftRegisterByteToSet &= ~MOT1_SELECTION_BITMASK;
					break;
				}
				case DIR_WHEEL_FORWARD:
				{
					/* Clear bits related to this motor and update its bits appropriately */
					g_ShiftRegisterByteToSet &= ~MOT1_SELECTION_BITMASK;
					g_ShiftRegisterByteToSet |= MOT1_SELECTION_IN1_BITMASK;
					break;
				}
				case DIR_WHEEL_BACKWARD:
				{
					/* Clear bits related to this motor and update its bits appropriately */
					g_ShiftRegisterByteToSet &= ~MOT1_SELECTION_BITMASK;
					g_ShiftRegisterByteToSet |= MOT1_SELECTION_IN2_BITMASK;
					break;
				}
			}

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



/******************************************* END OF FILE *******************************************/

