#ifndef __QUECTELL__
#define __QUECTELL__
#include "main.h"
#include "user_definitions.h"

typedef enum{
	QUECTEL_INVALID_DATA=0,
	QUECTEL_EMPTY_BUFFER,
	QUECTEL_SERVER_CONNECTION_FAIL=2,
	QUECTEL_SMS_FAIL=6	
}QuectellErrorStatusTypedef;

typedef enum{
	QECTEL_OK       = 0x00U,
	QECTEL_ERROR    = 0x01U,
	QECTEL_BUSY     = 0x02U,
	QECTEL_TIMEOUT  = 0x03U
}QuectellStatusTypedef;

QuectellStatusTypedef quecSMSProcess(_QUECTELL *struct_quectell);
QuectellStatusTypedef quecServerProcess(_QUECTELL *struct_quectell);
QuectellStatusTypedef quecProcess(_QUECTELL *struct_quectell);
//
QuectellStatusTypedef quecWakeModuleUp(_QUECTELL *struct_quectell);
QuectellStatusTypedef quecShutModuleDown(_QUECTELL *struct_quectell);
QuectellStatusTypedef quecResetModule(_QUECTELL *struct_quectell);
QuectellStatusTypedef quecSetAndSendSMS(_QUECTELL *struct_quectell);
QuectellStatusTypedef quecSetAndSendNetworkInfo(_QUECTELL *struct_quectell);
QuectellStatusTypedef quecSendMessageToModule(unsigned char *data,unsigned short data_lenght);

#endif

