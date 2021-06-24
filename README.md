### WIP FreeRTOS-BLE-Car Project ###

uC  : STM32F411RE Nucleo-64 Board
BLE : X-NUCLEO-BNRG2A1
IDE : STM32CubeIDE

# TODO:
* June 19, 2021:
	* Fix BLE initialization
	* Test FreeRTOS tasks
	* Status: Fixed - SPI clock speed too fast for BlueNRG-2 module, fixed by increasing APB1 Prescaler
* June 24, 2021:
	* Fix BLE FreeRTOS HardFault crash

# Done:


# Errors:
*BLE FreeRTOS HardFault crash:
	* Using dissasembly, 0x800_625E stored in stacked_pc points to "temp = temp->next" found in "list_get_size(tListNode * listHead)"
	* The function above is called by "free_event_list()", which is also called by "hci_send_req(struct hci_request* r, BOOL async)"
	* It was discovered that SCR->BFAR address points to one of the iterations of temp = temp->next, which is a part of tListNode member