
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
 * @brief	Function/routine to assign shift register values appropriate with each motor wheel movement
 * 			directions
 * @param	MotorWheel: The wheel to control/assign values
 * 			E_Dir_SingleWheel: The direction of the wheel (forward, backwards, off/disabled)
 * @note	SR output pin QA might represent LSbit, while QH might represent MSbit.
 * 			These functions have been properly calibrated/adjusted to incorrect hardware wirings.
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
		case MOTWHEEL_FRONTLEFT:
		{
			/* Front Left Wheel (M3) Selection and direction controlled by pins M3A and M3B */
			switch(WheelDirection)
			{
				case DIR_WHEEL_OFF:
				{
					/* Clear all bits related to this motor wheel */
					g_ShiftRegisterByteToSet &= ~MOT3_SELECTION_BITMASK;
					break;
				}
				case DIR_WHEEL_FORWARD:
				{
					/* Clear bits related to this motor and update its bits appropriately */
					g_ShiftRegisterByteToSet &= ~MOT3_SELECTION_BITMASK;
					g_ShiftRegisterByteToSet |= MOT3_SELECTION_IN1_BITMASK;
					break;
				}
				case DIR_WHEEL_BACKWARD:
				{
					/* Clear bits related to this motor and update its bits appropriately */
					g_ShiftRegisterByteToSet &= ~MOT3_SELECTION_BITMASK;
					g_ShiftRegisterByteToSet |= MOT3_SELECTION_IN2_BITMASK;
					break;
				}
			}

			break;
		}
		case MOTWHEEL_FRONTRIGHT:
		{
			/* Front Right Wheel (M4) Selection and direction controlled by pins M4A and M4B */
			switch(WheelDirection)
			{
				case DIR_WHEEL_OFF:
				{
					/* Clear all bits related to this motor wheel */
					g_ShiftRegisterByteToSet &= ~MOT4_SELECTION_BITMASK;
					break;
				}
				case DIR_WHEEL_FORWARD:
				{
					/* Clear bits related to this motor and update its bits appropriately */
					g_ShiftRegisterByteToSet &= ~MOT4_SELECTION_BITMASK;
					g_ShiftRegisterByteToSet |= MOT4_SELECTION_IN3_BITMASK;
					break;
				}
				case DIR_WHEEL_BACKWARD:
				{
					/* Clear bits related to this motor and update its bits appropriately */
					g_ShiftRegisterByteToSet &= ~MOT4_SELECTION_BITMASK;
					g_ShiftRegisterByteToSet |= MOT4_SELECTION_IN4_BITMASK;
					break;
				}
			}
			break;
		}
	}
}

/**
 * @brief	Controls the movement direction of the car
 * @param	CarDirection: Direction that car should go to
 * @note	Car moves based on wheel directions of each/every wheels
 */
void Car_ConfigDirection(E_Dir_Car CarDirection)
{
	switch(CarDirection)
	{
		case DIR_CAR_LEFT:
		{
			/* Move car left by moving both wheels on right side of the car */
			Motor_ConfigWheelDirection(MOTWHEEL_REARLEFT, DIR_WHEEL_OFF);
			Motor_ConfigWheelDirection(MOTWHEEL_REARRIGHT, DIR_WHEEL_FORWARD);
			Motor_ConfigWheelDirection(MOTWHEEL_FRONTLEFT, DIR_WHEEL_OFF);
			Motor_ConfigWheelDirection(MOTWHEEL_FRONTRIGHT, DIR_WHEEL_FORWARD);
			break;
		}
		case DIR_CAR_RIGHT:
		{
			/* Move car right by moving both wheels on left side of the car */
			Motor_ConfigWheelDirection(MOTWHEEL_REARLEFT, DIR_WHEEL_FORWARD);
			Motor_ConfigWheelDirection(MOTWHEEL_REARRIGHT, DIR_WHEEL_OFF);
			Motor_ConfigWheelDirection(MOTWHEEL_FRONTLEFT, DIR_WHEEL_FORWARD);
			Motor_ConfigWheelDirection(MOTWHEEL_FRONTRIGHT, DIR_WHEEL_OFF);
			break;
		}
		case DIR_CAR_FRONT:
		{
			/* Move car front by activating all wheels in forward direction */
			Motor_ConfigWheelDirection(MOTWHEEL_REARLEFT, DIR_WHEEL_FORWARD);
			Motor_ConfigWheelDirection(MOTWHEEL_REARRIGHT, DIR_WHEEL_FORWARD);
			Motor_ConfigWheelDirection(MOTWHEEL_FRONTLEFT, DIR_WHEEL_FORWARD);
			Motor_ConfigWheelDirection(MOTWHEEL_FRONTRIGHT, DIR_WHEEL_FORWARD);
			break;
		}
		case DIR_CAR_BACK:
		{
			/* Move car back/reverse by activating all wheels in backwards direction */
			Motor_ConfigWheelDirection(MOTWHEEL_REARLEFT, DIR_WHEEL_BACKWARD);
			Motor_ConfigWheelDirection(MOTWHEEL_REARRIGHT, DIR_WHEEL_BACKWARD);
			Motor_ConfigWheelDirection(MOTWHEEL_FRONTLEFT, DIR_WHEEL_BACKWARD);
			Motor_ConfigWheelDirection(MOTWHEEL_FRONTRIGHT, DIR_WHEEL_BACKWARD);
			break;
		}
		case DIR_CAR_BRAKES:
		{
			Motor_ConfigWheelDirection(MOTWHEEL_REARLEFT, DIR_WHEEL_OFF);
			Motor_ConfigWheelDirection(MOTWHEEL_REARRIGHT, DIR_WHEEL_OFF);
			Motor_ConfigWheelDirection(MOTWHEEL_FRONTLEFT, DIR_WHEEL_OFF);
			Motor_ConfigWheelDirection(MOTWHEEL_FRONTRIGHT, DIR_WHEEL_OFF);
			break;
		}
	}

	/* Apply Shift Register value changes to immediately apply motor effects */
	Motor_ApplyWheelChanges();
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

/**
 * @brief	Test code to check each wheel configuration and sequence
 */
void __TEST_MOTOR_AlternateWheel(uint32_t Counter)
{
	switch(Counter % 9)
	{
		case 0:
		{
			/* All wheels off */
			Motor_ConfigWheelDirection(MOTWHEEL_FRONTLEFT, DIR_WHEEL_OFF);
			Motor_ConfigWheelDirection(MOTWHEEL_FRONTRIGHT, DIR_WHEEL_OFF);
			Motor_ConfigWheelDirection(MOTWHEEL_REARLEFT, DIR_WHEEL_OFF);
			Motor_ConfigWheelDirection(MOTWHEEL_REARRIGHT, DIR_WHEEL_OFF);
			break;
		}
		case 1:
		{
			/* All wheels off except front left tires */
			Motor_ConfigWheelDirection(MOTWHEEL_FRONTLEFT, DIR_WHEEL_FORWARD);
			Motor_ConfigWheelDirection(MOTWHEEL_FRONTRIGHT, DIR_WHEEL_OFF);
			Motor_ConfigWheelDirection(MOTWHEEL_REARLEFT, DIR_WHEEL_OFF);
			Motor_ConfigWheelDirection(MOTWHEEL_REARRIGHT, DIR_WHEEL_OFF);
			break;
		}
		case 2:
		{
			/* All wheels off except front right tires */
			Motor_ConfigWheelDirection(MOTWHEEL_FRONTLEFT, DIR_WHEEL_OFF);
			Motor_ConfigWheelDirection(MOTWHEEL_FRONTRIGHT, DIR_WHEEL_FORWARD);
			Motor_ConfigWheelDirection(MOTWHEEL_REARLEFT, DIR_WHEEL_OFF);
			Motor_ConfigWheelDirection(MOTWHEEL_REARRIGHT, DIR_WHEEL_OFF);
			break;
		}
		case 3:
		{
			/* All wheels off except rear left tires */
			Motor_ConfigWheelDirection(MOTWHEEL_FRONTLEFT, DIR_WHEEL_OFF);
			Motor_ConfigWheelDirection(MOTWHEEL_FRONTRIGHT, DIR_WHEEL_OFF);
			Motor_ConfigWheelDirection(MOTWHEEL_REARLEFT, DIR_WHEEL_FORWARD);
			Motor_ConfigWheelDirection(MOTWHEEL_REARRIGHT, DIR_WHEEL_OFF);
			break;
		}
		case 4:
		{
			/* All wheels off except rear right tires */
			Motor_ConfigWheelDirection(MOTWHEEL_FRONTLEFT, DIR_WHEEL_OFF);
			Motor_ConfigWheelDirection(MOTWHEEL_FRONTRIGHT, DIR_WHEEL_OFF);
			Motor_ConfigWheelDirection(MOTWHEEL_REARLEFT, DIR_WHEEL_OFF);
			Motor_ConfigWheelDirection(MOTWHEEL_REARRIGHT, DIR_WHEEL_FORWARD);
			break;
		}
		case 5:
		{
			/* All wheels off except front left tires */
			Motor_ConfigWheelDirection(MOTWHEEL_FRONTLEFT, DIR_WHEEL_BACKWARD);
			Motor_ConfigWheelDirection(MOTWHEEL_FRONTRIGHT, DIR_WHEEL_OFF);
			Motor_ConfigWheelDirection(MOTWHEEL_REARLEFT, DIR_WHEEL_OFF);
			Motor_ConfigWheelDirection(MOTWHEEL_REARRIGHT, DIR_WHEEL_OFF);
			break;
		}
		case 6:
		{
			/* All wheels off except front right tires */
			Motor_ConfigWheelDirection(MOTWHEEL_FRONTLEFT, DIR_WHEEL_OFF);
			Motor_ConfigWheelDirection(MOTWHEEL_FRONTRIGHT, DIR_WHEEL_BACKWARD);
			Motor_ConfigWheelDirection(MOTWHEEL_REARLEFT, DIR_WHEEL_OFF);
			Motor_ConfigWheelDirection(MOTWHEEL_REARRIGHT, DIR_WHEEL_OFF);
			break;
		}
		case 7:
		{
			/* All wheels off except rear left tires */
			Motor_ConfigWheelDirection(MOTWHEEL_FRONTLEFT, DIR_WHEEL_OFF);
			Motor_ConfigWheelDirection(MOTWHEEL_FRONTRIGHT, DIR_WHEEL_OFF);
			Motor_ConfigWheelDirection(MOTWHEEL_REARLEFT, DIR_WHEEL_BACKWARD);
			Motor_ConfigWheelDirection(MOTWHEEL_REARRIGHT, DIR_WHEEL_OFF);
			break;
		}
		case 8:
		{
			/* All wheels off except rear right tires */
			Motor_ConfigWheelDirection(MOTWHEEL_FRONTLEFT, DIR_WHEEL_OFF);
			Motor_ConfigWheelDirection(MOTWHEEL_FRONTRIGHT, DIR_WHEEL_OFF);
			Motor_ConfigWheelDirection(MOTWHEEL_REARLEFT, DIR_WHEEL_OFF);
			Motor_ConfigWheelDirection(MOTWHEEL_REARRIGHT, DIR_WHEEL_BACKWARD);
			break;
		}
	}

	/* Apply Shift Register value changes to immediately apply motor effects */
	Motor_ApplyWheelChanges();
}


/******************************************* END OF FILE *******************************************/

