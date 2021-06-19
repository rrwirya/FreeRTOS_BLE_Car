/**
  **************************************************************************************************
  * @file       : car_app_ble.c
  * @brief      : This file contains all functions related to initializing the Bluetooth Low
  *			  	  Energy (BLE) Stack of the BlueNRG. Refer to the BlueNRGx Programming Manual found
  *			  	  in ST's website.
  * @author		: Reggie W
  **************************************************************************************************
  */


/* Includes --------------------------------------------------------------------------------------*/
#include "hci.h"
#include "hci_tl.h"
#include "car_app_ble.h"

#include "bluenrg1_hal_aci.h"
#include "bluenrg1_gatt_aci.h"
#include "bluenrg1_gap_aci.h"
#include "bluenrg1_hci_le.h"


/* Private includes ------------------------------------------------------------------------------*/
#include "bluenrg_conf.h"				/* Contains configured Bluetooth Parameters in CubeMX */


/* External variables ----------------------------------------------------------------------------*/


/* Private typedef -------------------------------------------------------------------------------*/


/* Private define --------------------------------------------------------------------------------*/


/* Private variables -----------------------------------------------------------------------------*/
uint16_t discovery_time 			= 0;

static uint16_t hGAPService;
static uint16_t hDevNameChar;
static uint16_t hAppearanceChar;

	/*--- Variables that will hold service and characteristic UUIDs ---*/
	Service_UUID_t suuid_object;
	Char_UUID_t char_obj_1, char_obj_2, char_obj_3, char_obj_4, char_obj_5;

	/*--- Handle to services and associated characteristics ---*/
	static uint16_t hService;
	static uint16_t hClientNotify_OverSpeed;
	static uint16_t hClientNotify_Crash;
	static uint16_t hClientRead_Velocity;
	static uint16_t hClientWrite_Direction;
	static uint16_t hClientRead_VerifyDirection;

	/*--- Handle to associated characteristic descriptors ---*/
	static uint16_t hFirstCharDesc;
	static uint16_t hSecondCharDesc;
	static uint16_t hThirdCharDesc;
	static uint16_t hFourthCharDesc;
	static uint16_t hFifthCharDesc;

	/* DISCOVERY/CONNECTIVITY DETAILS */
	static connectionStatus_t Conn_Details;


/* Private macro ---------------------------------------------------------------------------------*/


/* Private function prototypes -------------------------------------------------------------------*/
static void Setup_DeviceAddress(void);
static void GAP_Peripheral_ConfigService(void);
static void Server_ResetConnectionStatus(void);


/***************************** BLE Stack and Interface Initialization  **********************************/

/**
  * @brief	Main initialization function. To be called at system startup
  * @note		Initializes BlueNRG-2 SPI Interface, HCI application, GAP and GATT layers
  */
void BlueNRG_Init(void)
{
	uint8_t ret;

	/* Initialize SPI1 Peripheral and Bluetooth Host Controller Interface */
	hci_init(APP_UserEvtRx, NULL);

	/* SAMPLEAPP EXAMPLE - Software reset of device and required 2 second delay */
	hci_reset();
	HAL_Delay(2000);

	/* Configure transmit power to high power at -2dBm */
	ret = aci_hal_set_tx_power_level(1, 4);
	assert_param(ret == BLE_STATUS_SUCCESS);

	/* Configure BLE device public address if it will be used */
	Setup_DeviceAddress();

	/* Initialize BLE GATT layer */
	ret = aci_gatt_init();
	assert_param(ret == BLE_STATUS_SUCCESS);

#if defined(ENABLE_SM)

	/* Configure proper security I/O capability and authentication requirement */
	aci_gap_set_io_capability();
	aci_gap_set_authentication_requirement();

#endif

#if defined(DEVICE_TYPE_GAP_PERIPHERAL)

	/** Initialize BLE GAP layer with the following characteristics:
	 * 		+ role = 0x01 -> BLE-FreeRTOS Car is Peripheral device
	 *		+ privacy_enabled = 0x00 -> Privacy disabled
	 *		+ device_name_char_len = 8
	 */
	aci_gap_init(GAP_PERIPHERAL_ROLE, GAP_PRIVACY_DISABLED, 0x08, &hGAPService, &hDevNameChar, &hAppearanceChar);

	/* Configure further the services and characteristics to be included in the GATT database */
	GAP_Peripheral_ConfigService();

	Server_ResetConnectionStatus();

#elif defined(DEVICE_TYPE_GAP_CENTRAL)

	/** Initialize BLE GAP layer with the following characteristics:
	 * 		+ role = 0x04 -> Controller (non-existent at the moment) is Central device
	 *		+ privacy_enabled = 0x00 -> Privacy disabled
	 *		+ device_name_char_len = 8
     */
	aci_gap_init(GAP_CENTRAL_ROLE, GAP_PRIVACY_DISABLED, 0x08, &hGAPService, &hDevNameChar, &hAppearanceChar);

#endif

}

/**
  * @brief 	Sets up the device MAC address (first 3 bytes are fixed, while the last 3 bytes are randomized).
  * @note		This MAC address will only be used to connect with other (Central devices). Central devices
  *					will see this MAC address and use it to connect with this peripheral device. Peripheral will
  *					include the MAC address in the advertisement data.
  */
static void Setup_DeviceAddress(void)
{
	tBleStatus ret;
	uint8_t bdaddr[] = {0x00, 0x00, 0x00, 0xE1, 0x80, 0x02};
	uint8_t random_number[8];

	/* get a random number from BlueNRG */
	ret = hci_le_rand(random_number);
	if(ret != BLE_STATUS_SUCCESS)
	{
		PRINT_DBG("hci_le_rand() call failed: 0x%02x\r\n", ret);
	}

	discovery_time = 3000; /* at least 3 seconds */
	/* setup discovery time with random number */
	for (uint8_t i=0; i<8; i++)
	{
		discovery_time += (2*random_number[i]);
	}

	/* Setup last 3 bytes of public address with random number */
	bdaddr[0] = (uint8_t) (random_number[0]);
	bdaddr[1] = (uint8_t) (random_number[3]);
	bdaddr[2] = (uint8_t) (random_number[6]);

	/* Configure public MAC address (bdaddr[3:5] is company specific, while bdaddr[0:2] is device specific) */
	ret = aci_hal_write_config_data(CONFIG_DATA_PUBADDR_OFFSET, CONFIG_DATA_PUBADDR_LEN, bdaddr);
	if(ret != BLE_STATUS_SUCCESS)
	{
	  PRINT_DBG("Setting BD_ADDR failed 0x%02x\r\n", ret);
	}
	else
	{
		PRINT_DBG("Public address: ");
		for (uint8_t i=5; i>0; i--)
		{
		  PRINT_DBG("%02X-", bdaddr[i]);
		}
		PRINT_DBG("%02X\r\n", bdaddr[0]);
	}

}

/**
  * @brief	Configure Services and associated Characteristics in GATT Server
  * @note		Must be called as these characteristics are involved with data flow and data
  *					transfer via BLE protocol. Values will be exchanged through characteristics.
  */
static void GAP_Peripheral_ConfigService(void)
{
	/* 128-bit UUID Declarations for 1 Service and the 4 Characteristics underneath that Service */

	/* Configure 128-bit Service UUID since Sciton does not have dedicated 16-bit Service
	   UUID with Bluetooth SIG. Service UUID obtained through UUID generator.
	   UUID (uuidgenerator.net): a898328b-03f9-4d63-b11d-51505ae1ce5d */
	const uint8_t service_uuid[16] =
	{0x5D,0xCE,0xE1,0x5A,0x50,0x51,0x1D,0xB1,0x63,0x4D,0xF9,0x03,0x8B,0x32,0x98,0xA8};
	const uint8_t char1_uuid[16] =
	{0x96,0xF7,0x4E,0xBF,0xB3,0x8E,0xB7,0x82,0x36,0x4B,0x7E,0x8B,0x00,0x00,0x00,0x01};
	const uint8_t char2_uuid[16] =
	{0x96,0xF7,0x4E,0xBF,0xB3,0x8E,0xB7,0x82,0x36,0x4B,0x7E,0x8B,0x00,0x00,0x00,0x02};
	const uint8_t char3_uuid[16] =
	{0x96,0xF7,0x4E,0xBF,0xB3,0x8E,0xB7,0x82,0x36,0x4B,0x7E,0x8B,0x00,0x00,0x00,0x03};
	const uint8_t char4_uuid[16] =
	{0x96,0xF7,0x4E,0xBF,0xB3,0x8E,0xB7,0x82,0x36,0x4B,0x7E,0x8B,0x00,0x00,0x00,0x04};
	const uint8_t char5_uuid[16] =
	{0x96,0xF7,0x4E,0xBF,0xB3,0x8E,0xB7,0x82,0x36,0x4B,0x7E,0x8B,0x00,0x00,0x00,0x05};

	BLUENRG_memcpy(&suuid_object.Service_UUID_128, service_uuid, 16);

	/* Add the Bluetooth Service based on the configuration above */
	aci_gatt_add_service(UUID_TYPE_128, &suuid_object, PRIMARY_SERVICE, 20, &hService);

	/* Variables that will hold the four characteristics' 128-bit UUID number.
	   The first characteristic's UUID was generated with a UUID random number generator,
	   and the subsequent characteristics' UUID were derived from that first char UUID. */


	/**
	  * @brief First Characteristic (...TBD...)
		*
		* Handle 													: Service handle to associate it with
		* UUID type													: 128-bits
		* Maximum length of the characteristic value				: x bytes
		* Characteristic Properties									: CHAR_PROP_NOTIFY (...TBD...)
		* Security Permissions										: None
		* GATT event mask flags										: GATT_DONT_NOTIFY_EVENTS
		* Enc_Key_Size: 0x07 - the minimum encryption key size required to read the characteristic
		* Fixed characteristic value length							: FIXED_LENGTH
		*
		* This characteristic will be used (...TBD...)
		*/
	/* First characteristic's UUID */
	BLUENRG_memcpy(&char_obj_1.Char_UUID_128, char1_uuid, 16);

	/**
	  * @brief Second Characteristic (...TBD...)
		*
		* Handle 													: Service handle to associate it with
		* UUID type													: 128-bits
		* Maximum length of the characteristic value				: x bytes
		* Characteristic Properties									: CHAR_PROP_NOTIFY (...TBD...)
		* Security Permissions										: None
		* GATT event mask flags										: GATT_DONT_NOTIFY_EVENTS
		* Enc_Key_Size: 0x07 - the minimum encryption key size required to read the characteristic
		* Fixed characteristic value length							: FIXED_LENGTH
		*
		* This characteristic will be used (...TBD...)
		*/
	/* Second characteristic's UUID */
	BLUENRG_memcpy(&char_obj_2.Char_UUID_128, char2_uuid, 16);

	/**
	  * @brief Third Characteristic (...TBD...)
		*
		* Handle 													: Service handle to associate it with
		* UUID type													: 128-bits
		* Maximum length of the characteristic value				: x bytes
		* Characteristic Properties									: CHAR_PROP_READ (...TBD...)
		* Security Permissions										: None
		* GATT event mask flags										: GATT_DONT_NOTIFY_EVENTS
		* Enc_Key_Size: 0x07 - the minimum encryption key size required to read the characteristic
		* Fixed characteristic value length							: FIXED_LENGTH
		*
		* This characteristic will be used (...TBD...)
		*/
	/* Third characteristic's UUID */
	BLUENRG_memcpy(&char_obj_3.Char_UUID_128, char3_uuid, 16);

	/**
	  * @brief Fourth Characteristic (...TBD...)
		*
		* Handle 													: Service handle to associate it with
		* UUID type													: 128-bits
		* Maximum length of the characteristic value				: x bytes
		* Characteristic Properties									: CHAR_PROP_WRITE_WITHOUT_RESP (...TBD...)
		* Security Permissions										: None
		* GATT event mask flags										: GATT_DONT_NOTIFY_EVENTS
		* Enc_Key_Size: 0x07 - the minimum encryption key size required to read the characteristic
		* Fixed characteristic value length							: FIXED_LENGTH
		*
		* This characteristic will be used (...TBD...)
		*/
	/* Fourth characteristic's UUID */
	BLUENRG_memcpy(&char_obj_4.Char_UUID_128, char4_uuid, 16);

	/**
	  * @brief Fifth Characteristic (...TBD...)
		*
		* Handle 													: Service handle to associate it with
		* UUID type													: 128-bits
		* Maximum length of the characteristic value				: x bytes
		* Characteristic Properties									: CHAR_PROP_READ (...TBD...)
		* Security Permissions										: None
		* GATT event mask flags										: GATT_DONT_NOTIFY_EVENTS
		* Enc_Key_Size: 0x07 - the minimum encryption key size required to read the characteristic
		* Fixed characteristic value length							: FIXED_LENGTH
		*
		* This characteristic will be used (...TBD...)
		*/
	/* Third characteristic's UUID */
	BLUENRG_memcpy(&char_obj_5.Char_UUID_128, char5_uuid, 16);

	/* Configure the four characteristic defined above for the GATT server (peripheral) */\
	/* Characteristic will be used to notify if car went above speed limit */
	aci_gatt_add_char(hService, UUID_TYPE_128, &char_obj_1, 20, CHAR_PROP_NOTIFY,
											ATTR_PERMISSION_NONE, GATT_DONT_NOTIFY_EVENTS,
											0x07, CHAR_VALUE_LEN_CONSTANT, &hClientNotify_OverSpeed);

	/* Characteristic will be used to notify if car almost crashed */
	aci_gatt_add_char(hService, UUID_TYPE_128, &char_obj_2, 20, CHAR_PROP_NOTIFY,
											ATTR_PERMISSION_NONE, GATT_DONT_NOTIFY_EVENTS,
											0x07, CHAR_VALUE_LEN_CONSTANT, &hClientNotify_Crash);

	/* Characteristic will be used to read velocity */
	aci_gatt_add_char(hService, UUID_TYPE_128, &char_obj_3, 20, CHAR_PROP_READ,
											ATTR_PERMISSION_NONE, GATT_DONT_NOTIFY_EVENTS,
											0x07, CHAR_VALUE_LEN_CONSTANT, &hClientRead_Velocity);

	/* Characteristic will be used to receive input: N(orth), E(ast), S(outh), W(est), or SS (Stop) */
	aci_gatt_add_char(hService, UUID_TYPE_128, &char_obj_4, 20, CHAR_PROP_WRITE|CHAR_PROP_WRITE_WITHOUT_RESP,
											ATTR_PERMISSION_NONE, GATT_NOTIFY_ATTRIBUTE_WRITE,
											0x07, CHAR_VALUE_LEN_CONSTANT, &hClientWrite_Direction);

	/* Characteristic will be used to read the direction previously set/configured */
	aci_gatt_add_char(hService, UUID_TYPE_128, &char_obj_5, 20, CHAR_PROP_READ,
											ATTR_PERMISSION_NONE, GATT_DONT_NOTIFY_EVENTS,
											0x07, CHAR_VALUE_LEN_CONSTANT, &hClientRead_VerifyDirection);

	/* CCCD value */
	Char_Desc_Uuid_t DescriptorProperty;
	DescriptorProperty.Char_UUID_16 = CHAR_USER_DESC_UUID;

	const char char1name[] = {'W','R','N','_','S','P','E','E','D'};
	const char char2name[] = {'W','R','N','_','C','R','A','S','H'};
	const char char3name[] = {'R','D','_','V','E','L','O','C','I','T','Y'};
	const char char4name[] = {'W','R','_','D','I','R','E','C','T','I','O','N'};
	const char char5name[] = {'R','D','_','D','I','R','E','C','T','I','O','N'};

	/* Configure CCCD for the characteristics above (associated with characteristic UUIDs). The CCCD's
     might only be necessary for indicate/notify related events, as the CCCD feature in the GATT server
     allows client to configure server to automatically push new data to the client in the notify/indicate
     characteristics.	*/

	aci_gatt_add_char_desc(hService, hClientNotify_OverSpeed, UUID_TYPE_16, &DescriptorProperty,
														128, 9, (uint8_t*)char1name, ATTR_PERMISSION_NONE, ATTR_ACCESS_READ_ONLY,
														GATT_DONT_NOTIFY_EVENTS, 7, CHAR_VALUE_LEN_CONSTANT, &hFirstCharDesc);
	aci_gatt_add_char_desc(hService, hClientNotify_Crash, UUID_TYPE_16, &DescriptorProperty,
														128, 9, (uint8_t*)char2name, ATTR_PERMISSION_NONE, ATTR_ACCESS_READ_ONLY,
														GATT_DONT_NOTIFY_EVENTS, 7, CHAR_VALUE_LEN_CONSTANT, &hSecondCharDesc);
	aci_gatt_add_char_desc(hService, hClientRead_Velocity, UUID_TYPE_16, &DescriptorProperty,
														128, 11, (uint8_t*)char3name, ATTR_PERMISSION_NONE, ATTR_ACCESS_READ_ONLY,
														GATT_DONT_NOTIFY_EVENTS, 7, CHAR_VALUE_LEN_CONSTANT, &hThirdCharDesc);
	aci_gatt_add_char_desc(hService, hClientWrite_Direction, UUID_TYPE_16, &DescriptorProperty,
														128, 10, (uint8_t*)char4name, ATTR_PERMISSION_NONE, ATTR_ACCESS_READ_WRITE,
														GATT_DONT_NOTIFY_EVENTS, 7, CHAR_VALUE_LEN_CONSTANT, &hFourthCharDesc);
	aci_gatt_add_char_desc(hService, hClientRead_VerifyDirection, UUID_TYPE_16, &DescriptorProperty,
															128, 11, (uint8_t*)char5name, ATTR_PERMISSION_NONE, ATTR_ACCESS_READ_ONLY,
															GATT_DONT_NOTIFY_EVENTS, 7, CHAR_VALUE_LEN_CONSTANT, &hFifthCharDesc);

	/*
	Char_Desc_Uuid_t DescriptorProperty;
	DescriptorProperty.Char_UUID_16 = CHAR_CLIENT_CONFIG_DESC_UUID;
	uint16_t char_desc_value = 0;
	aci_gatt_add_char_desc(hService, hClientWrite_Direction, UUID_TYPE_16, &DescriptorProperty, 128, 9,
													(uint8_t*)&char_desc_value, ATTR_PERMISSION_NONE, ATTR_ACCESS_READ_WRITE,
													GATT_NOTIFY_ATTRIBUTE_WRITE, 7, CHAR_VALUE_LEN_CONSTANT, &hFourthCharDesc);
	*/
}


/**
  * @brief	Resets/Deletes the entries of the variable holding the details of the connection with
  *					the GATT client
  */
static void Server_ResetConnectionStatus(void)
{
	/* Set to unknown/unregistered device role */
	Conn_Details.deviceRole = 0xFF;

	/* Set all fields to MAX_UINT16_T */
	Conn_Details.connectionhandle = 0xFFFF;
	Conn_Details.BLE_ConnInterval = 0xFFFF;
	Conn_Details.BLE_ConnLatency = 0xFFFF;
	Conn_Details.BLE_SupervisionTimeout = 0xFFFF;

	/* Set status to not connected */
	Conn_Details.ConnectionStatus = STATE_NOT_CONNECTED;

	/* Reset 6-byte MAC address */
	BLUENRG_memset(&Conn_Details.BLE_Client_Addr[0], 0, 6);
}

/**
  * @brief	Enables BLE Peripheral device to be discoverable by advertising (with certain parameters)
  * @note		When BLE Peripheral adverises, it does so periodically at certain intervals. At these times
  *					power consumption of device will be high.
  */
void BlueNRG_MakeDeviceDiscoverable(void)
{
	uint8_t ret;

	/* Name that will be broadcasted to Central Devices scanning */
	const char local_name[] = {AD_TYPE_COMPLETE_LOCAL_NAME, 'F','R','T','S','-','B','L','E','-','C','a','r'};

	/* Disable scan response: passive scan */
	hci_le_set_scan_response_data(0, NULL);

	/* Put the GAP peripheral in general discoverable mode:
			Advertising_Type: ADV_IND(undirected scannable and connectable);
			Advertising_Interval_Min: 100;
			Advertising_Interval_Max: 100;
			Own_Address_Type: PUBLIC_ADDR (public address: 0x00);
			Adv_Filter_Policy: NO_WHITE_LIST_USE (no whit list is used);
			Local_Name_Lenght: 13
			Local_Name: BlueNRG1Test;
			Service_Uuid_Length: 0 (no service to be advertised); Service_Uuid_List: NULL;
			Slave_Conn_Interval_Min: 0 (Slave connection internal minimum value);
			Slave_Conn_Interval_Max: 0 (Slave connection internal maximum value).
	*/

	/* Configure scan response packet to be sent when GAP peripheral receives scan requests from GAP
     central performing general discovery procedure (active scan).
		 Scan Response Message must contain the following in this specific order:
				Length = 0x11
				Service UUID Type = 0x06 (128-bits Service UUID)
				Service UUID = (UUID taken from above)
	 */
	uint8_t uuidscanresponse[18] =
				{0x11,0x06,0x5D,0xCE,0xE1,0x5A,0x50,0x51,0x1D,0xB1,0x63,0x4D,0xF9,0x03,0x8B,0x32,0x98,0xA8};
	hci_le_set_scan_response_data(18, uuidscanresponse);

	/* Place Bluetooth Peripheral Device in Advertising State */
	ret = aci_gap_set_discoverable(ADV_IND, ADV_INTERV_MIN, ADV_INTERV_MAX, PUBLIC_ADDR,
																	NO_WHITE_LIST_USE, sizeof(local_name), (uint8_t*)local_name,
																	0, NULL, 0, 0);

	assert_param(ret == BLE_STATUS_SUCCESS);

	/* Update status */
	Conn_Details.ConnectionStatus = STATE_AWAITING_CONNECTION;
}

/********************** BLE HCI related events and event callbacks in Stack *****************************/

/**
  * @brief	Function taken from STM32Cube\Repository\Packs\STMicroelectronics\X-CUBE-BLE2\3.2.0\Projects\...
  *					...\Nucleo-L476RG\Application\SampleApp\...\gatt_db.c
  *					To be used with hci_init() that must be called before using BLE stack
  */
void APP_UserEvtRx(void *pData)
{
  uint32_t i;

  hci_spi_pckt *hci_pckt = (hci_spi_pckt *)pData;

  if(hci_pckt->type == HCI_EVENT_PKT)
  {
    hci_event_pckt *event_pckt = (hci_event_pckt*)hci_pckt->data;

    if(event_pckt->evt == EVT_LE_META_EVENT)
    {
      evt_le_meta_event *evt = (void *)event_pckt->data;

      for (i = 0; i < (sizeof(hci_le_meta_events_table)/sizeof(hci_le_meta_events_table_type)); i++)
      {
        if (evt->subevent == hci_le_meta_events_table[i].evt_code)
        {
          hci_le_meta_events_table[i].process((void *)evt->data);
        }
      }
    }
    else if(event_pckt->evt == EVT_VENDOR)
    {
      evt_blue_aci *blue_evt = (void*)event_pckt->data;

      for (i = 0; i < (sizeof(hci_vendor_specific_events_table)/sizeof(hci_vendor_specific_events_table_type)); i++)
      {
        if (blue_evt->ecode == hci_vendor_specific_events_table[i].evt_code)
        {
          hci_vendor_specific_events_table[i].process((void *)blue_evt->data);
        }
      }
    }
    else
    {
      for (i = 0; i < (sizeof(hci_events_table)/sizeof(hci_events_table_type)); i++)
      {
        if (event_pckt->evt == hci_events_table[i].evt_code)
        {
          hci_events_table[i].process((void *)event_pckt->data);
        }
      }
    }
  }
}

/*******************************************************************************
 * Function Name  : hci_le_connection_complete_event.
 * Description    : This event indicates the end of a connection procedure.
 * Input          : See file bluenrg1_events.h
 * Output         : See file bluenrg1_events.h
 * Return         : See file bluenrg1_events.h
 *******************************************************************************/
void hci_le_connection_complete_event(uint8_t Status,
                                      uint16_t Connection_Handle,
                                      uint8_t Role,
                                      uint8_t Peer_Address_Type,
                                      uint8_t Peer_Address[6],
                                      uint16_t Conn_Interval,
                                      uint16_t Conn_Latency,
                                      uint16_t Supervision_Timeout,
                                      uint8_t Master_Clock_Accuracy)

{
	/* This callback function/event only saves connection handle */
	Conn_Details.connectionhandle = Connection_Handle;

	/* Role should be slave: 0x01 (if 0x00, it is master and incorrect in this example project) */
	Conn_Details.deviceRole = Role;

	/* Save connection details in memory */
	BLUENRG_memcpy(&Conn_Details.BLE_Client_Addr, Peer_Address, 6);
	Conn_Details.BLE_ConnInterval = Conn_Interval;
	Conn_Details.BLE_ConnLatency = Conn_Latency;
	Conn_Details.BLE_SupervisionTimeout = Supervision_Timeout;

	/* Update connection status to connected */
	Conn_Details.ConnectionStatus = STATE_CONNECTED;

} /* end hci_le_connection_complete_event() */

/*******************************************************************************
 * Function Name  : hci_disconnection_complete_event.
 * Description    : This event indicates the end of a disconnection procedure.
 * Input          : See file bluenrg1_events.h
 * Output         : See file bluenrg1_events.h
 * Return         : See file bluenrg1_events.h
 *******************************************************************************/
void hci_disconnection_complete_event(uint8_t Status,
                                      uint16_t Connection_Handle,
                                      uint8_t Reason)
{
	/* Resets all connectivity status details */
	Server_ResetConnectionStatus();

} /* end hci_disconnection_complete_event() */

/*******************************************************************************
 * Function Name  : aci_gatt_notification_event.
 * Description    : Callback function triggered at client when GATT server does
										a NOTIFY.
 * Input          : See file bluenrg1_events.h
 * Output         : See file bluenrg1_events.h
 * Return         : See file bluenrg1_events.h
 *******************************************************************************/
void aci_gatt_notification_event(uint16_t Connection_Handle,
                                 uint16_t Attribute_Handle,
                                 uint8_t Attribute_Value_Length,
                                 uint8_t Attribute_Value[])
{


} /* end aci_gatt_notification_event() */

/*******************************************************************************
 * Function Name  : aci_gatt_notification_event.
 * Description    : Callback function triggered when a WRITE is executed.
 * Input          : See file bluenrg1_events.h
 * Output         : See file bluenrg1_events.h
 * Return         : See file bluenrg1_events.h
 *******************************************************************************/
void aci_gatt_attribute_modified_event(uint16_t Connection_Handle,
                                       uint16_t Attr_Handle,
                                       uint16_t Offset,
                                       uint16_t Attr_Data_Length,
                                       uint8_t Attr_Data[])
{

	/* Determine which characteristic was modified by Client (Indicate and Notify characteristics
	   are modified by Client only if Client acknowledges these features on Server) */
	if(Attr_Handle == hClientWrite_Direction+1)
	{
		if(Attr_Data_Length == 1)
		{
			if((Attr_Data[0] == 0x4E)||((Attr_Data[0] == 0x6E)))
			{
				/* If input character is 'N' or 'n' representing North or forward */

				/* Notify ACK to master through fifth characteristic (verify direction) printing 'NORTH'*/
				uint8_t buff[6] = {0x4E, 0x4F, 0x52, 0x54, 0x48, 0x00};
				aci_gatt_update_char_value(hService, hClientRead_VerifyDirection, 0, 6, buff);
			}
			else if((Attr_Data[0] == 0x45)||((Attr_Data[0] == 0x65)))
			{
				/* If input character is 'E' or 'e' representing East or right */

				/* Notify ACK to master through fifth characteristic (verify direction) printing 'EAST' */
				uint8_t buff[6] = {0x45, 0x41, 0x53, 0x54, 0x00, 0x00};
				aci_gatt_update_char_value(hService, hClientRead_VerifyDirection, 0, 6, buff);
			}
			else if((Attr_Data[0] == 0x53)||((Attr_Data[0] == 0x73)))
			{
				/* If input character is 'S' or 's' representing South or backwards */

				/* Notify ACK to master through fifth characteristic (verify direction) printing 'SOUTH' */
				uint8_t buff[6] = {0x53, 0x4F, 0x55, 0x54, 0x48, 0x00};
				aci_gatt_update_char_value(hService, hClientRead_VerifyDirection, 0, 6, buff);
			}
			else if((Attr_Data[0] == 0x57)||(Attr_Data[0] == 0x77))
			{
				/* If input character is 'W' or 'w' representing West or left */

				/* Notify ACK to master through fifth characteristic (verify direction) printing 'WEST' */
				uint8_t buff[6] = {0x57, 0x45, 0x53, 0x54, 0x00, 0x00};
				aci_gatt_update_char_value(hService, hClientRead_VerifyDirection, 0, 6, buff);
			}
		}
		else if(Attr_Data_Length == 2)
		{



		}
	}

} /* end aci_gatt_attribute_modified_event() */

/********************** User Application related functions/events/processes *****************************/

/**
  * @brief 	Updates the BLE events list. This will also contain FSM for BLE communication protocol
  * @note	hci_user_evt_proc() must be called after an event is received from the HCI interface. This
  *			function will call the appropriate event callback functions related to BLE write/read/indicate/
  *			notify events. Must be called outside an ISR.
  *
  */
void BlueNRG_Loop(void)
{
	hci_user_evt_proc();

	/* FSM to handle device connectivity */
	switch(Conn_Details.ConnectionStatus)
	{
		case STATE_NOT_CONNECTED:
		{
			BlueNRG_MakeDeviceDiscoverable();
			Conn_Details.ConnectionStatus = STATE_AWAITING_CONNECTION;
			break;
		}

		case STATE_AWAITING_CONNECTION:
		{
			/* Implement counter to turn off device when it doesn't connect successfully */
			Conn_Details.ConnectionStatus = STATE_AWAITING_CONNECTION;
			break;
		}

		case STATE_CONNECTED:
		{
			Conn_Details.ConnectionStatus = STATE_CONNECTED;
			break;
		}

	}
}



/******************************************* END OF FILE *******************************************/
