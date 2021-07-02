# WIP FreeRTOS-BLE-Car Project #

**uC**  : [STM32F411RE Nucleo-64 Board](https://www.st.com/resource/en/datasheet/stm32f411re.pdf) <br>
**BLE** : [X-NUCLEO-BNRG2A1](https://www.st.com/resource/en/user_manual/dm00673688-getting-started-with-the-xnucleobnrg2a1-ble-expansion-board-based-on-bluenrgm2sp-module-for-stm32-nucleo-stmicroelectronics.pdf) <br>
**IDE** : STM32CubeIDE <br>

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

### Errors:
* BLE FreeRTOS HardFault crash:
	* Using dissasembly, `0x800_625E` stored in `stacked_pc` points to `temp = temp->next` found in `list_get_size(tListNode \* listHead)`
	* The function above is called by `free_event_list()`, which is also called by `hci_send_req(struct hci_request\* r, BOOL async)`
	* It was discovered that `SCR->BFAR` address points to one of the iterations of `temp = temp->next`, which is a part of `tListNode` member
* Car front tires not moving:
	* Must initialize PWM for front wheels so that front tires can move

