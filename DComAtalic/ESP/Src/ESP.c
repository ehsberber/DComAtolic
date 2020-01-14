#include "ESP.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

ESPStatusTypedef espModuleReset(void){
	espSendData((unsigned char*)ESP_COMMAND_RESET,strlen((char*)ESP_COMMAND_RESET));
	return ESP_OK;
}
ESPStatusTypedef espSendData(unsigned char *data , unsigned short data_lenght){
	HAL_UART_Transmit(sUart.hpUartESP,data,data_lenght,data_lenght*5);
	HAL_UART_Transmit(sUart.hpUartESP,(unsigned char*)COMMAND_ENTER,2,100);
	return ESP_OK;
}
ESPStatusTypedef espConnectWifi(_ESP *struct_esp ){
	unsigned short tmpDataLenght=0;
	
	memcpy(struct_esp->ucpESPDataSendBuffer,(char*)ESP_COMMAND_WIFI_CONNECT,strlen((char*)ESP_COMMAND_WIFI_CONNECT));
	memcpy(&struct_esp->ucpESPDataSendBuffer[strlen((char*)ESP_COMMAND_WIFI_CONNECT)],struct_esp->cpWifiName,struct_esp->usESPWifiNameLenght);
	memcpy(&struct_esp->ucpESPDataSendBuffer[strlen((char*)ESP_COMMAND_WIFI_CONNECT)+struct_esp->usESPWifiNameLenght],struct_esp->cpWifiPassword,
				 struct_esp->usESPWifiPasswordLenght);
	tmpDataLenght=strlen((char*)ESP_COMMAND_WIFI_CONNECT)+struct_esp->usESPWifiNameLenght+struct_esp->usESPWifiPasswordLenght;
	espSendData(struct_esp->ucpESPDataSendBuffer,tmpDataLenght);
		return ESP_OK;
}
ESPStatusTypedef espConnectTCP(_ESP *struct_esp ){
	unsigned short tmpDataLenght=0;
	
	memcpy(struct_esp->ucpESPDataSendBuffer,(char*)ESP_COMMAND_CONNECT_PORT,strlen((char*)ESP_COMMAND_CONNECT_PORT));
	memcpy(&struct_esp->ucpESPDataSendBuffer[strlen((char*)ESP_COMMAND_CONNECT_PORT)],struct_esp->cpWifiPortIPName,ESP_LENGHT_OF_IP_ADDRESS);
	memcpy(&struct_esp->ucpESPDataSendBuffer[strlen((char*)ESP_COMMAND_CONNECT_PORT)+ESP_LENGHT_OF_IP_ADDRESS],struct_esp->cpWifiPortName,
				 ESP_LENGHT_OF_PORT_ADDRESS);
	tmpDataLenght=strlen((char*)ESP_COMMAND_CONNECT_PORT)+ESP_LENGHT_OF_IP_ADDRESS+ESP_LENGHT_OF_PORT_ADDRESS;
	espSendData(struct_esp->ucpESPDataSendBuffer,tmpDataLenght);
	return ESP_OK;
}
