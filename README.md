# WIP FreeRTOS-BLE-Car Project #

**uC**  : [STM32F411RE Nucleo-64 Board](https://www.st.com/resource/en/datasheet/stm32f411re.pdf) <br>
**BLE** : [X-NUCLEO-BNRG2A1](https://www.st.com/resource/en/user_manual/dm00673688-getting-started-with-the-xnucleobnrg2a1-ble-expansion-board-based-on-bluenrgm2sp-module-for-stm32-nucleo-stmicroelectronics.pdf) <br>
**IDE** : STM32CubeIDE <br>

### Relevant Files:
* FreeRTOS_BLE_Car/ApplicationDrivers/Src : contains driver files for motor and ADXL343 accelerometer
* FreeRTOS_BLE_Car/Core/Src/car_app_ble.c : contains BLE layer for communication between STM32 and Android/iOS
* FreeRTOS_BLE_Car/Core/Src/car_app_freertos.c : contains FreeRTOS tasks and objects running in the STM32
* FreeRTOS_BLE_Car/Core/Src : contains other files such as main, peripheral initializations, and interrupt handlers








### TODO:
* June 19, 2021:
	* Fix BLE initialization
	* Test FreeRTOS tasks
	* Status: Fixed - SPI clock speed too fast for BlueNRG-2 module, fixed by increasing APB1 Prescaler
* June 23, 2021:
	* Fix BLE FreeRTOS HardFault crash
	* Status: Fixed - FreeRTOS task that calls `hci_user_evt_proc()` did not have sufficient FreeRTOS heap
* June 28, 2021:
	* Fix issue where uC cannot control wheels in Debug mode when U5V jumper is selected (USB power) instead of E5V (external 5V)
	* uC can control wheels when uC E5V jumper is active, and uC is connected to an external voltage source
* July 9, 2021:
	* Fix issue where as soon as DATA_READY interrupts are enabled on the ADXL343, EXTI4 (DATA_READY) interrupt becomes immediately available
	* Determine the cause of why and how the DATA_READY interrupt gets triggered consecutively
	* Determine a reliable method to measure time difference between each FIFO reads to accurately measure displacement and velocity
	* In periodic measurement scenario, determine why acceleration is always non-zero, while velocity is always increasing with current algorithm
	

### Done:
* June 24, 2021:
	* Fixed SPI1 Bus Speed communication issue errors
	* Fixed FreeRTOS and BLE insufficient task stack size by allocating a new task with larger stack size
	* Implemented and tested 'N', 'E', 'S', 'W', 'X' commands on BLE Scanner App
* June 27, 2021:
	* Implemented and tested (through Logic Analyzer) `__MOTOR_SetShiftRegister()` function that will be used to communicate with Shift Register
	* Tested PWM pins including modifying CCRx registers
* June 28, 2021:
	* Tested `__TESTMOTOR_MoveWheel()` and writing `0xAA` into the shift register. Both rear wheels moved in reverse direction.
* June 29, 2021:
	* Managed to control directions (forward/backward/off) for both rear wheels
	* Included more implementations and fixed PWM speed control implementation (require more tests)
* July 2, 2021:
	* All wheels moving properly (although one of the front tires are wired incorrectly)
	* Successfully integrated BLE writes and car movements for moving car forward and backward
	* Implemented FreeRTOS timer to stop car movements after 1.5 seconds
* July 6, 2021:
	* Car can steer left (by moving forward all right wheels), and right (by moving all left wheels)
	* Included test function for testing all wheel sequences
	* Fixed front left wheel and front right wheel control mix-up. `Motor_ConfigWheelDirection()` function properly controls all wheels.
* July 9, 2021:
	* Implemented Accelerometer FIFO related functions including `ADXL_ReadAcceleration()`, `ADXL_TwosComplement_13bits()`, `ADXL_ConfigureAccelerationRange()`, and `ADXL343_FullResolutionMode()`
	* Tested Twos Complement on Online C Compiler. Observed some of the FIFO/Data initializations with a Logic Analyzer
	* Tested measuring acceleration and calculating velocity every 25ms. Velocity seems to be accumulating even though car is not moving. Acceleration seems to be non-zero even though car is not moving either.
	
	
### Errors:
* BLE FreeRTOS HardFault crash:
	* Using dissasembly, `0x800_625E` stored in `stacked_pc` points to `temp = temp->next` found in `list_get_size(tListNode \* listHead)`
	* The function above is called by `free_event_list()`, which is also called by `hci_send_req(struct hci_request\* r, BOOL async)`
	* It was discovered that `SCR->BFAR` address points to one of the iterations of `temp = temp->next`, which is a part of `tListNode` member
* Car front tires not moving:
	* Must initialize PWM for front wheels so that front tires can move
* I2C Bus hangs after DATA_READY interrupt enable:
	* STOP event was never issued by the STM32
	* EXTI4 (INT1 on ADXL343) gets triggered repeatedly at a fast rate as soon as DATA_READY interrupt is enabled
