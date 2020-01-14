#include "dac_process.h"
#include "DAC8760.h"
#include "user_definitions.h"

Dac8760TypeDef dac1;
Dac8760TypeDef dac2;

void initDac(){
//set local adc params
	__HAL_DAC_ENABLE(&hdac, DAC_CHANNEL_1);	
	__HAL_DAC_ENABLE(&hdac, DAC_CHANNEL_2); 		
//set dac1 Params  
	dac1.tphspi=&HSPI_DAC;
	dac1.configByte=pulusMinus10VoltAnd4To20mA;
	dac1.tpCSPort=HSPI_DAC1_PORT;
	dac1.usCSPin= HSPI_DAC1_PIN;
	DAC8760_Init(&dac1);
//set dac2 Params
	dac2.tphspi=&hspi1;
	dac2.configByte=pulusMinus10VoltAnd4To20mA;
	dac2.tpCSPort=HSPI_DAC2_PORT;
	dac2.usCSPin= HSPI_DAC2_PIN;
	DAC8760_Init(&dac2);
}
void setExternalDacValue(unsigned char dac_no, unsigned short value){
	if(dac_no==1){
		dac1.usData=value;
		DAC8760_SetDacValue(&dac1);
	}
	else{
		dac2.usData=value;
		DAC8760_SetDacValue(&dac2);
	}
}

void setInternalDacValue(unsigned int dac_channel,unsigned int value){
		HAL_DAC_Stop(&hdac,dac_channel);
		HAL_DAC_SetValue(&hdac, dac_channel,DAC_ALIGN_12B_L,value);
		HAL_DAC_Start(&hdac,dac_channel);	
}
