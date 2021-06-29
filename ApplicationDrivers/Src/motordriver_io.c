
/**
  **************************************************************************************************
  * @file           : motordriver_io.c
  * @brief          : This file contains HW IO level functions for interacting
  *                   with the Motor Driver Shield via PWM and GPIO
  * @author			: Reggie W
  **************************************************************************************************
  */


/* Includes --------------------------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "motordriver_io.h"


/* Private includes ------------------------------------------------------------------------------*/


/* Private typedef -------------------------------------------------------------------------------*/


/* Private define --------------------------------------------------------------------------------*/


/* Private macro ---------------------------------------------------------------------------------*/


/* Exported/Global variables ---------------------------------------------------------------------*/
__IO uint8_t g_RecentShiftRegisterByte = 0x00;	/* Variable to keep track of recent value sent */


/* External variables ----------------------------------------------------------------------------*/


/* Private variables -----------------------------------------------------------------------------*/


/* Private function prototypes -------------------------------------------------------------------*/
static void __MOTOR_EnableShiftRegister(void);
static void __MOTOR_DisableShiftRegister(void);
static void __MOTOR_ShiftRegister_DelaySetup(void);
static void __MOTOR_ShiftRegister_DelayHold(void);
static void __MOTOR_ShiftRegister_Delay(void);
static void __MOTOR_SetShiftRegisterBit(FlagStatus BitStatus);


/* Private user code -----------------------------------------------------------------------------*/

/**
  **************************************************************************************************
  * Shift Register I/O related code															       *
  **************************************************************************************************
  */

/**
 * @brief	Enables the shift register by setting DIR_EN to LOW
 * @note
 */
static void __MOTOR_EnableShiftRegister(void)
{
	/* The input pin is negated, and by default, the Shift Register is disabled */
	HAL_GPIO_WritePin(DIR_EN_GPIO_Port, DIR_EN_Pin, GPIO_PIN_RESET);
}

/**
 * @brief	Disables the shift register by setting DIR_EN to HIGH
 * @note
 */
static void __MOTOR_DisableShiftRegister(void)
{
	/* The input pin is negated, and by default, the Shift Register is disabled */
	HAL_GPIO_WritePin(DIR_EN_GPIO_Port, DIR_EN_Pin, GPIO_PIN_SET);
}

/**
 * @brief	Approximately 7.4us delay
 */
static void __MOTOR_ShiftRegister_DelaySetup(void)
{
	for(volatile uint8_t i=0; i<0x3F; i++);
}

/**
 * @brief	Approximately 14.1us delay
 */
static void __MOTOR_ShiftRegister_DelayHold(void)
{
	for(volatile uint8_t i=0; i<0x7F; i++);
}

/**
 * @brief	Approximately 28.1us - 34us delay
 */
static void __MOTOR_ShiftRegister_Delay(void)
{
	for(volatile uint8_t i=0; i<0xFF; i++);
}

/**
 * @brief	Sets and configures individual bit in the shift register by toggling the DIR_SER and DIR_CLK pins
 * @param	BitStatus: whether bit that will be sent is SET or RESET
 * @retval 	None
 * @note	Bit must be SET (Bit=1) on DIR_SER before DIR_CLK gets driven high (rising edge observed on DIR_CLK).
 * 			Bit can then be RESET (Bit=0) before setting DIR_CLK=0 to apply data to the Shift Register.
 */
static void __MOTOR_SetShiftRegisterBit(FlagStatus BitStatus)
{
	if(BitStatus == SET)
	{
		/* Configure rising edge on DIR_SER */
		HAL_GPIO_WritePin(DIR_SER_GPIO_Port, DIR_SER_Pin, GPIO_PIN_SET);

		/* Set minimum set-up time delay between DIR_SER rising edge and DIR_CLK rising edge */
		__MOTOR_ShiftRegister_DelaySetup();

		/* Configure rising edge on DIR_CLK */
		HAL_GPIO_WritePin(DIR_CLK_GPIO_Port, DIR_CLK_Pin, GPIO_PIN_SET);

		/* Set minimum hold time for DIR_SER */
		__MOTOR_ShiftRegister_DelayHold();

		/* Configure falling edge on DIR_SER */
		HAL_GPIO_WritePin(DIR_SER_GPIO_Port, DIR_SER_Pin, GPIO_PIN_RESET);

		/* Set minimum fall-time delay between DIR_SER falling edge and DIR_CLK falling edge */
		__MOTOR_ShiftRegister_DelaySetup();

		/* Configure falling edge on DIR_CLK */
		HAL_GPIO_WritePin(DIR_CLK_GPIO_Port, DIR_CLK_Pin, GPIO_PIN_RESET);
	}
	else
	{
		/* Included to match CLK waveform when a HIGH is sent to the shift register */
		__MOTOR_ShiftRegister_DelaySetup();

		/* Configure rising edge on DIR_CLK */
		HAL_GPIO_WritePin(DIR_CLK_GPIO_Port, DIR_CLK_Pin, GPIO_PIN_SET);

		/* Set minimum hold time for DIR_CLK */
		__MOTOR_ShiftRegister_DelayHold();
		__MOTOR_ShiftRegister_DelaySetup();

		/* Configure falling edge on DIR_CLK */
		HAL_GPIO_WritePin(DIR_CLK_GPIO_Port, DIR_CLK_Pin, GPIO_PIN_RESET);
	}
}

/**
 * @brief	Configures the shift register bytes that will be used to set/reset output pins
 * @param	cByte: Byte to send (must be 8-bits)
 * @retval	None
 * @note	Last bit sent should represent MSbit, and first bit sent should represent LSbit
 */
void __MOTOR_SetShiftRegister(uint8_t cByte)
{
	/* Variable declarations and assignments */
	g_RecentShiftRegisterByte = cByte;
	uint8_t temp = cByte;

#if PRIORITIZE_SR_DATA_TRF
	taskENTER_CRITICAL();
#endif

	/* Re-enable shift register to toggle its output pins */
	__MOTOR_EnableShiftRegister();

	/* Iterate through all 8-bits */
	for(uint8_t i=0; i<8; i++)
	{
		/* Check the zeroth bit each iteration. Note that bits sent must be from least order to highest order */
		if(temp & 0x1)
		{
			/* Send a HI over DIR_SER */
			__MOTOR_SetShiftRegisterBit(SET);
		}
		else
		{
			/* Send a LO over DIR_SER */
			__MOTOR_SetShiftRegisterBit(RESET);
		}

		/* Parse the following bits and set a small delay in between bit transfers */
		temp = temp >> 1;
		__MOTOR_ShiftRegister_DelaySetup();
	}


#if PRIORITIZE_SR_DATA_TRF
	taskEXIT_CRITICAL();
#endif

	/* A pulse must be sent to DIR_LATCH to output byte configured in Shift Register to its output pins */
	HAL_GPIO_WritePin(DIR_LATCH_GPIO_Port, DIR_LATCH_Pin, GPIO_PIN_SET);
	__MOTOR_ShiftRegister_Delay();
	HAL_GPIO_WritePin(DIR_LATCH_GPIO_Port, DIR_LATCH_Pin, GPIO_PIN_RESET);
	__MOTOR_ShiftRegister_Delay();

	/* Disable shift register after use/configuration, to prevent spurious bits from being written to the device */
	__MOTOR_DisableShiftRegister();
}

/**
  **************************************************************************************************
  * PWM/Velocity related code																       *
  **************************************************************************************************
  */

/**
 * @brief	Configures speed of motor wheel by modifying PWM duty cycle values
 * @param	MotorWheel: The wheel to modify
 * 			Percentage: Percentage duty cycle (ranging from 0% to 100%)
 * @retval	None
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
			/* Update motor wheel speed by changing duty cycles */
			TIM3->CCR1 = CCRvalue;
			break;
		}
		case MOTWHEEL_REARRIGHT:
		{
			/* Update motor wheel speed by changing duty cycles */
			TIM3->CCR2 = CCRvalue;
			break;
		}
		case MOTWHEEL_FRONTRIGHT:
		{
			/* Update motor wheel speed by changing duty cycles */
			TIM1->CCR3 = CCRvalue;
			break;
		}
		case MOTWHEEL_FRONTLEFT:
		{
			/* Update motor wheel speed by changing duty cycles */
			TIM1->CCR2 = CCRvalue;
			break;
		}
	}
}

/******************************************* END OF FILE *******************************************/

