
/**
  ******************************************************************************
  * @file           : adxl343.h
  * @brief          : Header for adxl343.c file.
  *                   Registers, functions, and macros pertaining to the ADXL343
  *                   accelerometer are contained here. Reset values of all the
  *                   ADXL343 internal registers are located in the I/O file.
  *                   Some of the registers' descriptions are contained here,
  *                   although further information can be found in the
  *                   datasheet.
  * @author         : Reggie W
  ******************************************************************************
  */


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ADXL343_H
#define __ADXL343_H


#ifdef __cplusplus
extern "C" {
#endif


/* Includes ------------------------------------------------------------------*/
#include "adxl343_io.h"


/* Private includes ----------------------------------------------------------*/


/* Exported constants --------------------------------------------------------*/


/* Private defines -----------------------------------------------------------*/
#define ADXL343_ACTIVITY_INACTIVITY


/*-------------------------  Device Status/Settings --------------------------*/
	/*--- Activity/Inactivity event configuration ---*/
	#define ACT_AC_DC_EN								A_DISABLE
	#define ACT_X_AXIS_EN								A_ENABLE	// En/Dis x axis as a participating axes in activity function
	#define ACT_Y_AXIS_EN								A_ENABLE	// En/Dis y axis as a participating axes in activity function
	#define ACT_Z_AXIS_EN								A_ENABLE	// En/Dis z axis as a participating axes in activity function

#if defined(ADXL343_LINK_ACTIVITY_INACTIVITY) || defined(ADXL343_ACTIVITY_INACTIVITY)

	#define INACT_AC_DC_EN							A_DISABLE
	#define INACT_X_AXIS_EN							A_ENABLE	
	#define INACT_Y_AXIS_EN							A_ENABLE 	
	#define INACT_Z_AXIS_EN							A_ENABLE 

#else

	#define INACT_AC_DC_EN							A_DISABLE
	#define INACT_X_AXIS_EN							A_DISABLE	
	#define INACT_Y_AXIS_EN							A_DISABLE 	
	#define INACT_Z_AXIS_EN							A_DISABLE 	

#endif

	/* Trigger INACTIVITY_INTERRUPT if no activity is detected for <INACTIVITY_PERIOD> seconds */
	#define INACTIVITY_PERIOD							11


/*-------------------------  BEGIN ADXL343 REGISTERS  ------------------------*/
/***
 * @notation
 * REG_XXXX 						indicates internal hardware register
 * MSK_<REG_NAME>_XXXX 	indicates bit masks corresponding to <REG_NAME> register
 * BITPOS_XXXX					indicates bit positioning
 **/

#define REG_DEVID_BASE						((uint8_t)0x00)

	/** 
	 * @type Read only (R)
	 * @desc Device ID
	 * This register holds a fixed device ID code of 0xE5
	 */

#define REG_THRESH_TAP_BASE					((uint8_t)0x1D)			
#define REG_OFSX_BASE						((uint8_t)0x1E)
#define REG_OFSY_BASE						((uint8_t)0x1F)


#define REG_OFSZ_BASE						((uint8_t)0x20)
#define REG_DUR_BASE						((uint8_t)0x21)
#define REG_LATENT_BASE						((uint8_t)0x22)
#define REG_WINDOW_BASE						((uint8_t)0x23)
#define REG_THRESH_ACT_BASE					((uint8_t)0x24)			
#define REG_THRESH_INACT_BASE				((uint8_t)0x25)

	/**
	 * @type Read/Write (R/W)
	 * @desc Inactivity threshold
	 * The THRESH_INACT register is eight bits and holds the threshold
	 * value for detecting inactivity. The data format is unsigned,
	 * therefore, the magnitude of the inactivity event is compared with
	 * the value in the THRESH_INACT register. The scale factor is 62.5mg/LSB.
	 * A value of 0 may result in undesirable behavior if the inactivity
	 * interrupt is enabled.
	 */

#define REG_TIME_INACT_BASE					((uint8_t)0x26)			

	/**
	 * @type Read/Write (R/W)
	 * @desc Inactivity time
	 * The TIME_INACT register is eight bits and contains an unsigned time
	 * value representing the amount of time that acceleration must be less
	 * than the value in the THRESH_INACT register for inactivity to be 
	 * declared. The scale factor is 1sec/LSB. Unlike the other interrupt 
	 * functions, which use unfiltered data, the inactivity function uses
	 * filtered output data. At least one output sample must be generated
	 * for the inactivity interrupt to be triggered. This results in the
	 * function appearing nonresponsive if the TIME_INACT register is set to
	 * a value less than the time constant of the output data rate. A value
	 * of 0 results in an interrupt when the output data is less than the
	 * value in the THRESH_INACT register.
	 */

#define REG_ACT_INACT_CTL_BASE				((uint8_t)0x27)

	/**
	 * @type Read/Write (R/W)
	 * @desc Axis enable control for activity and inactivity detection
	 *
	 * groups:
	 *			ACT AC/DC and INACT AC/DC Bits
	 *					A setting of 0 selects dc-coupled operation, and a setting of
	 *					1 enables ac-coupled operation. In dc-coupled operation, the
	 *					current acceleration magnitude is compared directly with
	 *					THRESH_ACT and THRESH_INACT to determine whether activity or
	 *					inactivity is detected.
	 *
	 *					In ac-coupled operation for activity detection, the acceleration
	 *					value at the start of activity detection is taken as a reference
	 *					value. New samples of acceleration are then compared to this
	 *					reference value, and if the magnitude of the difference exceeds
	 *					the THRESH_ACT value, the device triggers an activity interrupt.
	 *
	 *					Similarly, in ac-coupled operation for inactivity detection,
	 *					a reference value is used for comparison and is updated whenever
	 *					the device exceeds the inactivity threshold. After the
	 *					reference value is selected, the device compares the magnitude
	 *					of the difference between the reference value and the current
	 *					acceleration with THRESH_INACT. If the difference is less than
	 *					the value in THRESH_INACT for the time in TIME_INACT, the
	 *					device is considered inactive and the inactivity interrupt
	 *					is triggered.
	 *
	 *			ACT_x Enable Bits and INACT_x Enable Bits
	 *					A setting of 1 enables x-, y-, or z-axis participation in
	 *					detecting activity or inactivity. A setting of 0 excludes the
	 *					selected axis from participation. If all axes are excluded, the
	 *					function is disabled. For activity detection, all participating
	 *					axes are logically OR'ed, causing the activity function to
	 *					trigger when any of the participating axes exceeds the threshold.
	 *					For inactivity detection, all participating axes are logically
	 *					AND'ed, causing the inactivity function to trigger only if all
	 *					participating axes are below the threshold for the specified
	 *					time. 
	 */
	 
	 /* bit masks */
	 #define MSK_ACT_INACT_CTL_INACT_Z_EN				((uint8_t)0x01)
	 #define MSK_ACT_INACT_CTL_INACT_Y_EN				((uint8_t)0x02)
	 #define MSK_ACT_INACT_CTL_INACT_X_EN				((uint8_t)0x04)
	 #define MSK_ACT_INACT_CTL_INACT_ACDC_EN			((uint8_t)0x08)
	 #define MSK_ACT_INACT_CTL_ACT_Z_EN					((uint8_t)0x10)
	 #define MSK_ACT_INACT_CTL_ACT_Y_EN					((uint8_t)0x20)
	 #define MSK_ACT_INACT_CTL_ACT_X_EN					((uint8_t)0x40)
	 #define MSK_ACT_INACT_CTL_ACT_ACDC_EN				((uint8_t)0x80)
	 
	 /* --- End of ACT_INACT_CTL definition --- */

#define REG_THRESH_FF_BASE					((uint8_t)0x28)			
#define REG_TIME_FF_BASE					((uint8_t)0x29)
#define REG_TAP_AXES_BASE					((uint8_t)0x2A)
#define REG_ACT_TAP_STATUS_BASE				((uint8_t)0x2B)
#define REG_BW_RATE_BASE					((uint8_t)0x2C)

	/**
	 * @type Read/Write (R/W)
	 * @desc Data rate and power mode control
	 *
	 * bitfields:
	 *			BW_RATE[3:0] = Rate
	 *						These bits select the device bandwidth and output data
	 *						rate. The default value is 0x0A, which translates to a
	 *						100Hz output data rate. An output data rate should be
	 *						selected that is appropriate for the communication
	 *						protocol and frequency selected. Selecting too high of
	 *						an output rate with a low communication speed results
	 *						in samples being discarded. 
	 *
	 *			BW_RATE[4] = LOW_POWER
	 *						A setting of 0 in the LOW_POWER bit selects normal
	 *						operation, and a setting of 1 select reduced power
	 *						operation, which has somewhat higher noise. 
	 */
	 
	/* bit masks */
	#define MSK_REG_BW_RATE_0_10_HZ			((uint8_t)0x00)			/* 0.10Hz Output Data Rate */
	#define MSK_REG_BW_RATE_0_20_HZ			((uint8_t)0x01)			/* 0.20Hz Output Data Rate */
	#define MSK_REG_BW_RATE_0_39_HZ			((uint8_t)0x02)			/* 0.39Hz Output Data Rate */
	#define MSK_REG_BW_RATE_0_78_HZ			((uint8_t)0x03)			/* 0.78Hz Output Data Rate */
	#define MSK_REG_BW_RATE_1HZ				((uint8_t)0x04)			/* 1.56Hz Output Data Rate */
	#define MSK_REG_BW_RATE_3HZ				((uint8_t)0x05)			/* 3.13Hz Output Data Rate */
	#define MSK_REG_BW_RATE_6HZ				((uint8_t)0x06)			/* 6.25Hz Output Data Rate */
	#define MSK_REG_BW_RATE_12HZ			((uint8_t)0x07)			/* 12.5Hz Output Data Rate */
	#define MSK_REG_BW_RATE_25HZ			((uint8_t)0x08)			/* 25Hz Output Data Rate */
	#define MSK_REG_BW_RATE_50HZ			((uint8_t)0x09)			/* 50Hz Output Data Rate */
	#define MSK_REG_BW_RATE_100HZ			((uint8_t)0x0A)
	#define MSK_REG_BW_RATE_200HZ			((uint8_t)0x0B)
	#define MSK_REG_BW_RATE_400HZ			((uint8_t)0x0C)
	#define MSK_REG_BW_RATE_800HZ			((uint8_t)0x0D)
	#define MSK_REG_BW_RATE_1600HZ			((uint8_t)0x0E)
	#define MSK_REG_BW_RATE_3200HZ			((uint8_t)0x0F)
	#define MSK_REG_BW_LOWPOWER				((uint8_t)0x10)
	
	/* --- End of BW_RATE definition --- */

#define REG_POWER_CTL_BASE					((uint8_t)0x2D)

	/**
	 * @type Read/Write (R/W)
	 * @desc Power-saving features control
	 *
	 * bitfields:
	 *			POWER_CTL[1:0] = Wakeup
	 *						These bits control the frequency readings in sleep mode at
	 *						1Hz, 2Hz, 4Hz, or 8Hz
	 *
	 *			POWER_CTL[2] = Sleep
	 *						A setting of 0 in the sleep bit puts the part into the
	 *						normal mode of operation, and a setting of 1 places the part
	 *						into sleep mode. Sleep mode suppresses DATA_READY, stops
	 *						transmission of data to FIFO, and switches the sampling rate
	 *						to one specified by the Wakeup bits. In sleep mode, only
	 *						the activity function can be used. When the DATA_READY
	 *						interrupt is suppressed, the output data registers (0x32 to
	 *						0x37) are still updated at the sampling rate set by the
	 *						Wakeup bits (POWER_CTL[1:0])
	 *
	 *			POWER_CTL[3] = Measure
	 *						A setting of 0 in the measure bit places the part into 
	 *						STANDBY mode, and a setting of 1 places the part into
	 *						MEASUREMENT mode. The ADXL343 powers up in STANDBY mode with
	 *						minimum power consumption.
	 *
	 *			POWER_CTL[4] = AUTO_SLEEP
	 *						If the link bit is set, a setting of 1 in the AUTO_SLEEP bit
	 *						enables the auto-sleep functionality. In this mode, the
	 *						ADXL343 automatically switches to sleep mode if the inactivity
	 *						function is enabled and inactivity is detected (that is, when
	 *						acceleration is below the THRESH_INACT value for at least the
	 *						time indicated by TIME_INACT). If activity is also enabled,
	 *						the ADXL343 automatically wakes up from sleep after detecting
	 *						activity and returns to operation at the output data set in
	 *						the BW_RATE register. A setting of 0 in the AUTO_SLEEP bit
	 *						disables automatic switching to sleep mode.
	 *
	 *						If the link bit is not set, the AUTO_SLEEP feature is disabled
	 *						and setting the AUTO_SLEEP bit does not have an impact on
	 *						device operation.
	 *
	 *						When clearing the AUTO_SLEEP bit, it is recommended that the
	 *						part be placed into STANDBY mode and then set back to 
	 *						MEASUREMENT mode with a subsequent write. This is done to 
	 *						ensure that the device is properly biased if sleep mode is
	 *						manually disabled.
	 *
	 *			POWER_CTL[5] = Link
	 *						A setting of 1 in the link bit with both the activity and
	 *						inactivity functions enabled delays the start of the activity
	 *						function until inactivity is detected. After activity is 
	 *						detected, inactivity detection begins, preventing detection
	 *						of activity. This bit serially links the activity and 
	 *						inactivity functions. When this bit is set to 0, the
	 *						inactivity and activity functions are concurrent. Additional
	 *						info can be found in the Link Mode section.
	 *
	 *						When clearing the link bit, it is recommended that the part
	 *						be placed into standby mode and then set back to measurement
	 *						mode with a subsequent write. This is done to ensure that the
	 *						device is properly biased if sleep mode is manually disabled.
	 *						Otherwise, the first few samples of data after the link bit is
	 *						cleared may have additional noise, especially if the device
	 *						was asleep when the bit was cleared.
	 */
	 
	/* bit masks */
	#define MSK_POWER_CTL_WKUP_8HZ					((uint8_t)0x00)
	#define MSK_POWER_CTL_WKUP_4HZ					((uint8_t)0x01)
	#define MSK_POWER_CTL_WKUP_2HZ					((uint8_t)0x02)
	#define MSK_POWER_CTL_WKUP_1HZ					((uint8_t)0x03)
	#define MSK_POWER_CTL_SLEEP						((uint8_t)0x04)
	#define MSK_POWER_CTL_MEASURE					((uint8_t)0x08)
	#define MSK_POWER_CTL_AUTOSLEEP					((uint8_t)0x10)
	#define MSK_POWER_CTL_LINK						((uint8_t)0x20)
	
	/* --- End of POWER_CTL definition --- */
	
#define REG_INT_ENABLE_BASE					((uint8_t)0x2E)

	/**
	 * @type Read/Write (R/W)
	 * @desc Interrupt enable control
	 * Setting bits in this register to a value of 1 enables their respective
	 * functions to generate interrupts, whereas a value of 0 prevents the
 	 * functions from generating interrupts. The DATA_READY, watermark, and
	 * overrun bits enable only the interrupt output; the functions are 
	 * always enabled. It is recommended that interrupts be configured before
	 * enabling their outputs.
	 */

	/* bit masks */
	#define MSK_INT_ENABLE_DIS						((uint8_t)0x00)
	#define MSK_INT_ENABLE_EN						((uint8_t)0x01)

	#define BITPOS_INT_OVERRUN						0
	#define BITPOS_INT_WATERMARK					1
	#define BITPOS_INT_FREE_FALL					2
	#define BITPOS_INT_INACTIVITY					3
	#define BITPOS_INT_ACTIVITY						4
	#define BITPOS_INT_DOUBLE_TAP					5
	#define BITPOS_INT_SINGLE_TAP					6
	#define BITPOS_INT_DATA_READY					7
	
	#define MSK_INT_ENABLE_OVERRUN					((uint8_t)MSK_INT_ENABLE_EN << BITPOS_INT_OVERRUN)
	#define MSK_INT_ENABLE_WATERMARK				((uint8_t)MSK_INT_ENABLE_EN << BITPOS_INT_WATERMARK)
	#define MSK_INT_ENABLE_FREE_FALL				((uint8_t)MSK_INT_ENABLE_EN << BITPOS_INT_FREE_FALL)
	#define MSK_INT_ENABLE_INACTIVITY				((uint8_t)MSK_INT_ENABLE_EN << BITPOS_INT_INACTIVITY)
	#define MSK_INT_ENABLE_ACTIVITY					((uint8_t)MSK_INT_ENABLE_EN << BITPOS_INT_ACTIVITY)
	#define MSK_INT_ENABLE_DOUBLE_TAP				((uint8_t)MSK_INT_ENABLE_EN << BITPOS_INT_DOUBLE_TAP)
	#define MSK_INT_ENABLE_SINGLE_TAP				((uint8_t)MSK_INT_ENABLE_EN << BITPOS_INT_SINGLE_TAP)
	#define MSK_INT_ENABLE_DATA_READY				((uint8_t)MSK_INT_ENABLE_EN << BITPOS_INT_DATA_READY)
	
	/* --- End of INT_ENABLE definition --- */
	
#define REG_INT_MAP_BASE						((uint8_t)0x2F)

	/**
	 * @type Read/Write (R/W)
	 * @desc Interrupt mapping control
	 * Any bits set to 0 in this register send their respective interrupts
	 * to the INT1 pin, whereas bits set to 1 send their respective interrupts
	 * to the INT2 pin. All selected interrupts for a given pin are OR'ed.
	 */
	 
	/* bit masks */
	#define MSK_INT_MAP_INT1						((uint8_t)0x00)
	#define MSK_INT_MAP_INT2						((uint8_t)0x01)
	
	#define MSK_INT_MAP_INT2_OVERRUN				((uint8_t)MSK_INT_MAP_INT2 << BITPOS_INT_OVERRUN)
	#define MSK_INT_MAP_INT2_WATERMARK				((uint8_t)MSK_INT_MAP_INT2 << BITPOS_INT_WATERMARK)
	#define MSK_INT_MAP_INT2_FREE_FALL				((uint8_t)MSK_INT_MAP_INT2 << BITPOS_INT_FREE_FALL)
	#define MSK_INT_MAP_INT2_INACTIVITY				((uint8_t)MSK_INT_MAP_INT2 << BITPOS_INT_INACTIVITY)
	#define MSK_INT_MAP_INT2_ACTIVITY				((uint8_t)MSK_INT_MAP_INT2 << BITPOS_INT_ACTIVITY)
	#define MSK_INT_MAP_INT2_DOUBLE_TAP				((uint8_t)MSK_INT_MAP_INT2 << BITPOS_INT_DOUBLE_TAP)
	#define MSK_INT_MAP_INT2_SINGLE_TAP				((uint8_t)MSK_INT_MAP_INT2 << BITPOS_INT_SINGLE_TAP)
	#define MSK_INT_MAP_INT2_DATA_READY				((uint8_t)MSK_INT_MAP_INT2 << BITPOS_INT_DATA_READY)
	
	/* --- End of INT_MAP definition --- */

#define REG_INT_SOURCE_BASE						((uint8_t)0x30)

	/**
	 * @type Read only (R)
	 * @desc Source of interrupts
	 * Bits set to 1 in this register indicate that their respective functions
	 * have triggered an event, whereas a value of 0 indicates that the 
	 * corresponding event has not occurred. The DATA_READY, watermark, and
	 * overrun bits are always set if the corresponding events occur, regardless
	 * of the INT_ENABLE register settings, and are cleared by reading data from
	 * the DATAX, DATAY, and DATAZ registers. The DATA_READY and watermark bits
	 * may require multiple reads, as indicated in the FIFO mode descriptions in
	 * the FIFO section. Other bits, and the corresponding interrupts, are cleared
	 * by reading the INT_SOURCE register.
	 *
	 * bitfields:
	 *			INT_SOURCE[0] = Overrun
	 *			INT_SOURCE[1] = Watermark
	 *			INT_SOURCE[2] = FREE_FALL
	 *			INT_SOURCE[3] = Inactivity
	 *			INT_SOURCE[4] = Activity
	 *			INT_SOURCE[5] = DOUBLE_TAP
	 *			INT_SOURCE[6] = SINGLE_TAP
	 *			INT_SOURCE[7] = DATA_READY
	 */
	
	/* bit masks */
	#define MSK_INT_SOURCE_OVERRUN					((uint8_t)0x01)
	#define MSK_INT_SOURCE_WATERMARK				((uint8_t)0x02)
	#define MSK_INT_SOURCE_FREE_FALL				((uint8_t)0x04)
	#define MSK_INT_SOURCE_INACTIVITY				((uint8_t)0x08)
	#define MSK_INT_SOURCE_ACTIVITY					((uint8_t)0x10)
	#define MSK_INT_SOURCE_DOUBLE_TAP				((uint8_t)0x20)
	#define MSK_INT_SOURCE_SINGLE_TAP				((uint8_t)0x40)
	#define MSK_INT_SOURCE_DATA_READY				((uint8_t)0x80)

	/* --- End of INT_SOURCE definition --- */

#define REG_DATA_FORMAT_BASE					((uint8_t)0x31)

	/**
	 * @type Read/Write (R/W)
	 * @desc Data format control
	 * The DATA_FORMAT register controls the presentation of data to register 0x32
	 * to 0x37. All data, except for the +-16g range, must be clipped to avoid
	 * rollover.
	 *
	 * bitfields:
	 *			DATA_FORMAT[1:0] = Range
	 *							These bits set the g range as described in Table 21:
	 *							+-2g -> 2b_00
	 *							+-4g -> 2b_01
	 *							+-8g -> 2b_10
	 *							+-16g-> 2b_11
	 *
	 *			DATA_FORMAT[2] = Justify
	 *							A setting of 1 in the justify bit selects left-justified (MSB)
	 *							mode, and a setting of 0 selects right-justified mode with
	 *							sign extension.
	 *
	 *			DATA_FORMAT[3] = FULL_RES
	 *							When this bit is set to a value of 1, the device is in full 
	 *							resolution mode, where the output resolution increases with the
	 *							g range set by the range bits to maintain a 4 mg/LSB scale
	 *							factor. When the FULL_RES bit is set to 0, the device is in
	 *							10-bit mode, and the range bits determine the maximum g range
	 *							and scale factor.
	 *
	 *			DATA_FORMAT[5] = INT_INVERT
	 *							A value of 0 in the INT_INVERT bit sets the interrupts to active
	 *							high, and a value of 1 sets the interrupts to active low.
	 *
	 *			DATA_FORMAT[6] = SPI
	 *							A value of 1 in the SPI bit sets the device to 3-wire SPI mode,
	 *							and a value of 0 sets the device to 4-wire SPI mode
	 *
	 *			DATA_FORMAT[7] = SELF_TEST
	 *							A setting of 1 in the SELF_TEST bit applies a self-test force to
	 *							the sensor, causing a shift in the output data. A value of 0
	 *							disables the self-test force.
	 */
	 
	/* bit masks */
	#define MSK_DATA_FORMAT_JUSTIFY				((uint8_t)0x04)
	#define MSK_DATA_FORMAT_FULL_RES			((uint8_t)0x08)
	#define MSK_DATA_FORMAT_INT_INVERT			((uint8_t)0x20)
	#define MSK_DATA_FORMAT_SPI					((uint8_t)0x40)
	#define MSK_DATA_FORMAT_SELF_TEST			((uint8_t)0x80)
	
	/* --- End of DATA_FORMAT definition --- */

#define REG_DATA_X0_BASE						((uint8_t)0x32)
#define REG_DATA_X1_BASE						((uint8_t)0x33)
#define REG_DATA_Y0_BASE						((uint8_t)0x34)
#define REG_DATA_Y1_BASE						((uint8_t)0x35)
#define REG_DATA_Z0_BASE						((uint8_t)0x36)
#define REG_DATA_Z1_BASE						((uint8_t)0x37)
#define REG_FIFO_CTL_BASE						((uint8_t)0x38)

	/** 
	 * @type Read/Write (R/W)
	 * @desc FIFO control
	 * 
	 * bitfields:
	 * 			FIFO_CTL[4:0] = Samples
	 *						The function of these bits depends on the FIFO mode selected. 
	 *						Entering a value of 0 in the samples bits immediately sets the
	 *						watermark status bit in the INT_SOURCE register, regardless
	 *						of which FIFO mode is selected. Undesirable operation may
	 *						occur if a value of 0 is used for the samples bits when trigger
	 *						mode is used. 
	 *
	 *			FIFO_CTL[5] = Trigger
	 *						A value of 0 in the trigger bit links the trigger event of 
	 *						trigger mode to INT1, and a value of 1 links the trigger event
	 *						to INT2.
	 *
	 *			FIFO_CTL[7:6] = FIFO_MODE
	 *						These bits set the FIFO mode, as described in Table 22:
	 *						0b_00	: Bypass -> FIFO is bypassed
	 *						0b_01 : FIFO -> FIFO collects up to 32 values and then stops
	 *														collecting data, collecting new data only 
	 *														when FIFO is not full.
	 *						0b_10 : Stream -> FIFO holds the last 32 data values. When
	 *															FIFO is full, the oldest data is overwritten
	 *															with newer data.
	 *						0b_11 : Trigger -> When triggered by the trigger bit, FIFO 
	 *															 holds the last data samples before the
	 *															 trigger event and then continues to collect
	 *															 data until full. New data is collected only
	 *															 when FIFO is not full. 
	 */
	 
	/* bit masks */
	#define BITPOS_FIFO_CTL_FIFOMODE			6
	
	#define MSK_FIFO_CTL_BUFFER_BYPASS			((uint8_t)0x00 << BITPOS_FIFO_CTL_FIFOMODE)
	#define MSK_FIFO_CTL_BUFFER_FIFO			((uint8_t)0x01 << BITPOS_FIFO_CTL_FIFOMODE)
	#define MSK_FIFO_CTL_BUFFER_STREAM			((uint8_t)0x02 << BITPOS_FIFO_CTL_FIFOMODE)
	#define MSK_FIFO_CTL_BUFFER_TRIGGER			((uint8_t)0x03 << BITPOS_FIFO_CTL_FIFOMODE)
	
	/* --- End of FIFO_CTL definition --- */

#define REG_FIFO_STATUS_BASE				((uint8_t)0x39)


/* Exported types ------------------------------------------------------------*/
/* Boolean typedef for ADXL343 register bitfields */
typedef enum
{
	A_DISABLE = 0,
	A_ENABLE = !A_DISABLE
	
} AccelerometerBitState;


/* Accelerometer Irqs */
typedef enum
{
	OVERRUN = BITPOS_INT_OVERRUN,
	WATERMARK = BITPOS_INT_WATERMARK,
	FREE_FALL = BITPOS_INT_FREE_FALL,
	INACTIVITY = BITPOS_INT_INACTIVITY,
	ACTIVITY = BITPOS_INT_ACTIVITY,
	DOUBLE_TAP = BITPOS_INT_DOUBLE_TAP,
	SINGLE_TAP = BITPOS_INT_SINGLE_TAP,
	DATA_READY = BITPOS_INT_DATA_READY
	
} AccelerometerIrq;


/* Accelerometer Irq Pins */
typedef enum
{
	InterruptPin1,
	InterruptPin2
	
} AcceleromterIrqPin;


/* 32-level FIFO Memory Buffer modes */
typedef enum
{
	Buffer_Bypass,
	Buffer_FIFO,
	Buffer_Stream,
	Buffer_Trigger
	
} AccelerometerBufferStates;


/**
 * @brief 	Special structure for controlling how the Inactivity and Activity events
 *          are determined. This structure is related to the ACT_INACT_CTL register
 *          (addr: 0x27). Read page 22 of the ADXL343 Datasheet for more information.
 */
typedef struct
{
	AccelerometerBitState Activity_AcDcEnable;
	
	AccelerometerBitState Activity_XAxisEnable;
	
	AccelerometerBitState Activity_YAxisEnable;
	
	AccelerometerBitState Activity_ZAxisEnable;
	
	AccelerometerBitState Inactivity_AcDcEnable;
	
	AccelerometerBitState Inactivity_XAxisEnable;
	
	AccelerometerBitState Inactivity_YAxisEnable;
	
	AccelerometerBitState Inactivity_ZAxisEnable;
	
} ActInactControlBits;


/**
 * @brief		Special structure containing status of interrupts when the INT_SOURCE
 *          register is read. An interrupt was triggered if the value of the bitfield
 *          is A_ENABLED. 
 */
typedef struct
{
	AccelerometerBitState IRQ_OVERRUN;		/* In bypass mode, this bit is set when new data replaces 
																						unread data in DATAX, DATAY, DATAZ registers. In all 
																						other modes, this bit is set when FIFO is filled*/
	AccelerometerBitState IRQ_WATERMARK;	/* This bit is set when the number of samples in FIFO equals
																						the value stored in the sample bits (Register FIFO_CTL) */
	AccelerometerBitState IRQ_FREE_FALL;	/* This is set when acceleration of less than the value
																						stored  in the THRESH_FF register is experience for more
																						time than is specified in the TIME_FF register on all axes. */
	AccelerometerBitState IRQ_INACTIVITY;	/* This bit is set when acceleration of less than the value
																						stored in the THRESH_INACT register is experienced for more
																						time than is specified in TIME_INACT register on all
																						participating axes, which is set through ACT_INACT_CTL reg. */
	AccelerometerBitState IRQ_ACTIVITY;		/* This is set when acceleration greater than the value stored
																						in the THRESH_ACT register is experienced on any participating
																						axes, also controlled by ACT_INACT_CTL register. */
	AccelerometerBitState IRQ_DOUBLE_TAP;	/* bit is set when 2 acceleration events that are greater than the
																						value in THRESH_TAP occur for less time than is specified in
																						DUR register, with the 2nd tap starting after time specified
																						by the latent register (0x22) but within the time specified
																						in the window register (0x23). */
	AccelerometerBitState IRQ_SINGLE_TAP;	/* bit is set when a single acceleration event that is greater
																						than the value in the THRESH_TAP occurs for less time than
																						is specified in the DUR register. */
	AccelerometerBitState IRQ_DATA_READY; /* This bit is set when new data is available and is cleared when
																						no new data is available */
} AccelerometerIrqStatus;



/* Exported macro ------------------------------------------------------------*/
	/* Call to clear irq flags without irq flag identification */
	#define	__CLEAR_ADXL_IRQFLAGS()				(Accelerometer_ReturnIrqFlags())

	/* Sets 32-level buffer mode to Bypass mode */
	#define __RESET_MEMORY_BUFFER_MODE()		(ADXL343_ConfigureFIFOMode(Buffer_Bypass))


/* Exported functions prototypes ---------------------------------------------*/
	/*!< @group Power related functions */
	void Accelerometer_SetMeasurementMode(AccelerometerBitState xState);
	void Accelerometer_SetLowPowerMode(AccelerometerBitState xState);
	void Accelerometer_SetOutputDataRate(uint8_t OutputDataRate_Hz);
	void Accelerometer_EnableAutoSleep(uint8_t cTimeInactivity, float cThreshmg);


	/*!< @group Interrupt related functions */
	void Accelerometer_WriteInactivityThreshTime(uint8_t cTimeInactivity, uint8_t cThreshInactivity);
	void Accelerometer_WriteActivityThreshold(uint8_t cThreshActivity);
	void Accelerometer_WriteInactivityThreshTimeMg(uint8_t cTimeSeconds, float cThreshmg);
	void Accelerometer_WriteActivityThreshMg(float cThreshmg);
	void Accelerometer_ConfigureActInactControl(ActInactControlBits xInput);
	void Accelerometer_LinkActivityInactivity(AccelerometerBitState xState);
	void Accelerometer_SetInterrupt(AccelerometerIrq xIrqPos, AccelerometerBitState xState);
	void Accelerometer_ResetInterrupt(void);
	void Accelerometer_MapInterrupt(AccelerometerIrq xIrqPos, AcceleromterIrqPin xIrqPin);
	uint8_t Accelerometer_ReturnIrqFlags(void);
	void Accelerometer_CheckInterruptFlags(AccelerometerIrqStatus *xIrqStatus);


	/*!< @group Data and Device configuration */
	void ADXL343_InterruptActiveLow(AccelerometerBitState xState);
	void ADXL343_SPI3WireMode(AccelerometerBitState xState);


	/*!< @group Initialization */
	void ADXL343_Init(void);




#ifdef __cplusplus 
}
#endif



#endif  /* __ADXL343_H */


/********************************* END OF FILE *********************************/

