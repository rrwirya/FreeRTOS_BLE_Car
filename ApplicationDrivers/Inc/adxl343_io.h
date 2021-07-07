
/**
  ******************************************************************************
  * @file           : adxl343_io.h
  * @brief          : Header for adxl343_io.c file.
  * 				  Reset values of each registers contained here for
  * 				  initialization (all registers will be reset at each
  * 				  initialization).
  * @author			: Reggie W
  ******************************************************************************
  */


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ADXL343_IO_H
#define __ADXL343_IO_H


#ifdef __cplusplus
extern "C" {
#endif


/* Includes ------------------------------------------------------------------*/
#include "main.h"


/* Private includes ----------------------------------------------------------*/


/* Exported types ------------------------------------------------------------*/
	/*--- Boolean typedef for ADXL343 ---*/
	typedef enum
	{
		A_RESET = 0,
		A_SET = !A_RESET
	} AccelerometerFunctionalState;


/* Exported constants --------------------------------------------------------*/


/* Exported macro ------------------------------------------------------------*/


/* Private defines -----------------------------------------------------------*/
/* Reset values of registers 0x1D to 0x1F */
#define RST_REG_THRESH_TAP							((uint8_t)0x00)
#define RST_REG_OFSX								((uint8_t)0x00)
#define RST_REG_OFSY								((uint8_t)0x00)

/* Reset values of registers 0x20 to 0x2F */
#define RST_REG_OFSZ								((uint8_t)0x00)
#define RST_REG_DUR									((uint8_t)0x00)
#define RST_REG_LATENT								((uint8_t)0x00)
#define RST_REG_WINDOW								((uint8_t)0x00)
#define RST_REG_THRESH_ACT							((uint8_t)0x00)
#define RST_REG_THRESH_INACT						((uint8_t)0x00)
#define RST_REG_TIME_INACT							((uint8_t)0x00)
#define RST_REG_ACT_INACT_CTL						((uint8_t)0x00)
#define RST_REG_THRESH_FF							((uint8_t)0x00)
#define RST_REG_TIME_FF								((uint8_t)0x00)
#define RST_REG_TAP_AXES							((uint8_t)0x00)
#define RST_REG_ACT_TAP_STATUS						((uint8_t)0x00)
#define RST_REG_BW_RATE								((uint8_t)0x0A)			// Default: 100Hz Output Data Rate
#define RST_REG_POWER_CTL							((uint8_t)0x00)
#define RST_REG_INT_ENABLE							((uint8_t)0x00)
#define RST_REG_INT_MAP								((uint8_t)0x00)

/* Reset values of registers 0x30 to 0x39 */
#define RST_REG_INT_SOURCE							((uint8_t)0x02)			// Default: Watermark Interrupt
#define RST_REG_DATA_FORMAT							((uint8_t)0x00)
#define RST_REG_DATA_X0								((uint8_t)0x00)
#define RST_REG_DATA_X1								((uint8_t)0x00)
#define RST_REG_DATA_Y0								((uint8_t)0x00)
#define RST_REG_DATA_Y1								((uint8_t)0x00)
#define RST_REG_DATA_Z0								((uint8_t)0x00)
#define RST_REG_DATA_Z1								((uint8_t)0x00)
#define RST_REG_FIFO_CTL							((uint8_t)0x00)
#define RST_REG_FIFO_STATUS							((uint8_t)0x00)



/* Assume by default that the SDO/ALT ADDRESS pin is connected to GND */

	/**
	 * @note: The ADXL343 supports two different I2C addresses depending on
	 * 		  where the SDO pin is connected to. Check the datasheet for
	 * 		  further information on the two different I2C addresses.
	 */
#if !defined(ALT_ADDRESS_GND) && !defined(ALT_ADDRESS_VDD)
	#define ALT_ADDRESS_GND
#endif


/* Exported functions prototypes ---------------------------------------------*/
void AccelerometerBus_Init(void);
// void __io_accelerometer_i2cWrite(uint8_t* pTxBuff, uint16_t cTxLen);
// void __io_accelerometer_i2cRead(uint8_t* pRxBuff, uint16_t cRxLen);
void __io_accelerometer_i2cWriteRegister(uint8_t cRegAddress, uint8_t pData, uint8_t nRetransmissions);
uint8_t __io_accelerometer_i2cReadRegister(uint8_t cRegAddress, uint8_t nRetransmissions);


void __RESET_ADXL343_REGISTERS(void);


#ifdef __cplusplus 
}
#endif



#endif  /* __ADXL343_IO_H */


/********************************* END OF FILE *********************************/
