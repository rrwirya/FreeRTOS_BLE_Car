
/**
  **************************************************************************************************
  * @file           : motordriver_io.c
  * @brief          : This file contains HW IO level functions for interacting
  *                   with the Motor Driver Shield via PWM and GPIO
  * @author			: Reggie W
  **************************************************************************************************
  */


/* Includes --------------------------------------------------------------------------------------*/
#include "motordriver_io.h"


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
  * Section 1																				       *
  **************************************************************************************************
  */

/**
 * @brief
 * @param
 * @retval
 * @note
 */
void __MOTOR_ConfigureSpeed(E_MotorWheel_Pos MotorWheel, uint8_t Percentage)
{
	/* The percentage duty cycle is represented by: ((TIMx->CCRy)/(TIMx_period + 1)) * 100 */
	uint16_t CCRvalue = 0;

	/* Determine CCR value */
	if(Percentage >= MAX_PERCENTAGE)
		CCRvalue = TIM_PWM_MAX_CCR_VALUE;
	else
		CCRvalue = Percentage * 10;

	/* Modify respective CCR register */
	switch(MotorWheel)
	{
		case MOTWHEEL_REARLEFT:
		{
			TIM3->CCR1 = CCRvalue;
			break;
		}
		case MOTWHEEL_REARRIGHT:
		{
			TIM3->CCR2 = CCRvalue;
			break;
		}
		case MOTWHEEL_FRONTRIGHT:
		{
			TIM1->CCR3 = CCRvalue;
			break;
		}
		case MOTWHEEL_FRONTLEFT:
		{
			TIM1->CCR2 = CCRvalue;
			break;
		}
	}
}

/******************************************* END OF FILE *******************************************/

