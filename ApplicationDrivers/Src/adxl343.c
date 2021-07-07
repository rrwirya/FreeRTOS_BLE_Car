/**
  ******************************************************************************
  * @file   : adxl343.c
  * @brief  : This file contains high level functions for initializing, using,
  *           and operating the ADXL343 accelerometer
  * @author	: Reggie W
  * @note   : Most bit toggling operations do not change contents
  *			  of registers aside from the toggled bit/bits. A change of
  *			  bit/bits to the registers uses 2 i2c operations: a Read
  *			  followed by a Write that updates the register.
  ******************************************************************************
  */
  
  
/* Includes ------------------------------------------------------------------*/
#include "adxl343.h"


/* Private includes ----------------------------------------------------------*/


/* Private typedef -----------------------------------------------------------*/


/* Private define ------------------------------------------------------------*/
/* Maximum i2c comms retransmissions */
#define NMAX_I2C_RETX													1
// Uncomment the line below to utilize SWO printf outputs 
// #define USE_VCOM


/* Private variables ---------------------------------------------------------*/
ActInactControlBits xActInactCtlValues = {
	ACT_AC_DC_EN,
	ACT_X_AXIS_EN,
	ACT_Y_AXIS_EN,
	ACT_Z_AXIS_EN,
	INACT_AC_DC_EN,
	INACT_X_AXIS_EN,
	INACT_Y_AXIS_EN,
	INACT_Z_AXIS_EN
};


/* Private macro -------------------------------------------------------------*/


/* Private function prototypes -----------------------------------------------*/

/******!< @group Power related functions ******/

/**
 * @brief Sets the measurement bit of the device to exit/enter standby mode
 * @note  Configure device prior to setting the measurement bit high (while
 *        device is still in STANDBY mode). Clearing the measurement bit
 *        will place device back in STANDBY mode.
 */
void Accelerometer_SetMeasurementMode(AccelerometerBitState xState)
{
	/* Read contents of POWER_CTL register before modifying */
	uint8_t temp = __io_accelerometer_i2cReadRegister(REG_POWER_CTL_BASE, NMAX_I2C_RETX);
	
	if(xState == A_ENABLE)
	{
		/* Set Measure bit high */
		temp |= MSK_POWER_CTL_MEASURE;
	}
	else if(xState == A_DISABLE)
	{
		/* Set Measure bit low */
		temp &= ~MSK_POWER_CTL_MEASURE;
	}
	
	/* Overwrite/update the POWER_CTL register */
	__io_accelerometer_i2cWriteRegister(REG_POWER_CTL_BASE, temp, NMAX_I2C_RETX);
	
}


/**
 * @brief Sets the low power bit for normal/low power operation
 * @note  At low power operation, the accelerometer will produce larger noise
 *        If using low power mode, set the Rate bits with Accelerometer_SetOutputDataRate()
 */
void Accelerometer_SetLowPowerMode(AccelerometerBitState xState)
{
	/* Read contents of BW_RATE register before modifying */
	uint8_t temp = __io_accelerometer_i2cReadRegister(REG_BW_RATE_BASE, NMAX_I2C_RETX);
	
	if(xState == A_ENABLE)
	{
		/* Set Low Power bit high indicating reduced power operation */
		temp |= MSK_REG_BW_LOWPOWER;
	}
	else if(xState == A_DISABLE)
	{
		/* Set Low Power bit low indicating normal operation */
		temp &= ~MSK_REG_BW_LOWPOWER;
	}
	
	/* Overwrite/update the BW_RATE register */
	__io_accelerometer_i2cWriteRegister(REG_BW_RATE_BASE, temp, NMAX_I2C_RETX);
	
}


/**
 * @brief	Sets the output data rate of the accelerometer
 * @param OutputDataRate_Hz: fill with one of the bit masks format
 *													 MSK_REG_BW_RATE_XXXX_HZ found under the REG_BW_RATE_BASE
 *													 register definition
 */
void Accelerometer_SetOutputDataRate(uint8_t cOutputDataRate_Hz)
{
	/* Read contents of BW_RATE register before modifying */
	uint8_t temp = __io_accelerometer_i2cReadRegister(REG_BW_RATE_BASE, NMAX_I2C_RETX);
	
	/* Clear/reset Output Data Rate field */
	temp &= MSK_REG_BW_LOWPOWER;
	
	/* Update 4 least significant bits with desired output data rate */
	temp |= cOutputDataRate_Hz;
	
	/* Overwrite/update the BW_RATE register */
	__io_accelerometer_i2cWriteRegister(REG_BW_RATE_BASE, temp, NMAX_I2C_RETX);
	
}


/**
 * @brief Enables the Auto Sleep Mode of the ADXL343
 * @param		cTimeInactivity: time representing time of inactivity before auto sleep in seconds
 *				cThreshInactivity: value representing inactivity event (gravity) in mg (not g)
 */
void Accelerometer_EnableAutoSleep(uint8_t cTimeInactivity, float cThreshmg)
{
	/* Configure inactivity conditions/requirements that will trigger Auto Sleep mode */
	Accelerometer_WriteInactivityThreshTimeMg(cTimeInactivity, cThreshmg);
	
	/* Read contents of POWER_CTL register before modifying */
	uint8_t temp = __io_accelerometer_i2cReadRegister(REG_POWER_CTL_BASE, NMAX_I2C_RETX);
	
	/* Enable Auto sleep bit */
	temp |= MSK_POWER_CTL_AUTOSLEEP;
	
	/* Enable link bit */
	temp |= MSK_POWER_CTL_LINK;
	
	/* Overwrite/update the POWER_CTL register */
	__io_accelerometer_i2cWriteRegister(REG_POWER_CTL_BASE, temp, NMAX_I2C_RETX);
	
}

/****** !< @group Interrupt related functions ******/

/**
 * @brief Sets Inactivity Threshold and Inactivity Time to trigger inactivity status
 * @param		cTimeInactivity: raw 8-bit value representing time of inactivity before auto sleep
 *				cThreshInactivity: raw 8-bit value representing inactivity event (gravity) 
 */
void Accelerometer_WriteInactivityThreshTime(uint8_t cTimeInactivity, uint8_t cThreshInactivity)
{
	/* Configure inactivity conditions/requirements for inactivity function */
	__io_accelerometer_i2cWriteRegister(REG_THRESH_INACT_BASE, cThreshInactivity, NMAX_I2C_RETX);
	__io_accelerometer_i2cWriteRegister(REG_TIME_INACT_BASE, cTimeInactivity, NMAX_I2C_RETX);
	
}


/**
 * @brief Sets the Activity Threshold in the THRESH_ACT register (addr: 0x24)
 */
void Accelerometer_WriteActivityThreshold(uint8_t cThreshActivity)
{
	/* Configure the activity threshold */
	__io_accelerometer_i2cWriteRegister(REG_THRESH_ACT_BASE, cThreshActivity, NMAX_I2C_RETX);
	
}


/**
 * @brief		Sets inactivity threshold and time to trigger inactivity status
 * @param		cTimeSeconds: time value in seconds (1sec/LSB for TIME_INACT register)
 *				     cThreshmg: gravity value in mg (not raw 8-bit value)
 */
void Accelerometer_WriteInactivityThreshTimeMg(uint8_t cTimeSeconds, float cThreshmg)
{
	/* Note that cTimeSeconds is already bounded with data type uint8_t */
	uint8_t cThresh;
	
	/* Convert threshold mg value to raw 8-bit hex value */
	if(cThreshmg >= 15937.5f)
		cThresh = 0xFF;
	else if(cThreshmg <= 0)
		cThresh = 0x00;
	else
	{
		/* Decimal divisions will be rounded down to 1 unsigned integer */
		cThresh = cThreshmg/62.5f;
	}
	
	/* Writes to the respective inactivity thresholds */
	Accelerometer_WriteInactivityThreshTime(cTimeSeconds, cThresh);
	
}


/**
 * @brief Sets the Activity Threshold. Input parameter is value in mg units (not g)
 */
void Accelerometer_WriteActivityThreshMg(float cThreshmg)
{
	/* Convert mg value to raw 8-bit hex value */
	uint8_t cThresh;
	
	/* Calculate the threshold */
	if(cThreshmg >= 15937.5f)
		cThresh = 0xFF;
	else if(cThreshmg <= 0)
		cThresh = 0x00;
	else
		cThresh = cThreshmg/62.5f;
	
	/* Writes to the respective activity thresholds register */
	Accelerometer_WriteActivityThreshold(cThresh);
	
}


/**
 * @brief Sets the Activity/Inactivity axis considerations and ac/dc control/comparison
 */
void Accelerometer_ConfigureActInactControl(ActInactControlBits xInput)
{
	/* Configure 8-bit configuration MSb first, starting from ACT ac/dc enable */
	uint8_t temp = 0x00;
	
	/* Set ACT ac/dc high if it is enabled */
	if(xInput.Activity_AcDcEnable == A_ENABLE)
		temp |= MSK_ACT_INACT_CTL_ACT_ACDC_EN;
	
	/* Set ACT_X high if it is enabled */
	if(xInput.Activity_XAxisEnable == A_ENABLE)
		temp |= MSK_ACT_INACT_CTL_ACT_X_EN;
	
	/* Set ACT_Y high if it is enabled */
	if(xInput.Activity_YAxisEnable == A_ENABLE)
		temp |= MSK_ACT_INACT_CTL_ACT_Y_EN;
	
	/* Set ACT_Z high if it is enabled */
	if(xInput.Activity_ZAxisEnable == A_ENABLE)
		temp |= MSK_ACT_INACT_CTL_ACT_Z_EN;
	
	/* Set INACT ac/dc high if it is enabled */
	if(xInput.Inactivity_AcDcEnable == A_ENABLE)
		temp |= MSK_ACT_INACT_CTL_INACT_ACDC_EN;
	
	/* Set INACT_X high if it is enabled */
	if(xInput.Inactivity_XAxisEnable == A_ENABLE)
		temp |= MSK_ACT_INACT_CTL_INACT_X_EN;
	
	/* Set INACT_Y high if it is enabled */
	if(xInput.Inactivity_YAxisEnable == A_ENABLE)
		temp |= MSK_ACT_INACT_CTL_INACT_Y_EN;

	/* Set INACT_Z high if it is enabled */
	if(xInput.Inactivity_ZAxisEnable == A_ENABLE)
		temp |= MSK_ACT_INACT_CTL_INACT_Z_EN;
	
	/* Update the ACT_INACT_CTL register with the new configuration */
	__io_accelerometer_i2cWriteRegister(REG_ACT_INACT_CTL_BASE, temp, NMAX_I2C_RETX);
	
}


/**
 * @brief Enables/Disables the link bit that links the activity function and 
 *				inactivity function together. If they both are linked, device will
 *				detect activity only after an inactivity is detected (while also
 *				disabling future detection of inactivity until an activity is
 *				detected), and vice versa.
 * @note	If the link bit is enabled, ensure that the inactivity functions are
 *				also enabled through Accelerometer_ConfigureActInactControl()
 */
void Accelerometer_LinkActivityInactivity(AccelerometerBitState xState)
{
	/* Read POWER_CTL register prior to changing/modifying its value */
	uint8_t temp = __io_accelerometer_i2cReadRegister(REG_POWER_CTL_BASE, NMAX_I2C_RETX);
	
	if(xState == A_ENABLE)
	{
		/* Set Link bit high to link activity and inactivity together */
		temp |= MSK_POWER_CTL_LINK;
	}
	else if(xState == A_DISABLE)
	{
		/* Set Link bit to low */
		temp &= ~MSK_POWER_CTL_LINK;
	}
	
	/* Update POWER_CTL register */
	__io_accelerometer_i2cWriteRegister(REG_POWER_CTL_BASE, temp, NMAX_I2C_RETX);
}


/**
 * @brief Enables or disables the ADXL343's registered interrupts
 * @param   xIrq: Interrupt type/category to enable/disable
 *				xState: A_ENABLE or A_DISABLE to respectively enable/disable the interrupt
 */
void Accelerometer_SetInterrupt(AccelerometerIrq xIrqPos, AccelerometerBitState xState)
{
	/* Read contents of INT_ENABLE before modifying */
	uint8_t temp = __io_accelerometer_i2cReadRegister(REG_INT_ENABLE_BASE, NMAX_I2C_RETX);
	
	if(xState == A_ENABLE)
	{
		/* Set respective irq bit high to enable interrupt */
		temp |= (MSK_INT_ENABLE_EN << xIrqPos);
	}
	else if(xState == A_DISABLE)
	{
		/* Set respective irq bit low to disable interrupt */
		temp &= ~(MSK_INT_ENABLE_EN << xIrqPos);
	}
	
	/* Overwrite/update the INT_ENABLE register */
	__io_accelerometer_i2cWriteRegister(REG_INT_ENABLE_BASE, temp, NMAX_I2C_RETX);
	
}


/**
 * @brief	Disables all accelerometer interrupts
 */
void Accelerometer_ResetInterrupt(void)
{
	/* Disable all interrupts by setting 0 on all bitfields of INT_ENABLE register */
	__io_accelerometer_i2cWriteRegister(REG_INT_ENABLE_BASE, 0x00, NMAX_I2C_RETX);
}


/**
 * @brief Maps interrupts to either INT1 pin or INT2 pin
 */
void Accelerometer_MapInterrupt(AccelerometerIrq xIrqPos, AcceleromterIrqPin xIrqPin)
{
	/* Read contents of INT_MAP before modifying */
	uint8_t temp = __io_accelerometer_i2cReadRegister(REG_INT_MAP_BASE, NMAX_I2C_RETX);
	
	if(xIrqPin == InterruptPin1)
	{
		/* Set respective bit to 0 to map to Interrupt pin 1 */
		temp &= ~(MSK_INT_MAP_INT2 << xIrqPos);
	}
	else if(xIrqPin == InterruptPin2)
	{
		/* Set respective bit to 1 to map to Interrupt pin 2 */
		temp |= MSK_INT_MAP_INT2 << xIrqPos;
	}
	
	/* Overwrite/update the INT_MAP register */
	__io_accelerometer_i2cWriteRegister(REG_INT_MAP_BASE, temp, NMAX_I2C_RETX);
	
}


/**
 * @brief Returns raw 8-bit value of all the interrupts that occurred
 * @note  This function can be used to clear interrupt flags 
 */
uint8_t Accelerometer_ReturnIrqFlags(void)
{
	/* Read INT_SOURCE register to clear all the interrupt flags */
	uint8_t retval = __io_accelerometer_i2cReadRegister(REG_INT_SOURCE_BASE, NMAX_I2C_RETX);
	
	return retval;
}


/**
 * @brief	Saves status of the accelerometer irq flags for further processing
 * @param *xIrqStatus: pointer to the struct object where the status of the
 *										 irq flags will be saved to. 
 * @note	This function also clears the irq flags by reading the INT_SOURCE register
 */
void Accelerometer_CheckInterruptFlags(AccelerometerIrqStatus *xIrqStatus)
{
	/* Read INT_SOURCE register to obtain status of interrupt flags */
	uint8_t temp = __io_accelerometer_i2cReadRegister(REG_INT_SOURCE_BASE, NMAX_I2C_RETX);
	
	/* Set respective bits/struct members to high if flags are SET */
	xIrqStatus->IRQ_OVERRUN = (temp & MSK_INT_SOURCE_OVERRUN) ? A_ENABLE : A_DISABLE;
	xIrqStatus->IRQ_WATERMARK = (temp & MSK_INT_SOURCE_WATERMARK) ? A_ENABLE : A_DISABLE;
	xIrqStatus->IRQ_FREE_FALL = (temp & MSK_INT_SOURCE_FREE_FALL) ? A_ENABLE : A_DISABLE;
	xIrqStatus->IRQ_INACTIVITY = (temp & MSK_INT_SOURCE_INACTIVITY) ? A_ENABLE : A_DISABLE;
	xIrqStatus->IRQ_ACTIVITY = (temp & MSK_INT_SOURCE_ACTIVITY) ? A_ENABLE : A_DISABLE;
	xIrqStatus->IRQ_DOUBLE_TAP = (temp & MSK_INT_SOURCE_DOUBLE_TAP) ? A_ENABLE : A_DISABLE;
	xIrqStatus->IRQ_SINGLE_TAP = (temp & MSK_INT_SOURCE_SINGLE_TAP) ? A_ENABLE : A_DISABLE;
	xIrqStatus->IRQ_DATA_READY = (temp & MSK_INT_SOURCE_DATA_READY) ? A_ENABLE : A_DISABLE;
	
}

/****** !< @group Data and Device configuration ******/

/**
 * @brief	Controls if the interrupt is active high or active low. Passing in an
 *				A_ENABLE value will configure the device to produce active high
 *				interrupts on the INTx pins. Default is active high.
 */
void ADXL343_InterruptActiveLow(AccelerometerBitState xState)
{
	/* Read DATA_FORMAT register prior to changing its values */
	uint8_t temp = __io_accelerometer_i2cReadRegister(REG_DATA_FORMAT_BASE, NMAX_I2C_RETX);
	
	if(xState == A_ENABLE)
	{
		/* value of 1 in INT_INVERT bit sets the interrupts to active low */
		temp |= MSK_DATA_FORMAT_INT_INVERT;
	}
	else if(xState == A_DISABLE)
	{
		temp &= ~MSK_DATA_FORMAT_INT_INVERT;
	}
	
	/* Update the DATA_FORMAT register */
	__io_accelerometer_i2cWriteRegister(REG_DATA_FORMAT_BASE, temp, NMAX_I2C_RETX);
	
}


/**
 * @brief Set the accelerometer to 3-wire SPI Mode or 4-wire SPI Mode
 */
void ADXL343_SPI3WireMode(AccelerometerBitState xState)
{
	/* Read DATA_FORMAT register prior to changing its values */
	uint8_t temp = __io_accelerometer_i2cReadRegister(REG_DATA_FORMAT_BASE, NMAX_I2C_RETX);
	
	if(xState == A_ENABLE)
	{
		/* value of 1 in SPI bit sets the device to 3-wire SPI mode */
		temp |= MSK_DATA_FORMAT_SPI;
	}
	else if(xState == A_DISABLE)
	{
		temp &= ~MSK_DATA_FORMAT_SPI;
	}
	
	/* Update the DATA_FORMAT register */
	__io_accelerometer_i2cWriteRegister(REG_DATA_FORMAT_BASE, temp, NMAX_I2C_RETX);
	
}


/**
 * @brief Configures the Buffer Mode in either Bypass mode, FIFO mode, Stream
 *				mode, or Trigger mode. Default value at startup is Bypass mode.
 */
void ADXL343_ConfigureFIFOMode(AccelerometerBufferStates xBufferMode)
{
	/* Read FIFO_CTL register prior to changing/modifying its values */
	uint8_t temp = __io_accelerometer_i2cReadRegister(REG_FIFO_CTL_BASE, NMAX_I2C_RETX);
	
	/* Clear FIFO_MODE = FIFO_CTL[7:5] field */
	temp &= 0x3F;
	
	/* Configure FIFO_MODE field with respect to given xBufferMode */
	if(xBufferMode == Buffer_Bypass)
	{
		temp |= MSK_FIFO_CTL_BUFFER_BYPASS;
	}
	else if(xBufferMode == Buffer_FIFO)
	{
		temp |= MSK_FIFO_CTL_BUFFER_FIFO;
	}
	else if(xBufferMode == Buffer_Stream)
	{
		temp |= MSK_FIFO_CTL_BUFFER_STREAM;
	}
	else if(xBufferMode == Buffer_Trigger)
	{
		temp |= MSK_FIFO_CTL_BUFFER_TRIGGER;
	}
	
	/* Overwrite/Update FIFO_CTL register with new buffer mode */
	__io_accelerometer_i2cWriteRegister(REG_FIFO_CTL_BASE, temp, NMAX_I2C_RETX);
	
}



/* ---------------------------- CHANGE ACCORDINGLY --------------------------- */

/**
 * @brief Initializes the ADXL343 in Standby Mode
 */
void ADXL343_Init(void)
{
	/* Place device in STANDBY mode for configuration */
	__RESET_ADXL343_REGISTERS();
	
	/* Check accelerometer device/serial ID */
	if(__io_accelerometer_i2cReadRegister(REG_DEVID_BASE, NMAX_I2C_RETX) == 0xE5)
	{
		/* printf statements took around 0.7 ms observed through Logic Analyzer */
		
#if defined(USE_VCOM)
		printf("Correct Accelerometer ID detected\n");
		printf("Accelerometer: ADXL343\n");
		printf("Device ID: 0xE5\n\n");
#endif
		
	}
	else
	{
		Error_Handler();
		
#if defined(USE_VCOM)
		printf("Wrong Accelerometer ID detected\n\n");
#endif
		
	}
	
	/* Configure output data rate of 25Hz */
	Accelerometer_SetOutputDataRate(MSK_REG_BW_RATE_25HZ);
	
	/* Enable low power run mode */
	Accelerometer_SetLowPowerMode(A_ENABLE);
	
	/* Clear all Irq Flags */
	__CLEAR_ADXL_IRQFLAGS();
	
	Accelerometer_LinkActivityInactivity(A_DISABLE);
	
	/* Enable relevant interrupts */
	Accelerometer_SetInterrupt(ACTIVITY, A_ENABLE);
	
	/*!< @testedparams
		130mg = 0.13g -> ok, need to be able to detect small muscle movements
										 in a period of 255 seconds if there is an ongoing
										 treatment but the doctor does not move a lot.
	  @note current/default range is +-2g, for less precision but higher
	        range change the range to +-4g and above
	*/
	Accelerometer_WriteActivityThreshMg(1300);
	
	/* Map ACTIVITY interrupt to INT2 pin */
	Accelerometer_MapInterrupt(ACTIVITY, InterruptPin2);
	
#if defined(ADXL343_LINK_ACTIVITY_INACTIVITY)

	/* Enable inactivity interrupts */
	Accelerometer_SetInterrupt(INACTIVITY, A_ENABLE);
	Accelerometer_WriteInactivityThreshTimeMg(10, 130);
	Accelerometer_MapInterrupt(INACTIVITY, InterruptPin2);
	
	/* Link activity and inactivity together */
	Accelerometer_LinkActivityInactivity(A_ENABLE);
	
#elif defined(ADXL343_ACTIVITY_INACTIVITY)

	/* Enable inactivity interrupts */
	Accelerometer_SetInterrupt(INACTIVITY, A_ENABLE);
	Accelerometer_WriteInactivityThreshTimeMg(INACTIVITY_PERIOD, 1300);
	Accelerometer_MapInterrupt(INACTIVITY, InterruptPin2);

#endif
	
	/* Configure Activity/Inactivity conditions/thresholds */
	Accelerometer_ConfigureActInactControl(xActInactCtlValues);
	
	/* Place device in non STANDBY mode */
	Accelerometer_SetMeasurementMode(A_ENABLE);
	
}


/********************************* END OF FILE *********************************/
