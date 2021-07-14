
/**
  ******************************************************************************
  * @file       : adxl343_io.c
  * @brief      : This file contains HW IO level functions for interacting
  *               with the Analog Devices ADXL343 Accelerometer via I2C/SPI
  * @author		: Reggie W
  ******************************************************************************
  */


/* Includes ------------------------------------------------------------------*/
#include "i2c.h"						/* I2C1 handle located in this header file */
#include "adxl343_io.h"


/* Private includes ----------------------------------------------------------*/


/* Private typedef -----------------------------------------------------------*/


/* Private define ------------------------------------------------------------*/
#define N_ERROR_RETX								5				/* Allow up to 5 transmission in addition to the original failed transmission */
#define TOTALNUM_ADXL_REGISTERS						29				/* 29 registers available for use in the ADXL343 */
#define FIRST_ADXL_REGISTER_ADDR					((uint8_t)0x1D)

	/*--- Possible I2C addresses for the ADXL343 accelerometer ---*/
	#if defined(ALT_ADDRESS_VDD)
		/* 8-bit I2C Slave Write address, if ALT_ADDRESS pin (12) is connected to VDD */
		#define ACCELEROMETER_ADDRESS					((uint16_t)0x3A)
	#elif defined(ALT_ADDRESS_GND)
		/* 8-bit I2C Slave Write address, if ALT_ADDRESS pin (12) is connected to GND */
		#define ACCELEROMETER_ADDRESS					((uint16_t)0xA6)
	#endif


/* This will be used to look for all the I2C devices on the I2C1 bus */
#if !defined(I2C_ADDR_LOOP) && !defined(I2C_ADDR_CHECK)
	#define I2C_ADDR_LOOP
#endif


/* Private variables ---------------------------------------------------------*/
static const uint32_t i2cTimeout = 50;
static const uint16_t cRegisterSize = I2C_MEMADD_SIZE_8BIT;			/* registers' addresses are 8-bits wide */
static uint16_t cTotalAccelerometerRegisters = TOTALNUM_ADXL_REGISTERS;


/* Array containing first register to write to and the reset value of
   that first register and all registers onwards in ascending order
 */
static const uint8_t ResetValues[TOTALNUM_ADXL_REGISTERS + 1] = {
		FIRST_ADXL_REGISTER_ADDR, RST_REG_THRESH_TAP, RST_REG_OFSX, RST_REG_OFSY, \
		RST_REG_OFSZ, RST_REG_DUR, RST_REG_LATENT, \
		RST_REG_WINDOW, RST_REG_THRESH_ACT, RST_REG_THRESH_INACT, \
		RST_REG_TIME_INACT, RST_REG_ACT_INACT_CTL, RST_REG_THRESH_FF, \
		RST_REG_TIME_FF, RST_REG_TAP_AXES, RST_REG_ACT_TAP_STATUS, \
		RST_REG_BW_RATE, RST_REG_POWER_CTL, RST_REG_INT_ENABLE, \
		RST_REG_INT_MAP, RST_REG_INT_SOURCE, RST_REG_DATA_FORMAT, \
		RST_REG_DATA_X0, RST_REG_DATA_X1, RST_REG_DATA_Y0, \
		RST_REG_DATA_Y1, RST_REG_DATA_Z0, RST_REG_DATA_Z1, \
		RST_REG_FIFO_CTL, RST_REG_FIFO_STATUS};


/* Private macro -------------------------------------------------------------*/


/* Private function prototypes -----------------------------------------------*/
static uint16_t __I2C_AddressLoop(void);


/* Private user code ---------------------------------------------------------*/

/**
 * @brief	Loops through all possible I2C addresses for STM32 to receive ACK signal
 * @note	ACK condition will be observed through a Logic Analyzer
 * @retval	NumDevices: The number of devices that responded with an ACK
 */
static uint16_t __I2C_AddressLoop(void)
{
	/* This function will return the number of I2C devices on the bus */
	uint16_t NumDevices = 0;

#if defined(I2C_ADDR_LOOP)

	/* Iterate through all possible addresses */
	for(volatile uint16_t i=0; i<=127; i++)
	{
		/* 8-bit I2C address to check/probe */
		uint16_t l_Address = ((uint8_t)i << 1);

		/* Send nothing over I2C, this operation will see if an ACK is received */
		HAL_I2C_Master_Transmit(&hi2c1, l_Address, NULL, 0, i2cTimeout);

		/* Wait until I2C bus is ready/available */
		while(HAL_I2C_GetState(&hi2c1) != HAL_I2C_STATE_READY);

		/* Increment variable if an ACK was received */
		if(HAL_I2C_GetError(&hi2c1) != HAL_I2C_ERROR_AF)
			NumDevices++;
	}

#elif defined(I2C_ADDR_CHECK)

	/* Sends no data only to the accelerometer device to see if an ACK is received on the Bus */
	HAL_I2C_Master_Transmit(&hi2c1, ACCELEROMETER_ADDRESS, NULL, 0, i2cTimeout);

	/* Wait until I2C bus is ready/available */
	while(HAL_I2C_GetState(&hi2c1) != HAL_I2C_STATE_READY);

	/* Increment variable if an ACK was received */
	if(HAL_I2C_GetError(&hi2c1) != HAL_I2C_ERROR_AF)
		return 1;

#endif

	/* Return the number of devices that responded back with an ACK */
	return NumDevices;
}


/**
 * @brief	I2C bus initialization
 * @note  	I2C1 bus connected to the ADXL343 accelerometer
 */
void AccelerometerBus_Init(void)
{
	/* Relevant I2C1 parameters for the ADXL343 */
	hi2c1.Instance = I2C1;
	hi2c1.Init.OwnAddress1 = 192;
	hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	hi2c1.Init.OwnAddress2 = 0;
	hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;

	/* Initialize i2c peripheral and enable i2c1 bus clock */
	if (HAL_I2C_Init(&hi2c1) != HAL_OK)
	{
		Error_Handler();
	}
}


/**
 * @brief Writes data onto the ADXL343's internal register
 * @param      cRegAddress: Address of internal register to write into (8-bit internal address)
 *                   pData: 8-bit data to write
 *        nRetransmissions: Number of retransmissions to perform if a NACK occurs at each try
 */
void __io_accelerometer_i2cWriteRegister(uint8_t cRegAddress, uint8_t pData, uint8_t nRetransmissions)
{
	uint8_t pTxBuff[1] = {pData};

	/* timeout counter variable */
	volatile uint8_t i2c_current_retx = 0;
	volatile ErrorStatus i2c_process_status;

	do
	{
		i2c_process_status = SUCCESS;

		/* i2c single byte write operation */
		if(HAL_I2C_Mem_Write(&hi2c1, ACCELEROMETER_ADDRESS, cRegAddress, cRegisterSize, (uint8_t*)pTxBuff, 1, i2cTimeout) != HAL_OK)
		{
			Error_Handler();
		}

		/* Variable used as a counter */
		volatile uint32_t errorhandler_counter = 0;

		/* Wait until i2c bus is free/ready */
		while((HAL_I2C_GetState(&hi2c1) != HAL_I2C_STATE_READY) && (errorhandler_counter <= 15999))
		{
			errorhandler_counter++;
		}

		/* If I2C is BUSY even after 16000 CPU cycles */
		if((errorhandler_counter >= 16000) && (HAL_I2C_GetState(&hi2c1) != HAL_I2C_STATE_READY))
		{
			printf("I2C Bus still busy\n");
			Error_Handler();
		}

		/* Counter to ensure repeating transaction only happens up to nRetransmissions */
		i2c_current_retx++;

  /* When Acknowledge failure occurs (Slave did not acknowledge it's address)
     Master restarts communication up to nRetransmissions */
	}
	while(((HAL_I2C_GetError(&hi2c1) == HAL_I2C_ERROR_AF) && (i2c_current_retx <= nRetransmissions)) ||\
		((i2c_process_status == ERROR) && (i2c_current_retx <= N_ERROR_RETX)));

}


/**
 * @brief Reads data from the ADXL343's internal register
 * @param      cRegAddress: Address of internal register to read from (8-bit internal address)
 *        nRetransmissions: Number of retransmissions to perform if a NACK occurs at each try
 */
uint8_t __io_accelerometer_i2cReadRegister(uint8_t cRegAddress, uint8_t nRetransmissions)
{
	uint8_t pRxBuff[1] = {0x00};

	/* timeout counter variable */
	volatile uint8_t i2c_current_retx = 0;
	volatile ErrorStatus i2c_process_status;

	do
	{
		i2c_process_status = SUCCESS;

		/* i2c single byte read operation */
		if(HAL_I2C_Mem_Read(&hi2c1, ACCELEROMETER_ADDRESS, cRegAddress, cRegisterSize, (uint8_t*)pRxBuff, 1, i2cTimeout) != HAL_OK)
		{
			Error_Handler();
		}

		/* Variable used as a counter */
		volatile uint32_t errorhandler_counter = 0;

		/* Wait until i2c bus is free/ready */
		while((HAL_I2C_GetState(&hi2c1) != HAL_I2C_STATE_READY) && (errorhandler_counter <= 15999))
		{
			errorhandler_counter++;
		}

		/* If I2C is BUSY even after 16000 CPU cycles */
		if((errorhandler_counter >= 16000) && (HAL_I2C_GetState(&hi2c1) != HAL_I2C_STATE_READY))
		{
			printf("I2C Bus still busy\n");
			Error_Handler();
		}

		/* Counter to ensure repeating transaction only happens up to nRetransmissions */
		i2c_current_retx++;

		/* When Acknowledge failure occurs (Slave did not acknowledge it's address)
       Master restarts communication up to nRetransmissions */
	}
	while(((HAL_I2C_GetError(&hi2c1) == HAL_I2C_ERROR_AF) && (i2c_current_retx <= nRetransmissions)) ||\
		((i2c_process_status == ERROR) && (i2c_current_retx <= N_ERROR_RETX)));

	/* Return 8-bit value read from internal register */
	return pRxBuff[0];

}


/**
 * @brief 	Reads data from the ADXL343's internal register
 * @param   Pointer to variables that will hold raw 16-bit acceleration values
 * @note	To be used with reading Accelerometer FIFO/Data Registers
 */
void __ADXL_READMULTIBYTE_FIFO(uint16_t *DataX, uint16_t *DataY, uint16_t *DataZ)
{
	/* Variable declarations */
	uint8_t pRxBuff[6] = {0x00};		/* Store received bytes in this array/buffer */
	HAL_StatusTypeDef l_status;			/* Used to check if HAL operations were successful or not */
	uint8_t RxLen = 6;					/* Number of bytes to be received in I2C operation */

	/* Perform I2C Memory Read operation. 0x32 represents address of DATAX0 register. */
	HAL_I2C_Mem_Read(&hi2c1, ACCELEROMETER_ADDRESS, ((uint8_t)0x32), cRegisterSize, (uint8_t*)pRxBuff, RxLen, i2cTimeout);

	/* Ensure HAL terminated/executed successfully */
	assert_param(l_status == HAL_OK);

	/* Wait until I2C bus is ready, and check if an ACK was received or not afterwards */
	while(HAL_I2C_GetState(&hi2c1) != HAL_I2C_STATE_READY);
	assert_param(HAL_I2C_GetError(&hi2c1) != HAL_I2C_ERROR_AF);

	/* Assign passed input arguments the raw acceleration values for each axes */
	*DataX = (((uint16_t)pRxBuff[1] << 8) | pRxBuff[0]);
	*DataY = (((uint16_t)pRxBuff[3] << 8) | pRxBuff[2]);
	*DataZ = (((uint16_t)pRxBuff[5] << 8) | pRxBuff[4]);
}


/**
 * @brief Resets all the ADXL343 registers to reset values
 *        Check page 21 of the datasheet for its reset values
 */
void __RESET_ADXL343_REGISTERS(void){

	volatile ErrorStatus i2c_process_status;

	do
	{
		i2c_process_status = SUCCESS;

		/* Perform multiple byte write to reset all the registers */
		HAL_StatusTypeDef l_status;
		l_status = HAL_I2C_Master_Transmit(&hi2c1, ACCELEROMETER_ADDRESS, (uint8_t*)ResetValues, cTotalAccelerometerRegisters + 1, i2cTimeout);

		if(l_status != HAL_OK)
		{
			Error_Handler();
		}

		/* Will be used for a ~3.5ms counter */
		volatile uint32_t errorhandler_counter = 0;

		/* Wait for i2c bus to become available */
		while((HAL_I2C_GetState(&hi2c1) != HAL_I2C_STATE_READY) && (errorhandler_counter <= 55999))
		{
			errorhandler_counter++;
		}

		/* If I2C bus is still BUSY after 56000 CPU cycles */
		if((errorhandler_counter >= 56000) && (HAL_I2C_GetState(&hi2c1) != HAL_I2C_STATE_READY))
		{
			printf("I2C Bus still busy\n");
			Error_Handler();
		}

		/* Repeat transmission until an ACK signal is received */
	}
	while((HAL_I2C_GetError(&hi2c1) == HAL_I2C_ERROR_AF)||(i2c_process_status == ERROR));

}

/********************************* END OF FILE *********************************/

