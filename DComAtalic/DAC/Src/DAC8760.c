#include "DAC8760.h"
#define LSB 0
#define MSB 1
/// prototypes
void DAC8760_SetSPIEnablePin(Dac8760TypeDef *dac,char SetResetStatus);
void DAC8760_Write(Dac8760TypeDef * dac,unsigned char spiTransmitArray[3]);
void DAC8760_Config(Dac8760TypeDef *dac,unsigned char ControlRegister[2],	unsigned char ConfigurationRegister[2]);
/// definitions
void DAC8760_SetDacValue(Dac8760TypeDef *dac){
	unsigned char ucWriteArray[3]={WriteDacDataRegister,dac->usData>>8,dac->usData};	
	
	DAC8760_Write(dac,ucWriteArray);
}
void DAC8760_SetSPIEnablePin(Dac8760TypeDef *dac,char SetResetStatus){
		if(SetResetStatus=='S'||SetResetStatus=='s') {
			HAL_GPIO_WritePin(dac->tpCSPort,dac->usCSPin,GPIO_PIN_SET);		
		}else {
			HAL_GPIO_WritePin(dac->tpCSPort,dac->usCSPin,GPIO_PIN_RESET);
		}		
}
void DAC8760_Write(Dac8760TypeDef * dac,unsigned char spiTransmitArray[3]){																 
	//low the latch clock for data input 
	DAC8760_SetSPIEnablePin(dac,'R');// Open the spi communication
	//HAL_GPIO_WritePin(SPI1_CS5_GPIO_Port,SPI1_CS5_Pin,GPIO_PIN_RESET);///FORMER CODE WILL DELETE
	// PortB13 for just logic anaylzer
	//	HAL_GPIO_WritePin(PB13_GPIO_Port,PB13_Pin,GPIO_PIN_RESET);
	// transmit the right amount of data in to spi register
	HAL_SPI_Transmit(&hspi1,spiTransmitArray,3,1000);
	// after sending the suitable data to register we must togle the latch pin for writng the 
	//data in to the register.
	DAC8760_SetSPIEnablePin(dac,'S');//Set the communication for fatcing the latch pin
	DAC8760_SetSPIEnablePin(dac,'R');//One toggle for accurate data accusation
	DAC8760_SetSPIEnablePin(dac,'S');// Set the chipselect for next communication					 
}
void DAC8760_Config(Dac8760TypeDef *dac,unsigned char ControlRegister[2],	unsigned char ConfigurationRegister[2]){	
	unsigned char ucaData[3];
	
	ucaData[0]=WriteResetRegister;
	ucaData[1]=0x00;
	ucaData[2]=0x01;
	DAC8760_Write(dac,ucaData);
	ucaData[0]=WriteControlRegister;
	ucaData[1]=ControlRegister[LSB];
	ucaData[2]=ControlRegister[MSB];	
	DAC8760_Write(dac,ucaData);
	ucaData[0]=WriteConfigurationRegister;
	ucaData[1]=ConfigurationRegister[LSB];
	ucaData[2]=ConfigurationRegister[MSB];		
	DAC8760_Write(dac,ucaData);
}

void DAC8760_Init(Dac8760TypeDef * dac){
	unsigned char ControlRegister[2];
	unsigned char ConfigurationRegister[2];
		
	switch ((unsigned char)dac->configByte){		
			case 0x00:
			case 0x08:
				ControlRegister[MSB]=ControlRegDual_PlusMinus10V_MSB;
				ControlRegister[LSB]=ControlRegDual_PlusMinus10V_LSB;
				ConfigurationRegister[MSB]=ConfigurationRegDual_0to20mA_MSB;
				ConfigurationRegister[LSB]=ConfigurationRegDual_0to20mA_LSB;
				break;		
			
			case 0x01:
			case 0x09:
				ControlRegister[MSB]=ControlRegDual_PlusMinus5V_MSB;
				ControlRegister[LSB]=ControlRegDual_PlusMinus5V_LSB;
				ConfigurationRegister[MSB]=ConfigurationRegDual_0to20mA_MSB;
				ConfigurationRegister[LSB]=ConfigurationRegDual_0to20mA_LSB;
				break;
		
			case 0x02:
			case 0x0A:
				ControlRegister[MSB]=ControlRegDual_0to10V_MSB;
				ControlRegister[LSB]=ControlRegDual_0to10V_LSB;
				ConfigurationRegister[MSB]=ConfigurationRegDual_0to20mA_MSB;
				ConfigurationRegister[LSB]=ConfigurationRegDual_0to20mA_LSB;
				break;
		
			case 0x03:
			case 0x0B:			
				ControlRegister[MSB]=ControlRegDual_0Vto5V_MSB;
				ControlRegister[LSB]=ControlRegDual_0Vto5V_LSB;
				ConfigurationRegister[MSB]=ConfigurationRegDual_0to20mA_MSB;
				ConfigurationRegister[LSB]=ConfigurationRegDual_0to20mA_LSB;	
				break;
				
			case 0x04:
			case 0x0C:		
				ControlRegister[MSB]=ControlRegDual_PlusMinus10V_MSB;
				ControlRegister[LSB]=ControlRegDual_PlusMinus10V_LSB;
				ConfigurationRegister[MSB]=ConfigurationRegDual_4to20mA_MSB;
				ConfigurationRegister[LSB]=ConfigurationRegDual_4to20mA_LSB;
				break;
			
			case 0x05:
			case 0x0D:
				ControlRegister[MSB]=ControlRegDual_PlusMinus5V_MSB;
				ControlRegister[LSB]=ControlRegDual_PlusMinus5V_LSB;
				ConfigurationRegister[MSB]=ConfigurationRegDual_4to20mA_MSB;
				ConfigurationRegister[LSB]=ConfigurationRegDual_4to20mA_LSB;
				break;
			
			case 0x06:
			case 0x0E:
				ControlRegister[MSB]=ControlRegDual_0to10V_MSB;
				ControlRegister[LSB]=ControlRegDual_0to10V_LSB;
				ConfigurationRegister[MSB]=ConfigurationRegDual_4to20mA_MSB;
				ConfigurationRegister[LSB]=ConfigurationRegDual_4to20mA_LSB;
				break;
				
			case 0x07:
			case 0x0F:
				ControlRegister[MSB]=ControlRegDual_0Vto5V_MSB;
				ControlRegister[LSB]=ControlRegDual_0Vto5V_LSB;
				ConfigurationRegister[MSB]=ConfigurationRegDual_4to20mA_MSB;
				ConfigurationRegister[LSB]=ConfigurationRegDual_4to20mA_LSB;
				break;	
			
			default:
				ControlRegister[MSB]=ControlRegDual_0Vto5V_MSB;
				ControlRegister[LSB]=ControlRegDual_0Vto5V_LSB;
				ConfigurationRegister[MSB]=ConfigurationRegDual_4to20mA_MSB;
				ConfigurationRegister[LSB]=ConfigurationRegDual_4to20mA_LSB;				
		}	
	DAC8760_Config(dac,ControlRegister,ConfigurationRegister);	
}

