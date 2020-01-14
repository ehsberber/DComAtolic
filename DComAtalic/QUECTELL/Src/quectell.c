#include "quectell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*

IF TIMOUT CHANGE INCREMENT DEVICE NO
CHECK SMS FIRST IF SMS RECIEVED EMPTY ARAY 
SMS FORMAT LAST DECIMAL IS THE NUMBER OF LENGHT IN STRING TAKE ARRAY PROCESS EMPTY BUFFER


*/
unsigned char localCheckQuectellRegisterBit(unsigned short register_name, unsigned char flag_name){
		return (register_name>>flag_name)&1U;
}

unsigned char localCheckResponseFromServer(unsigned char *commend_for_comparing, unsigned short data_len){
return 0;
}
void localEmptyAnArry(unsigned char *array, unsigned short size){
	int tmpC=0;
	for(tmpC=0;tmpC<size;tmpC++)
		array[tmpC]=0;
}
void localResetUartBufferAndInterrupt(){
		localEmptyAnArry(sUart.ucpUartQuectellRxDataBuffer,SIZE_OF_QUECTELL_READ_BUFFER);
		sUart.hpUartQuectell->pRxBuffPtr=
	  sUart.hpUartQuectell->pRxBuffPtr-(sUart.hpUartQuectell->RxXferSize-sUart.hpUartQuectell->RxXferCount);
		sUart.hpUartQuectell->RxXferCount=SIZE_OF_QUECTELL_READ_BUFFER;
		sUart.hpUartQuectell->RxXferSize=SIZE_OF_QUECTELL_READ_BUFFER;	
}

///
QuectellStatusTypedef quecCheckSMS(_QUECTELL *struct_quectell){
// TODO SMS Process
return QECTEL_OK;
}
QuectellStatusTypedef quecSMSProcess(_QUECTELL *struct_quectell){
	static unsigned char g_ucActiveDevice;
		if(localCheckQuectellRegisterBit(struct_quectell->ucActiveDeviceSMS,g_ucActiveDevice)==FLAG_SET){
				struct_quectell->ucActiveDeviceToSendSMS=g_ucActiveDevice;
				quecSetAndSendSMS(struct_quectell);				
			/*
				@todo
				// if ok g_ucActiveDevice is next device
			
				// if time out happens g_ucActiveDevice is default zero device
			*/			
		}else{
			g_ucActiveDevice=(g_ucActiveDevice+1)%SMS_MAX_NO;
		}		
		return QECTEL_OK;
}
QuectellStatusTypedef quecServerProcess(_QUECTELL *struct_quectell){
return QECTEL_OK;
}
QuectellStatusTypedef quecProcess(_QUECTELL *struct_quectell){
	quecCheckSMS(struct_quectell);
	quecSMSProcess(struct_quectell);
	quecServerProcess(struct_quectell);
	return QECTEL_OK;
}

QuectellStatusTypedef quecSetAndSendNetworkInfo(_QUECTELL * struct_quectell){
	unsigned char tmpC;
	static unsigned char connectionFailCounter=0;
	for(tmpC=0;tmpC<struct_quectell->ucNumberOfNetwork;tmpC++){
	//AT+QIOPEN="TCP","116.226.39.202","7007" 
				// adjust IP no 
				strcpy((char*)&struct_quectell->ucpQuectellDataBuffer[0],"AT+QIOPEN=\"TCP\",\"");
				strcat((char*)struct_quectell->ucpQuectellDataBuffer,struct_quectell->caTCPClientIP[tmpC]);
				strcat((char*)struct_quectell->ucpQuectellDataBuffer,"\"");
				//adjust port no 
				strcat((char*)struct_quectell->ucpQuectellDataBuffer,",\"");
				strcat((char*)struct_quectell->ucpQuectellDataBuffer,struct_quectell->caTCPClientPort[tmpC]);
				strcat((char*)struct_quectell->ucpQuectellDataBuffer,"\"");
				// send first network config to quectell
				quecSendMessageToModule(struct_quectell->ucpQuectellDataBuffer,strlen((char*)struct_quectell->ucpQuectellDataBuffer));
				HAL_Delay(2000);
				// send variables
				strcpy((char*)&struct_quectell->ucpQuectellDataBuffer[0],"AT+QISEND");
				quecSendMessageToModule(struct_quectell->ucpQuectellDataBuffer,strlen((char*)struct_quectell->ucpQuectellDataBuffer));
				HAL_Delay(1000);
				/// CHECK CONNECTION FAIL
				if((strncmp((char*)sUart.ucpUartQuectellTxDataBuffer,(const char*)"AT+QISEND\r\r\nERROR\r\n",19))){
							connectionFailCounter++;
							if(connectionFailCounter>=struct_quectell->ucNumberOfNetwork-1){						
								return QECTEL_ERROR;
							}								
				}
				// send variables from uart2 strcutre data package for exp
				//FAFAFFFFFFFFFFAAAAFBAAFFDFDFAFAF
				strcpy((char*)struct_quectell->ucpQuectellDataBuffer,(char*)(char*)sUart.ucpUartQuectellRxDataBuffer);
				HAL_Delay(400);
				strcat((char*)struct_quectell->ucpQuectellDataBuffer,"\x1A");
				
				quecSendMessageToModule(struct_quectell->ucpQuectellDataBuffer,strlen((char*)struct_quectell->ucpQuectellDataBuffer));
				HAL_Delay(1000);
				// close network
				// Deactivate GPRS/CSD context AT+QUDEACT
				strcpy((char*)&struct_quectell->ucpQuectellDataBuffer[0],"AT+QICLOSE");
				quecSendMessageToModule(struct_quectell->ucpQuectellDataBuffer,strlen((char*)struct_quectell->ucpQuectellDataBuffer));
				HAL_Delay(200);
				strcpy((char*)&struct_quectell->ucpQuectellDataBuffer[0],"AT+QIDEACT");
				quecSendMessageToModule(struct_quectell->ucpQuectellDataBuffer,strlen((char*)struct_quectell->ucpQuectellDataBuffer));	
				HAL_Delay(200);
				// reset array values for next iterations 
				localEmptyAnArry((unsigned char*)(char*)struct_quectell->ucpQuectellDataBuffer,150);	
				// every half second connect one server send information to server 
				HAL_Delay(500);				
	}
	return	QECTEL_OK;
}
QuectellStatusTypedef quecSetAndSendSMS(_QUECTELL * struct_quectell){
	// this function send SMS information to user recursively
	// the amount of sms no is setted from cnfiguration 			
				// set SMS message format as text mode
	if(struct_quectell->enSmsStatus==QUECTELL_FLAG_SMS_CMGF){	
			localResetUartBufferAndInterrupt();
			strcpy((char*)struct_quectell->ucpQuectellDataBuffer,"AT+CMGF=1");	
			quecSendMessageToModule(struct_quectell->ucpQuectellDataBuffer,strlen((const char*)struct_quectell->ucpQuectellDataBuffer));
			//struct_quectell->enNetworkStatus=QUECTELL_FLAG_SMS_CSCS;			
			return QECTEL_OK;
		}
	else if(struct_quectell->enSmsStatus==QUECTELL_FLAG_SMS_CSCS ){
		unsigned char t_ucVal= localCheckResponseFromServer((unsigned char*)QUECTELL_RESPONSE_OF_SMS_CMGF,QUECTELL_RESPONSE_OF_SMS_CMGF_SIZE);
		if(t_ucVal==FLAG_SET){
			localResetUartBufferAndInterrupt();
			// set character set as GSM which is used by the TE
			strcpy((char*)&struct_quectell->ucpQuectellDataBuffer[0],"AT+CSCS=\"GSM\"");
			quecSendMessageToModule(struct_quectell->ucpQuectellDataBuffer,strlen((const char*)struct_quectell->ucpQuectellDataBuffer));
			//struct_quectell->enNetworkStatus=QUECTELL_FLAG_SMS_CMGS;			
			return QECTEL_OK;
		}
	}
	// SET THE SMS AND SEND VAR 	
	strcpy((char*)&struct_quectell->ucpQuectellDataBuffer[0],"AT+CMGS=\"");
	strcat((char*)&struct_quectell->ucpQuectellDataBuffer,struct_quectell->caSMSPartner[struct_quectell->ucActiveDeviceToSendSMS]);
	strcat((char*)&struct_quectell->ucpQuectellDataBuffer,"\"");
	// send firt parameters 
	quecSendMessageToModule(struct_quectell->ucpQuectellDataBuffer,strlen((char*)&struct_quectell->ucpQuectellDataBuffer));
	// set sms 
	strcpy((char*)&struct_quectell->ucpQuectellDataBuffer,(char*)sUart.ucpUartQuectellTxDataBuffer);
	strcat((char*)&struct_quectell->ucpQuectellDataBuffer,"\x1A");
	quecSendMessageToModule(struct_quectell->ucpQuectellDataBuffer,strlen((char*)&struct_quectell->ucpQuectellDataBuffer));
	return QECTEL_OK;
}
QuectellStatusTypedef quecShutModuleDown(_QUECTELL * struct_quectell){
	HAL_GPIO_WritePin(struct_quectell->sModeuleShutDownGpioPort,struct_quectell->usModeuleShutDownGpioPin,GPIO_PIN_SET);
	return QECTEL_OK;
}
QuectellStatusTypedef quecResetModule(_QUECTELL * struct_quectell){
	HAL_GPIO_WritePin(struct_quectell->sModeuleResetGpioPort,struct_quectell->usMoludeResetGpioPin,GPIO_PIN_SET);
	HAL_Delay(200);
	HAL_GPIO_WritePin(struct_quectell->sModeuleResetGpioPort,struct_quectell->usMoludeResetGpioPin,GPIO_PIN_RESET);
	return QECTEL_OK;
}
QuectellStatusTypedef quecWakeModuleUp(_QUECTELL * struct_quectell){
	HAL_GPIO_WritePin(struct_quectell->sModeuleWakeUpGpioPort,struct_quectell->usMoludeWakeUpGpioPin,GPIO_PIN_SET);
	return QECTEL_OK;
}
QuectellStatusTypedef quecSendMessageToModule(unsigned char *data,unsigned short data_lenght){
	HAL_UART_Transmit(sUart.hpUartQuectell,data,data_lenght,data_lenght*5);
	HAL_UART_Transmit(sUart.hpUartQuectell,(unsigned char*)COMMAND_ENTER,2,100);
	return QECTEL_OK;
}

