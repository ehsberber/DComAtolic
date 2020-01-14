#ifndef __MESSAGE_PARSER_
#define __MESSAGE_PARSER_
#include "main.h"
#include "user_definitions.h"
//
typedef enum{
	OPCODE_OK,
	OPCODE_WRONG,
	OPCODE_EXCEED_LIMIT
}ParserTypedef;

#define DI_SET 1
#define DI_RESET 0

#define PARSER 0xC0
#define IFSTATIC 0x20
#define IFMASTER 0x04

//PERIPHERAL RELATED
#define PERIP_SetDO 0x01
#define PERIP_GetDI 0x02
#define PERIP_SetAO 0x03
#define PERIP_GetAI 0x04
#define PERIP_SetLED 0x05
#define PERIP_GetRotary 0x06

//GPRS RELATED 
#define GPRS_Process 0x00
#define GPRS_StaticSaveParamsToFlash 0x30
#define GPRS_StaticDoNotSaveParamsToFlash 0x20
#define GPRS_DynamicRepeatationActiveTimeOutPassive 0x10
#define GPRS_DynamicRepeatationPassiveTimeOutActive 0x08
#define GPRS_DynamicRepeatationActiveTimeOutActive 0x18
//SMS RELATED
#define SMS_Process 0x40
#define SMS_StaticSaveParamsToFlash 0x70
#define SMS_StaticDoNotSaveParamsToFlash 0x60
#define SMS_DynamicRepeatationActiveTimeOutPassive 0x50
#define SMS_DynamicRepeatationPassiveTimeOutActive 0x48
#define SMS_DynamicRepeatationActiveTimeOutActive 0x58
//BLE RELATED -MASTER
#define BLE_Process 0x80
#define BLE_MasterStaticSaveParamsToFlash 0xB4
#define BLE_MasterStaticDoNotSaveParamsToFlash 0xA4
#define BLE_MasterDynamicRepeatationActiveTimeOutPassive 0x94
#define BLE_MasterDynamicRepeatationPassiveTimeOutActive 0x8C
#define BLE_MasterDynamicRepeatationActiveTimeOutActive 0x9C
//BLE RELATED -SLAVE
#define BLE_SlaveStaticSaveParamsToFlash 0xB0
#define BLE_SlaveStaticDoNotSaveParamsToFlash 0xA0
#define BLE_SlaveDynamicRepeatationActiveTimeOutPassive 0x90
#define BLE_SlaveDynamicRepeatationPassiveTimeOutActive 0x88
#define BLE_SlaveDynamicRepeatationActiveTimeOutActive 0x98
//WIFI RELATED -MASTER
#define WIFI_Process 0xC0
#define WIFI_MasterStaticSaveParamsToFlash 0xF4
#define WIFI_MasterStaticDoNotSaveParamsToFlash 0xE4
#define WIFI_MasterDynamicRepeatationActiveTimeOutPassive 0xD4
#define WIFI_MasterDynamicRepeatationPassiveTimeOutActive 0xCC
#define WIFI_MasterDynamicRepeatationActiveTimeOutActive 0xDC
//WIFI RELATED -SLAVE
#define WIFI_SlaveStaticSaveParamsToFlash 0xF0
#define WIFI_SlaveStaticDoNotSaveParamsToFlash 0xE0
#define WIFI_SlaveDynamicRepeatationActiveTimeOutPassive 0xD0
#define WIFI_SlaveDynamicRepeatationPassiveTimeOutActive 0xC8
#define WIFI_SlaveDynamicRepeatationActiveTimeOutActive 0xD8


//Function Prototypes
ParserTypedef peripheralProcess(unsigned char opcode, unsigned char *message);
ParserTypedef peripheralDO(unsigned char *message);
ParserTypedef peripheralDI(unsigned char *message);
ParserTypedef peripheralAO(unsigned char *message);
ParserTypedef peripheralAI(unsigned char *message);
ParserTypedef peripheralLED(unsigned char *message);
ParserTypedef peripheralROTARY(unsigned char *message);
//
ParserTypedef messageParser(unsigned char opcode, unsigned char *message);
ParserTypedef gprsProcess(unsigned char opcode,unsigned char *message);
ParserTypedef gprsStaticProcess(unsigned char opcode,unsigned char *message);
ParserTypedef gprsDynamicProcess(unsigned char opcode,unsigned char *message);
ParserTypedef smsProcess(unsigned char opcode,unsigned char *message);
ParserTypedef smsStaticProcess(unsigned char opcode,unsigned char *message);
ParserTypedef smsDynamicProcess(unsigned char opcode,unsigned char *message);
ParserTypedef bleProcess(unsigned char opcode,unsigned char *message);
ParserTypedef bleMasterStaticProcess(unsigned char opcode,unsigned char *message);
ParserTypedef bleMasterDynamicProcess(unsigned char opcode,unsigned char *message);
ParserTypedef bleSlaveStaticProcess(unsigned char opcode,unsigned char *message);
ParserTypedef bleSlaveDynamicProcess(unsigned char opcode,unsigned char *message);
ParserTypedef wifiProcess(unsigned char opcode,unsigned char *message);
ParserTypedef wifiMasterStaticProcess(unsigned char opcode,unsigned char *message);
ParserTypedef wifiMasterDynamicProcess(unsigned char opcode,unsigned char *message);
ParserTypedef wifiSlaveStaticProcess(unsigned char opcode,unsigned char *message);
ParserTypedef wifiSlaveDynamicProcess(unsigned char opcode,unsigned char *message);
#endif
