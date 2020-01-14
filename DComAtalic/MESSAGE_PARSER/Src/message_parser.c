#include "message_parser.h"
#include <stdlib.h>
#include <string.h>
//TODO check the index place insrt new ones
//DEFINES
#define INDEX_NUMBER_OF_SIZE 1
#define INDEX_NUMBER_OF_ADV_TIME 1
#define INDEX_NUMBER_OF_BLE_DEVICE_NAME 1
#define INDEX_NUMBER_OF_BLE_DEIVE_PASSWORD 2
#define INDEX_NUMBER_OF_WIFI_NAME 1
#define INDEX_NUMBER_OF_WIFI_PASSWORD 2
//FUNCTIONS
void sendVariablesToMasterDevice(unsigned short size,unsigned int time_out){
	HAL_UART_Transmit(sSytemVar.sUart.hpUartPLC,sSytemVar.sUart.ucpUartPLCTxDataBuffer,size,time_out);
}
void parseUnsingedIntToCharPointer(unsigned char *p, unsigned int data){
		p[0] = 	data & 0xFF;
    p[1] = (data>>8) & 0xFF;
    p[2] = (data>>16) & 0xFF;
    p[3] = (data>>24) & 0xFF;
}
ParserTypedef gprsProcess(unsigned char opcode,unsigned char *message){
	if((opcode & IFSTATIC) ==1){
		return gprsStaticProcess(opcode,message);
	}else{
		return gprsDynamicProcess(opcode,message);
	}
}	
ParserTypedef gprsStaticProcess(unsigned char opcode,unsigned char *message){
	int IPCounter=0;
	int i=0;
	int INDEX_NUMBER_OF_PORT;
	IPCounter=message[INDEX_NUMBER_OF_SIZE];
	if(IPCounter>NETWORK_IP_MAX_NO)
		IPCounter=NETWORK_IP_MAX_NO;
	sSytemVar.sQuectell.ucActiveDeviceNetwork=IPCounter;
	INDEX_NUMBER_OF_PORT=INDEX_NUMBER_OF_SIZE+(IPCounter*15);
	for(i=0;i<IPCounter;i++){
		int j=0;
		for(j=0;j<15;j++){
			sSytemVar.sQuectell.caTCPClientIP[i][j]=message[(15*i)+(INDEX_NUMBER_OF_SIZE+j)];
		}
		for(j=0;j<5;j++)
			sSytemVar.sQuectell.caTCPClientIP[i][j]=message[(5*i)+(INDEX_NUMBER_OF_PORT+j)];
	}
	switch((int)opcode){
	case GPRS_StaticDoNotSaveParamsToFlash:
			//TODO Save PARAMS on flash 
		break;	
	case GPRS_StaticSaveParamsToFlash:
		// CURRENTLY DO NOTHING 
		break;		
	default:
		return OPCODE_WRONG;
	}
	return OPCODE_OK;
}
ParserTypedef gprsDynamicProcess(unsigned char opcode,unsigned char *message){	
	switch((int)opcode){
		case GPRS_DynamicRepeatationActiveTimeOutPassive:
			sSytemVar.sQuectell.ucNumberOfRepeatationGPRS=message[1];
			if(sSytemVar.sQuectell.ucNumberOfRepeatationGPRS>GPRS_MAX_REPEATATION_NO)
				sSytemVar.sQuectell.ucNumberOfRepeatationGPRS=GPRS_MAX_REPEATATION_NO;
			sSytemVar.sQuectell.ucMessageLenghtGPRS=message[2];
			int i=0;
			for(i=0;i<sSytemVar.sQuectell.ucMessageLenghtGPRS;i++)
				sSytemVar.sUart.ucpUartQuectellGPRSDataBuffer[i]=message[(3)+i];		
			break;
		case GPRS_DynamicRepeatationPassiveTimeOutActive:
			sSytemVar.sQuectell.ucTimeOutGPRS=message[1];
			if(sSytemVar.sQuectell.ucTimeOutGPRS>GPRS_MAX_TIMEIOT_TIME)
				sSytemVar.sQuectell.ucTimeOutGPRS=GPRS_MAX_TIMEIOT_TIME;	
			sSytemVar.sQuectell.ucMessageLenghtGPRS=message[2];
			for(i=0;i<sSytemVar.sQuectell.ucMessageLenghtGPRS;i++)
				sSytemVar.sUart.ucpUartQuectellGPRSDataBuffer[i]=message[(3)+i];	
			break;
		case GPRS_DynamicRepeatationActiveTimeOutActive:
			sSytemVar.sQuectell.ucNumberOfRepeatationGPRS=message[1];
			if(sSytemVar.sQuectell.ucNumberOfRepeatationGPRS>GPRS_MAX_REPEATATION_NO)
				sSytemVar.sQuectell.ucNumberOfRepeatationGPRS=GPRS_MAX_REPEATATION_NO;
					sSytemVar.sQuectell.ucTimeOutGPRS=message[2];
			if(sSytemVar.sQuectell.ucTimeOutGPRS>GPRS_MAX_TIMEIOT_TIME)
				sSytemVar.sQuectell.ucTimeOutGPRS=GPRS_MAX_TIMEIOT_TIME;
			sSytemVar.sQuectell.ucMessageLenghtGPRS=message[3];
			for(i=0;i<sSytemVar.sQuectell.ucMessageLenghtGPRS;i++)
				sSytemVar.sUart.ucpUartQuectellGPRSDataBuffer[i]=message[(4)+i];				
			break;
		default:
			return OPCODE_WRONG;
	}
	return OPCODE_OK;
}

ParserTypedef smsProcess(unsigned char opcode,unsigned char *message){
	if((opcode & IFSTATIC) ==1){
		return smsStaticProcess(opcode,message);
	}else{
		return smsDynamicProcess(opcode,message);
	}
}
ParserTypedef smsStaticProcess(unsigned char opcode,unsigned char *message){
	unsigned char SMSCounter=0;
	int i=0;
	char *token;
	SMSCounter=message[INDEX_NUMBER_OF_SIZE];
	if(SMSCounter>SMS_MAX_NO)
		SMSCounter=SMS_MAX_NO;
	sSytemVar.sQuectell.ucActiveDeviceSMS=SMSCounter;
		token=strtok((char *)&message[INDEX_NUMBER_OF_SIZE+1],SMS_DELIMETER);
	while(token!=NULL){
		if(i==SMS_MAX_NO)
			break;
		if(strlen(token)<=SMS_MSISDN_MAX_NO)
			strcpy(sSytemVar.sQuectell.caSMSPartner[i],token);
		i++;
			token=strtok(NULL,SMS_DELIMETER);
	}
	switch(opcode){
	case SMS_StaticDoNotSaveParamsToFlash:
		// SAVE PARAMS ON  FLASH
		break;	
	case SMS_StaticSaveParamsToFlash:
		// DO NOTHING 
		break;
	default:
		return OPCODE_WRONG;	
	}
	return OPCODE_OK;
 }
ParserTypedef smsDynamicProcess(unsigned char opcode,unsigned char *message){
	switch((int)opcode){
	case SMS_DynamicRepeatationPassiveTimeOutActive :
		sSytemVar.sQuectell.ucNumberOfRepeatationSMS=message[1];
		if(sSytemVar.sQuectell.ucNumberOfRepeatationSMS>SMS_MAX_REPEATATION_NO)
			sSytemVar.sQuectell.ucNumberOfRepeatationSMS=SMS_MAX_REPEATATION_NO;
		sSytemVar.sQuectell.ucMessageLenghtSMS=message[2];
		int i=0;
		for(i=0;i<sSytemVar.sQuectell.ucMessageLenghtSMS;i++)
			sSytemVar.sUart.ucpUartQuectellGPRSDataBuffer[i]=message[(3)+i];	
		break;
	case SMS_DynamicRepeatationActiveTimeOutPassive:
		sSytemVar.sQuectell.ucTimeOutSMS=message[1];
		if(sSytemVar.sQuectell.ucTimeOutSMS>SMS_MAX_TIMEIOT_TIME)
			sSytemVar.sQuectell.ucTimeOutSMS=SMS_MAX_TIMEIOT_TIME;	
		sSytemVar.sQuectell.ucMessageLenghtSMS=message[2];
		for(i=0;i<sSytemVar.sQuectell.ucMessageLenghtSMS;i++)
			sSytemVar.sUart.ucpUartQuectellSMSDataBuffer[i]=message[(3)+i];	
		break;
	case SMS_DynamicRepeatationActiveTimeOutActive:
		sSytemVar.sQuectell.ucNumberOfRepeatationSMS=message[1];
		if(sSytemVar.sQuectell.ucNumberOfRepeatationSMS>SMS_MAX_REPEATATION_NO)
			sSytemVar.sQuectell.ucNumberOfRepeatationSMS=SMS_MAX_REPEATATION_NO;
				sSytemVar.sQuectell.ucTimeOutSMS=message[2];
		if(sSytemVar.sQuectell.ucTimeOutSMS>SMS_MAX_TIMEIOT_TIME)
			sSytemVar.sQuectell.ucTimeOutSMS=SMS_MAX_TIMEIOT_TIME;
		sSytemVar.sQuectell.ucMessageLenghtSMS=message[3];
		for(i=0;i<sSytemVar.sQuectell.ucMessageLenghtSMS;i++)
			sSytemVar.sUart.ucpUartQuectellSMSDataBuffer[i]=message[(4)+i];
		break;
	default:
		return OPCODE_WRONG;	
	}
	return OPCODE_OK;
}

ParserTypedef bleProcess(unsigned char opcode,unsigned char *message){
	if((opcode & IFSTATIC) ==1){
		if((opcode & IFMASTER) ==1){
			return bleMasterStaticProcess(opcode,message);
		}else{
			return bleSlaveStaticProcess(opcode,message);
		}			
	}else{
		if((opcode & IFMASTER) ==1){
			return bleMasterDynamicProcess(opcode,message);
		}else{
			return bleSlaveDynamicProcess(opcode,message);
		}	
	}	
}
ParserTypedef bleMasterStaticProcess(unsigned char opcode,unsigned char *message){
	unsigned char BLECounter=0;
	int i=0;
	char *token;
	BLECounter=message[INDEX_NUMBER_OF_SIZE];
	if(BLECounter>BLE_MAX_NO)
		BLECounter=BLE_MAX_NO;
	sSytemVar.sEsp.ucActiveDeviceBLE=BLECounter;
	token=strtok((char *)&message[INDEX_NUMBER_OF_SIZE+1],BLE_DELIMETER);
	while(token!=NULL){
		if(i==BLE_MAX_NO)
			break;
		if(strlen(token)<=BLE_MAC_ADDRESS_MAX_NO)
			strcpy(sSytemVar.sEsp.caBleMacAddress[i],token);
		i++;
			token=strtok(NULL,BLE_DELIMETER);
		}
	switch(opcode){
	case BLE_MasterStaticDoNotSaveParamsToFlash:
		//TODO SAVE
		break;	
	case BLE_MasterStaticSaveParamsToFlash:
		// DO NOTHING
		break;	
	default:
		return OPCODE_WRONG;	
	}
	return OPCODE_OK;
}
ParserTypedef bleMasterDynamicProcess(unsigned char opcode,unsigned char *message){
	unsigned char lenghtOfBleSerice;
	unsigned char lenghtOfBleCharacter;
	int i=0;
	switch(opcode){
	case BLE_MasterDynamicRepeatationPassiveTimeOutActive:
		lenghtOfBleSerice=message[1];
		if(lenghtOfBleSerice>BLE_MAX_LENGHT_SERVICE)
			lenghtOfBleSerice=BLE_MAX_LENGHT_SERVICE;
		lenghtOfBleCharacter=message[2];
		if(lenghtOfBleCharacter>BLE_MAX_LENGHT_CHAR)
			lenghtOfBleCharacter=BLE_MAX_LENGHT_CHAR;
		sSytemVar.sEsp.ucTimeOutBLE=message[3];
		sSytemVar.sEsp.ucMessageLenghtBLE=message[4];
		for(i=0;i<lenghtOfBleSerice;i++)
			sSytemVar.sUart.ucpUartBleServiceName[i]=message[(5)+i];	
		for(i=0;i<lenghtOfBleCharacter;i++)
			sSytemVar.sUart.ucpUartBleCharName[i]=message[(lenghtOfBleSerice+6)+i];	
		for(i=0;i<sSytemVar.sEsp.ucMessageLenghtBLE;i++)
			sSytemVar.sUart.ucpUartBleDataBuffer[i]=message[(lenghtOfBleCharacter+lenghtOfBleSerice+7)+i];	
		break;
	case BLE_MasterDynamicRepeatationActiveTimeOutPassive:
		lenghtOfBleSerice=message[1];
		if(lenghtOfBleSerice>BLE_MAX_LENGHT_SERVICE)
			lenghtOfBleSerice=BLE_MAX_LENGHT_SERVICE;
		lenghtOfBleCharacter=message[2];
		if(lenghtOfBleCharacter>BLE_MAX_LENGHT_CHAR)
			lenghtOfBleCharacter=BLE_MAX_LENGHT_CHAR;
		sSytemVar.sEsp.ucNumberOfRepeatationBLE=message[3];
		sSytemVar.sEsp.ucMessageLenghtBLE=message[4];
		for(i=0;i<lenghtOfBleSerice;i++)
			sSytemVar.sUart.ucpUartBleServiceName[i]=message[(4)+i];	
		for(i=0;i<lenghtOfBleCharacter;i++)
			sSytemVar.sUart.ucpUartBleCharName[i]=message[(lenghtOfBleSerice+5)+i];	
		for(i=0;i<sSytemVar.sEsp.ucMessageLenghtBLE;i++)
			sSytemVar.sUart.ucpUartBleDataBuffer[i]=message[(lenghtOfBleCharacter+lenghtOfBleSerice+5)+i];	
		break;
	case  BLE_MasterDynamicRepeatationActiveTimeOutActive:
		lenghtOfBleSerice=message[1];
		if(lenghtOfBleSerice>BLE_MAX_LENGHT_SERVICE)
			lenghtOfBleSerice=BLE_MAX_LENGHT_SERVICE;
		lenghtOfBleCharacter=message[2];
		if(lenghtOfBleCharacter>BLE_MAX_LENGHT_CHAR)
			lenghtOfBleCharacter=BLE_MAX_LENGHT_CHAR;
		sSytemVar.sEsp.ucNumberOfRepeatationBLE=message[3];
		sSytemVar.sEsp.ucTimeOutBLE=message[4];
		sSytemVar.sEsp.ucMessageLenghtBLE=message[5];
		for(i=0;i<lenghtOfBleSerice;i++)
			sSytemVar.sUart.ucpUartBleServiceName[i]=message[(6)+i];	
		for(i=0;i<lenghtOfBleCharacter;i++)
			sSytemVar.sUart.ucpUartBleCharName[i]=message[(lenghtOfBleSerice+6)+i];	
		for(i=0;i<sSytemVar.sEsp.ucMessageLenghtBLE;i++)
			sSytemVar.sUart.ucpUartBleDataBuffer[i]=message[(lenghtOfBleCharacter+lenghtOfBleSerice+3)];
		break;
	default:
		return OPCODE_WRONG;	
	}
	return OPCODE_OK;
}
ParserTypedef bleSlaveStaticProcess(unsigned char opcode,unsigned char *message){	
	sSytemVar.sEsp.ucBleAdvTimeoutTime=message[INDEX_NUMBER_OF_ADV_TIME];
	unsigned char lenghtOfBLEName=message[INDEX_NUMBER_OF_BLE_DEVICE_NAME];
	unsigned char lenghtOfBLEPassword= message[INDEX_NUMBER_OF_BLE_DEIVE_PASSWORD];
	int i=0;
	if((lenghtOfBLEName > BLE_MAX_LENGHT_NAME) || (lenghtOfBLEPassword > BLE_MAX_LENGHT_PASSWORD))
		return OPCODE_EXCEED_LIMIT;
	for(i=0;i<lenghtOfBLEName;i++){
		sSytemVar.sEsp.cpBleDeviceName[i]=message[INDEX_NUMBER_OF_BLE_DEVICE_NAME+i];
	}
	for(i=0;i<lenghtOfBLEPassword;i++){
		sSytemVar.sEsp.cpBleDevicePassword[i]=message[INDEX_NUMBER_OF_BLE_DEIVE_PASSWORD+INDEX_NUMBER_OF_BLE_DEVICE_NAME+i];
	}
	switch(opcode){
	case BLE_SlaveStaticDoNotSaveParamsToFlash:
		// TODO
		break;	
	case BLE_SlaveStaticSaveParamsToFlash:
		// DO NOTHING 
		break;	
	default:
		return OPCODE_WRONG;	
	}
	return OPCODE_OK;
}
ParserTypedef bleSlaveDynamicProcess(unsigned char opcode,unsigned char *message){
	switch(opcode){
	case BLE_SlaveDynamicRepeatationActiveTimeOutActive:
		break;
	case BLE_SlaveDynamicRepeatationActiveTimeOutPassive:
		break;
	case BLE_SlaveDynamicRepeatationPassiveTimeOutActive:
		break;
	default:
		return OPCODE_WRONG;	
	}
	bleMasterDynamicProcess(opcode,message);//becuase its same with master/slave
	return OPCODE_OK;
}


ParserTypedef wifiProcess(unsigned char opcode,unsigned char *message){
	if((opcode & IFSTATIC) ==1){
		if((opcode & IFMASTER) ==1){
			return wifiMasterStaticProcess(opcode,message);
		}else{
			return wifiSlaveStaticProcess(opcode,message);
		}			
	}else{
		if((opcode & IFMASTER) ==1){
			return wifiMasterDynamicProcess(opcode,message);
		}else{
			return wifiSlaveDynamicProcess(opcode,message);
		}	
	}	
}
ParserTypedef wifiMasterStaticProcess(unsigned char opcode,unsigned char *message){
	sSytemVar.sEsp.cBleAdvTimeoutTime=message[INDEX_NUMBER_OF_ADV_TIME];
	unsigned char lenghtOfWifiName=message[INDEX_NUMBER_OF_WIFI_NAME];
	unsigned char lenghtOfWifiPassword= message[INDEX_NUMBER_OF_WIFI_PASSWORD];
	int i=0;
	if((lenghtOfWifiName > WIFI_MAX_LENGHT_NAME) || (lenghtOfWifiPassword > WIFI_MAX_LENGHT_PASSWORD))
		return OPCODE_EXCEED_LIMIT;
	for(i=0;i<lenghtOfWifiName;i++){
		sSytemVar.sEsp.cpWifiName[i]=message[INDEX_NUMBER_OF_WIFI_NAME+i];
	}
	for(i=0;i<lenghtOfWifiPassword;i++){
		sSytemVar.sEsp.cpWifiPassword[i]=message[INDEX_NUMBER_OF_WIFI_PASSWORD+INDEX_NUMBER_OF_WIFI_NAME+i];
	}
	switch(opcode){
	case WIFI_MasterStaticDoNotSaveParamsToFlash:
		//
		break;	
	case WIFI_MasterStaticSaveParamsToFlash:
		//
		break;	
	default:
		return OPCODE_WRONG;	
	}
	return OPCODE_OK;
}
ParserTypedef wifiMasterDynamicProcess(unsigned char opcode,unsigned char *message){
	unsigned char lenghtOfWifiIP;
	unsigned char lenghtOfWifiPort;
	int i=0;
	switch(opcode){
	case WIFI_MasterDynamicRepeatationPassiveTimeOutActive:
		lenghtOfWifiIP=message[1];
		if(lenghtOfWifiIP>WIFI_MAX_LENGHT_IP)
			lenghtOfWifiIP=WIFI_MAX_LENGHT_IP;
		lenghtOfWifiPort=message[2];
		if(lenghtOfWifiPort>WIFI_MAX_LENGHT_PORT)
			lenghtOfWifiPort=WIFI_MAX_LENGHT_PORT;
		sSytemVar.sEsp.ucTimeOutWifi=message[3];
		sSytemVar.sEsp.ucMessageLenghtWifi=message[4];
		for(i=0;i<lenghtOfWifiIP;i++)
			sSytemVar.sUart.ucpUartWifiIpName[i]=message[(5)+i];	
		for(i=0;i<lenghtOfWifiPort;i++)
			sSytemVar.sUart.ucpUartWifiPortName[i]=message[(lenghtOfWifiIP+6)+i];	
		for(i=0;i<sSytemVar.sEsp.ucMessageLenghtBLE;i++)
			sSytemVar.sUart.ucpUartBleDataBuffer[i]=message[(lenghtOfWifiPort+lenghtOfWifiIP+7)+i];
		break;
	case WIFI_MasterDynamicRepeatationActiveTimeOutPassive:
		lenghtOfWifiIP=message[1];
		if(lenghtOfWifiIP>WIFI_MAX_LENGHT_IP)
			lenghtOfWifiIP=WIFI_MAX_LENGHT_IP;
		lenghtOfWifiPort=message[2];
		if(lenghtOfWifiPort>WIFI_MAX_LENGHT_PORT)
			lenghtOfWifiPort=WIFI_MAX_LENGHT_PORT;
		sSytemVar.sEsp.ucNumberOfRepeatationWifi=message[3];
		sSytemVar.sEsp.ucMessageLenghtWifi=message[4];
		for(i=0;i<lenghtOfWifiIP;i++)
			sSytemVar.sUart.ucpUartWifiIpName[i]=message[(4)+i];	
		for(i=0;i<lenghtOfWifiPort;i++)
			sSytemVar.sUart.ucpUartWifiPortName[i]=message[(lenghtOfWifiPort+5)+i];	
		for(i=0;i<sSytemVar.sEsp.ucMessageLenghtWifi;i++)
			sSytemVar.sUart.ucpUartWifiDataBuffer[i]=message[(lenghtOfWifiIP+lenghtOfWifiPort+5)+i];			
		break;
	case WIFI_MasterDynamicRepeatationActiveTimeOutActive :
		lenghtOfWifiIP=message[1];
		if(lenghtOfWifiIP>WIFI_MAX_LENGHT_IP)
			lenghtOfWifiIP=WIFI_MAX_LENGHT_IP;
		lenghtOfWifiPort=message[2];
		if(lenghtOfWifiPort>WIFI_MAX_LENGHT_PORT)
			lenghtOfWifiPort=WIFI_MAX_LENGHT_PORT;
		sSytemVar.sEsp.ucNumberOfRepeatationWifi=message[3];
		sSytemVar.sEsp.ucTimeOutWifi=message[4];
		sSytemVar.sEsp.ucMessageLenghtWifi=message[5];
		for(i=0;i<lenghtOfWifiIP;i++)
			sSytemVar.sUart.ucpUartWifiIpName[i]=message[(6)+i];	
		for(i=0;i<lenghtOfWifiPort;i++)
			sSytemVar.sUart.ucpUartWifiPortName[i]=message[(lenghtOfWifiIP+6)+i];	
		for(i=0;i<sSytemVar.sEsp.ucMessageLenghtWifi;i++)
			sSytemVar.sUart.ucpUartWifiDataBuffer[i]=message[(lenghtOfWifiIP+lenghtOfWifiPort+3)];
		break;
	default:
		return OPCODE_WRONG;	
	}

	return OPCODE_OK;
}
ParserTypedef wifiSlaveStaticProcess(unsigned char opcode,unsigned char *message){
	switch(opcode){
	case WIFI_SlaveDynamicRepeatationActiveTimeOutActive:
		break;
	case WIFI_SlaveDynamicRepeatationActiveTimeOutPassive:
		break;
	case WIFI_SlaveDynamicRepeatationPassiveTimeOutActive:
		break;
	}
	return OPCODE_OK;
}
ParserTypedef wifiSlaveDynamicProcess(unsigned char opcode,unsigned char *message){
	switch(opcode){
	case WIFI_SlaveStaticDoNotSaveParamsToFlash:
		//
		break;	
	case WIFI_SlaveStaticSaveParamsToFlash:
		//
		break;		
	}
	return OPCODE_OK;
}

ParserTypedef messageParser(unsigned char opcode, unsigned char *message){
	if((opcode & PARSER)==GPRS_Process){
		return gprsProcess(opcode,message);
	}
	else if((opcode & PARSER)==SMS_Process){
		return smsProcess(opcode,message);
	}
	else if((opcode & PARSER)==BLE_Process){
		return bleProcess(opcode,message);
	}
	else if((opcode & PARSER)==WIFI_Process){
		return wifiProcess(opcode,message);
	}
	else{		
		return peripheralProcess(opcode,message);
	} 
}

ParserTypedef peripheralProcess(unsigned char opcode, unsigned char *message){
	if(opcode==PERIP_SetDO){
		return peripheralDO(message);
	}else if(opcode==PERIP_GetDI){
		return peripheralDI(message);
	}else if(opcode==PERIP_SetAO){
		return peripheralAO(message);
	}else if(opcode==PERIP_GetAI){
		return peripheralAI(message);
	}else if(opcode==PERIP_SetLED){
		return peripheralLED(message);
	}else if(opcode==PERIP_GetRotary){
		return peripheralROTARY(message);
	}else
		return OPCODE_WRONG;	
}

ParserTypedef peripheralDO(unsigned char *message){
	int i=0;
	for(i=0;i<4;i++){
		sSytemVar.sPeripheral.ucaDigitalOutput[i]=(message[0] >> i) & 1U;
	}
	return OPCODE_OK;	
}
ParserTypedef peripheralDI(unsigned char *message){
	unsigned char oneByteDIPerips=0;
	int i=0;
	for(i=0;i<4;i++){
		if(sSytemVar.sPeripheral.ucaDigitalInput[i]==DI_SET)
			oneByteDIPerips |= 1UL << i;
	}
	// set outputFormat
	sSytemVar.sUart.ucpUartPLCTxDataBuffer[0]=PERIP_GetDI;
	sSytemVar.sUart.ucpUartPLCTxDataBuffer[1]=oneByteDIPerips;
	//TODO
	//sSytemVar.sUart.ucpUartPLCTxDataBuffer[2]=CRC_BYTES;
	//sSytemVar.sUart.ucpUartPLCTxDataBuffer[3]=CRC_BYTES;
	sendVariablesToMasterDevice(4,300);
	return OPCODE_OK;	
}
ParserTypedef peripheralAO(unsigned char *message){
	unsigned short tempDOData=0;
	tempDOData=message[0];
	tempDOData=tempDOData<<8 | message[1];
	sSytemVar.sPeripheral.usaAnalogOutputExternal[0]=tempDOData;

	tempDOData=message[2];
	tempDOData=tempDOData<<8 | message[3];
	sSytemVar.sPeripheral.usaAnalogOutputExternal[1]=tempDOData;

	tempDOData=message[4];
	tempDOData=tempDOData<<8 | message[5];
	sSytemVar.sPeripheral.uiaAnalogOutputInternal[0]=tempDOData;

	tempDOData=message[6];
	tempDOData=tempDOData<<8 | message[7];
	sSytemVar.sPeripheral.uiaAnalogOutputInternal[1]=tempDOData;
	return OPCODE_OK;	
}
ParserTypedef peripheralAI(unsigned char *message){
	int i=0;
	sSytemVar.sUart.ucpUartPLCTxDataBuffer[0]=PERIP_GetAI;
	for(i=0;i<4;i++){
		parseUnsingedIntToCharPointer(&sSytemVar.sUart.ucpUartPLCTxDataBuffer[1+(i*4)],
		sSytemVar.sPeripheral.uiaAnalogInput[i]);
	}
	//sSytemVar.sUart.ucpUartPLCTxDataBuffer[2]=CRC_BYTES;
	//sSytemVar.sUart.ucpUartPLCTxDataBuffer[3]=CRC_BYTES;
	sendVariablesToMasterDevice(11,300);
	return OPCODE_OK;	
}
ParserTypedef peripheralLED(unsigned char *message){
	sSytemVar.sPeripheral.ucaLedStatus[0]=message[0];
	sSytemVar.sPeripheral.ucaLedStatus[1]=message[1];	
return OPCODE_OK;	
}
ParserTypedef peripheralROTARY(unsigned char *message){
	uParser.i=sSytemVar.sPeripheral.iRotaryCounter;
	memcpy(sSytemVar.sUart.ucpUartPLCTxDataBuffer,uParser.uc,4);
	sSytemVar.sUart.ucpUartPLCTxDataBuffer[4]=sSytemVar.sPeripheral.cRotaryOrientation;
	sendVariablesToMasterDevice(5,300);
	sSytemVar.sPeripheral.iRotaryCounter=0;
return OPCODE_OK;	
}
