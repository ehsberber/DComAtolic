#ifndef __USER_DEFINITIONS__
#define __USER_DEFINITIONS__
/// includes 
#include "main.h"
#include "usart.h"

#define UART_FL_SET 1
#define UART_FL_RESET 0

#define HI2C_PCF hi2c3

#define HSPI_DAC hspi1
#define HSPI_DAC1_PORT DAC1_CS_GPIO_Port
#define HSPI_DAC1_PIN DAC1_CS_Pin
#define HSPI_DAC2_PORT DAC2_CS_GPIO_Port
#define HSPI_DAC2_PIN DAC2_CS_Pin

#define HSPI_EEPROM hspi1
#define HSPI_EEPROM_PORT EEPROM_CS_GPIO_Port
#define HSPI_EEPROM_PIN EEPROM_CS_Pin

#define HUART_PLC huart1
#define HUART_PLC_INSTANCE USART1

#define PLC_PACKET_FINISHED_COUNTER_LENGHT 10
#define PLC_UART_TX_BUFFER_LENGHT 200
#define PLC_UART_RX_BUFFER_LENGHT 200
//
#define SMS_DELIMETER ","
#define BLE_DELIMETER ";"
#define WIFI_DELIMETER ";"
// 
#define NETWORK_MAX_NO 6
#define GPRS_MAX_REPEATATION_NO 3
#define GPRS_MAX_TIMEIOT_TIME 100

#define SMS_MAX_REPEATATION_NO 3
#define SMS_MAX_TIMEIOT_TIME 100
#define SMS_MSISDN_MAX_NO 16
#define SMS_MAX_NO 6

#define BLE_MAX_LENGHT_NAME 10
#define BLE_MAX_LENGHT_PASSWORD 20
#define BLE_MAX_LENGHT_SERVICE 10
#define BLE_MAX_LENGHT_CHAR 10

#define WIFI_MAX_LENGHT_NAME 10
#define WIFI_MAX_LENGHT_PASSWORD 20
#define WIFI_MAX_LENGHT_IP 15
#define WIFI_MAX_LENGHT_PORT 5

#define BLE_MAX_NO 5
#define BLE_MAC_ADDRESS_MAX_NO 17
#define NETWORK_IP_MAX_NO 20
#define NETWORK_PORT_MAX_NO 5

//
#define FLAG_SET 1
#define FLAG_RESET 0
//
#define QUECTELL_SMS_DEVICE1 0
#define QUECTELL_SMS_DEVICE2 1
#define QUECTELL_SMS_DEVICE3 2
#define QUECTELL_SMS_DEVICE4 3
#define QUECTELL_SMS_DEVICE5 4 
#define QUECTELL_SMS_DEVICE6 5
#define QUECTELL_SMS_DEVICE7 6
#define QUECTELL_SMS_DEVICE8 7
//
#define QUECTELL_NETWORK_DEVICE1 0
#define QUECTELL_NETWORK_DEVICE2 1
#define QUECTELL_NETWORK_DEVICE3 2
#define QUECTELL_NETWORK_DEVICE4 3
#define QUECTELL_NETWORK_DEVICE5 4
#define QUECTELL_NETWORK_DEVICE6 5 
#define QUECTELL_NETWORK_DEVICE7 6
#define QUECTELL_NETWORK_DEVICE8 7
//
#define QUECTELL_FLAG_SMS_CMGF 0
#define QUECTELL_FLAG_SMS_CSCS 1
#define QUECTELL_FLAG_SMS_CMGS 2
//
#define QUECTELL_RESPONSE_OF_SMS_CMGF "OK"
#define QUECTELL_RESPONSE_OF_SMS_CMGF_SIZE 2
#define QUECTELL_RESPONSE_OF_SMS_CSCS "OK"
#define QUECTELL_RESPONSE_OF_SMS_CSCS_SIZE 2
#define QUECTELL_RESPONSE_OF_SMS_CMGS "+CMGS"
#define QUECTELL_RESPONSE_OF_SMS_CMGS_SIZE 5
//
#define QUECTELL_FLAG_NETWORK_QIOPEN 3
#define QUECTELL_FLAG_NETWORK_QISEND 4
#define QUECTELL_FLAG_NETWORK_QICLOSE 5
#define QUECTELL_FLAG_NETWORK_QIDEACT 6
/*

WE ARE LEFT 10 REGISTER R MORE 

*/

#define SIZE_OF_IP_IN_STR 15
#define SIZE_OF_PORT_IN_STR 5
#define SIZE_OF_FLASH_READ_WRITE_BUFFER
#define SIZE_OF_ESP_READ_WRITE_BUFFER
#define SIZE_OF_QUECTELL_READ_BUFFER 128
#define SIZE_OF_QUECTELL_WRITE_BUFFER 
//

#define FLASH_ADDRESS_WIFI_NAME
#define FLASH_ADDRESS_WIFI_PASSWORD
#define FLASH_ADDRESS_TCP_OR_UDP_FLAG
// MAX 5 DEVICES
#define FLASH_ADDRESS_NUMBER_OF_SERVER
#define FLASH_ADDRESS_SEND_INTERVAL 
#define FLASH_ADDRESS_ONE_IP
#define FLASH_ADDRESS_ONE_PORT
//
#define FLASH_ADDRESS_TWO_IP
#define FLASH_ADDRESS_TWO_PORT
//
#define FLASH_ADDRESS_THREE_IP
#define FLASH_ADDRESS_THREE_PORT
//
#define FLASH_ADDRESS_FOUR_IP
#define FLASH_ADDRESS_FOUR_PORT
//
#define FLASH_ADDRESS_FIVE_IP
#define FLASH_ADDRESS_FIVE_PORT
//
#define FLASH_ADDRESS_BLE_DATA
//
#define COMMAND_ENTER "\r\n"
typedef enum{
	QUECTELL_SMS_PASSIVE,
	QUECTELL_SMS_TIMEOUT,
	QUECTELL_SMS_CMGF,
	QUECTELL_SMS_CSCS,
	QUECTELL_SMS_CMGS,
	QUECTELL_SMS_SEND_OK
}
QuectellSMSSTatusTypeDef;
typedef enum{
	QUECTELL_NETWORK_PASSIVE,
	QUECTELL_NETWORK_TIMEOUT,	
	QUECTELL_NETWORK_QIOPEN,
	QUECTELL_NETWORK_QISEND,
	QUECTELL_NETWORK_QICLOSE,
	QUECTELL_NETWORK_QIDEACT,
	QUECTELL_NETWORK_SEND_OK
}
QuectellNetworkStatusTypeDef;
typedef enum{
	
	ERR_INVALID_DATA,
	ERR_EMPTY_BUFFER,
	ERR_SERVER_CONNECTION_FAIL,
	ERR_WIFI_CONNECTION_FAIL,
	ERR_BLE_CONNECTION_FAIL,
	ERR_FLASH_READ_FAIL,
	ERR_FLASH_WRITE_FAIL,
	ERR_SMS_FAIL
}
ErrorStatusTypedef;
typedef enum{
	SOURCE_ERR_NONE,
	SOURCE_ERR_ESP,
	SOURCE_ERR_QUECTELL,
	SOURCE_ERR_FLASH,
	SOURCE_ERR_PROCESS
}
ErrorSourceFile;
typedef enum{
	SYS_OK       = 0x00U,
	SYS_ERROR    = 0x01U,
	SYS_BUSY     = 0x02U,
	SYS_TIMEOUT  = 0x03U
}
SystemStatusTypedef;
//
typedef struct{
	unsigned char *ucpQuectellDataBuffer;
  unsigned char *ucpQuectellLastSendCommend;
	unsigned char ucActiveDeviceNetwork;
	unsigned char ucNumberOfNetwork;
	unsigned char ucTimeOutGPRS;
	unsigned char ucNumberOfRepeatationGPRS;
	unsigned char ucActiveDeviceToSendNetwork;
	unsigned char ucMessageLenghtGPRS;
	unsigned char ucActiveDeviceSMS;
	unsigned char ucNumberOfSMSUser;	
	unsigned char ucTimeOutSMS;
	unsigned char ucNumberOfRepeatationSMS;		
	unsigned char ucActiveDeviceToSendSMS;
	unsigned char ucMessageLenghtSMS;
	//	
	char caSMSPartner[SMS_MAX_NO][SMS_MSISDN_MAX_NO];
	char caTCPClientIP[NETWORK_MAX_NO][NETWORK_IP_MAX_NO];
	char caTCPClientPort[NETWORK_MAX_NO][NETWORK_PORT_MAX_NO];
	//
	unsigned short usQuectellConnectionIntervalInMS;	
	unsigned short usMoludeWakeUpGpioPin;
	unsigned short usMoludeResetGpioPin;
	unsigned short usModeuleShutDownGpioPin;
	unsigned short usQuectellStatusRegister;
  //
	GPIO_TypeDef *sModeuleWakeUpGpioPort;
	GPIO_TypeDef *sModeuleResetGpioPort;
	GPIO_TypeDef *sModeuleShutDownGpioPort;
	//
	QuectellSMSSTatusTypeDef enSmsStatus;
	QuectellNetworkStatusTypeDef enNetworkStatus;
}
_QUECTELL;
//
typedef struct{
	unsigned char *ucpFlashReadWriteBuffer;
}
_FLASH;
//
typedef struct{
	unsigned char *ucpEspLastSendCommend;
	unsigned char *ucpESPDataSendBuffer;
	unsigned char  ucActiveDeviceBLE;
	unsigned char ucBleAdvTimeoutTime;	
	unsigned char ucTimeOutBLE;
	unsigned char ucNumberOfRepeatationBLE;		
	unsigned char ucMessageLenghtBLE;
	unsigned char ucTimeOutWifi;
	unsigned char ucNumberOfRepeatationWifi;		
	unsigned char ucMessageLenghtWifi;
	
	char *cpWifiName;
	char *cpWifiPassword;
	char *cpWifiPortName;
	char *cpWifiPortIPName;
	char caBleMacAddress[BLE_MAX_NO][BLE_MAC_ADDRESS_MAX_NO];
	char *cpBleDeviceName;
	char *cpBleDevicePassword;
	char *cpBleServiceName;
	char *cpBleCharName;
	char cBleAdvTimeoutTime;

	unsigned short usESPDataSendBufferLenght;
	unsigned short usEspConnectionIntervalInMS;
	unsigned short usESPWifiPasswordLenght;
	unsigned short usESPWifiNameLenght;
}
_ESP;
//
typedef struct{	
	UART_HandleTypeDef  *hpUartESP;
	UART_HandleTypeDef  *hpUartQuectell;
	UART_HandleTypeDef  *hpUartPLC;
	
	unsigned char *ucpUartPLCDataReadyFlag;
	unsigned char *ucpUartPLCPacketFinishedFlag;
	unsigned char *ucpUartPLCTxDataBuffer;
	unsigned char *ucpUartPLCRxDataBuffer;
	unsigned char *ucpUartESPDataBuffer;
	unsigned char *ucpUartQuectellTxDataBuffer;
	unsigned char *ucpUartQuectellRxDataBuffer;
	unsigned char *ucpUartQuectellGPRSDataBuffer;
	unsigned char *ucpUartQuectellSMSDataBuffer;
	unsigned char *ucpUartBleDataBuffer;
	unsigned char *ucpUartBleServiceName;
	unsigned char *ucpUartBleCharName;
	unsigned char *ucpUartWifiDataBuffer;
	unsigned char *ucpUartWifiIpName;
	unsigned char *ucpUartWifiPortName;

	unsigned short usUartEspLenght;
	unsigned short usUartQuectellLenght;
}
_UART;

typedef struct{
	unsigned char *ucpTimer1Counter;
	unsigned char *ucpTimer2Counter;
	unsigned char *ucpTimer4Counter;
}_TIMER;

typedef struct{
	unsigned char ucaDigitalOutput[4];
	unsigned char ucaDigitalInput[4];	
	unsigned char ucaLedStatus[2];
	
	char cRotaryOrientation;
	
	unsigned short usaAnalogOutputExternal[2];
	
	unsigned int uiaAnalogOutputInternal[2];	
	unsigned int uiaAnalogInput[4];	
	
	int iRotaryCounter;	
}_PERIPHERAL;
	
//
typedef struct{
 	unsigned int iErrorLine;
	ErrorStatusTypedef enError;
	ErrorSourceFile enErrPage;
	SystemStatusTypedef enSysStatus;
	
	_QUECTELL sQuectell;
	_FLASH	sFlash;
	_ESP sEsp;
	_UART sUart;
	_PERIPHERAL	 sPeripheral;
	_TIMER sTimer;
}
_SYS;

typedef union{
	float f;
	int i;
	unsigned int ui;
	unsigned short us[2];
	unsigned char uc[4];
	char c[4];
}unionTypeDef;

void mainCheckUartHartBeat( UART_HandleTypeDef *huart);
extern _SYS sSytemVar;
extern _UART sUart;
extern unionTypeDef uParser;
#endif



