#ifndef __ESP__
#define __ESP__
#include "main.h"
#include "user_definitions.h"

#define ESP_COMMAND_SCAN "AT+SCAN"
#define ESP_COMMAND_SEND_PE "AT+SEND="
#define ESP_COMMAND_WIFI_CONNECT "AT+CWJAP="
#define ESP_COMMAND_CONNECT_PORT "AT+CONN="
#define ESP_COMMAND_RESET "AT+RST"
#define ESP_LENGHT_OF_IP_ADDRESS 15
#define ESP_LENGHT_OF_PORT_ADDRESS 5

typedef enum{
	
	ESP_INVALID_DATA=0,
	ESP_EMPTY_BUFFER=1,
	ESP_SERVER_CONNECTION_FAIL=2,
	ESP_WIFI_CONNECTION_FAIL=3,
}ESPErrorStatusTypedef;

typedef enum{
	ESP_OK       = 0x00U,
	ESP_ERROR    = 0x01U,
	ESP_BUSY     = 0x02U,

	ESP_TIMEOUT  = 0x03U
}ESPStatusTypedef;

ESPStatusTypedef espModuleReset(void);
ESPStatusTypedef espSendData(unsigned char *data , unsigned short data_lenght);
ESPStatusTypedef espConnectWifi(_ESP *struct_esp );
ESPStatusTypedef espConnectTCP(_ESP *struct_esp );


#endif
