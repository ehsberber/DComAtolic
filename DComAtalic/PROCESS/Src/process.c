#include "process.h"
#include "dac_process.h"
#include "user_definitions.h"
#include "message_parser.h"
#include "25LC512_EEprom.h"
//
#include <stdio.h> 
#include <string.h> 
// private defines
#define ADC_TIMEOUT 300
#define DO_SET 1
#define DO_RESET 0
#define CH1 0
#define CH2 1
//prototypes 
void prsCheckDO(_SYS *sys);
void prsCheckAO(_SYS *sys);
void prsCheckAOEx(_SYS *sys);
void prsCheckAOIn(_SYS *sys);
void prsCheckAI(_SYS *sys);
void prsCheckDI(_SYS *sys);
void prsCheckPCF(_SYS *sys);
//void prsCheckRotary(_SYS *sys);
//user declerations
_UART sUart;

_SYS sSytemVar;

unionTypeDef uParser;
//TODO adjust do/DI port pin 
GPIO_TypeDef *usaDOPort[]={DO0_GPIO_Port,DO1_GPIO_Port,DO2_GPIO_Port,DO3_GPIO_Port};
GPIO_TypeDef *usaDIPort[]={DI_0_GPIO_Port,DI_1_GPIO_Port,DI_2_GPIO_Port,DI_3_GPIO_Port};

uint16_t usaDOPin[]={DO0_Pin,DO1_Pin,DO2_Pin,DO3_Pin};
uint16_t usaDIPin[]={DI_0_Pin,DI_1_Pin,DI_2_Pin,DI_3_Pin};

unsigned char PCF_ADDRESS=0x40 ;
unsigned int uiAIConfigChannel[]={ADC_CHANNEL_0,ADC_CHANNEL_1,ADC_CHANNEL_2,ADC_CHANNEL_10};
//definitions

void prsInit(void){
	initDac();
	HAL_ADC_Start(&hadc1);
	initEEProm();
}

void prsLoop(void){
	prsCheckPeripheral();
	prsCheckUartPacket();
}
void prsCheckPeripheral(void){
	prsCheckDO(&sSytemVar);
	prsCheckDI(&sSytemVar);
	prsCheckAO(&sSytemVar);
	prsCheckAI(&sSytemVar);
	prsCheckPCF(&sSytemVar);	
}

void prsCheckPCF(_SYS *sys){
	if( (0x01 & PCF_ADDRESS)==1){
		HAL_I2C_Master_Receive(&HI2C_PCF,PCF_ADDRESS,sys->sPeripheral.ucaLedStatus,2,300);
	}else{
		HAL_I2C_Master_Transmit(&HI2C_PCF,PCF_ADDRESS,sys->sPeripheral.ucaLedStatus,2,300);	
	}
}
void prsCheckUartPacket(void){	
	if(	*sSytemVar.sUart.ucpUartPLCPacketFinishedFlag==UART_FL_SET){	
		*sSytemVar.sUart.ucpUartPLCPacketFinishedFlag=UART_FL_RESET;		
		// TODO check peripheral if perip mode get set A/D input/output values
		if(messageParser(sSytemVar.sUart.ucpUartPLCRxDataBuffer[0],&sSytemVar.sUart.ucpUartPLCRxDataBuffer[1])==OPCODE_WRONG){
			HAL_UART_Transmit(sSytemVar.sUart.hpUartPLC,(unsigned char*)"WRONG OPCODE",12,300);
		}
		prsFlushUartAndResetPointer(sSytemVar.sUart.hpUartPLC,
									sSytemVar.sUart.ucpUartPLCRxDataBuffer,PLC_UART_RX_BUFFER_LENGHT);
	}
}
void prsCheckAO(_SYS *sys){	
	prsCheckAOIn(sys);
	prsCheckAOEx(sys);	
}

void prsCheckAOEx(_SYS *sys){		
	setExternalDacValue(1,sys->sPeripheral.usaAnalogOutputExternal[CH1]);
	setExternalDacValue(2,sys->sPeripheral.usaAnalogOutputExternal[CH2]);
}
void prsCheckAOIn(_SYS *sys){
	setInternalDacValue(DAC_CHANNEL_1,sys->sPeripheral.uiaAnalogOutputInternal[CH1]);
	setInternalDacValue(DAC_CHANNEL_2,sys->sPeripheral.uiaAnalogOutputInternal[CH2]);		
}
void prsCheckAI(_SYS *sys){	
	static unsigned char counter=0;
	counter=(counter+1)%4;
	ADC_ChannelConfTypeDef sConfig;
	sConfig.Channel      = uiAIConfigChannel[counter];
  sConfig.Rank         = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
  sConfig.Offset       = 0;	
	if(HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK){
    Error_Handler();
  }
	HAL_ADC_Start(&hadc1);
	if(HAL_ADC_PollForConversion(&hadc1,ADC_TIMEOUT)==HAL_OK)
		sys->sPeripheral.uiaAnalogInput[counter]=HAL_ADC_GetValue(&hadc1);
}
void prsCheckDI(_SYS *sys){
	int i=0;
	for(i=0;i<4;i++){
		if(HAL_GPIO_ReadPin(usaDIPort[i],usaDIPin[i])==GPIO_PIN_SET){
			sys->sPeripheral.ucaDigitalInput[i]=DI_SET;
		}else{
			sys->sPeripheral.ucaDigitalInput[i]=DI_RESET;
		}			
	}
}
void prsCheckDO(_SYS *sys){
	int i=0;
	for(i=0;i<4;i++){
		if(sys->sPeripheral.ucaDigitalOutput[i]==DO_SET){
			HAL_GPIO_WritePin(usaDOPort[i],usaDOPin[i],GPIO_PIN_SET);
		}else{
			HAL_GPIO_WritePin(usaDOPort[i],usaDOPin[i],GPIO_PIN_RESET);
		}
	}
}

void prsFlushUartAndResetPointer(UART_HandleTypeDef *huart, unsigned char *data, unsigned short lenght){
	HAL_UART_Receive_IT(huart,data,lenght);	
	memset(data,0,lenght);
	huart->pRxBuffPtr=data;
	huart->RxXferCount=lenght;
	huart->RxXferSize =lenght;
}
