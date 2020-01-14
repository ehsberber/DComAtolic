#include "25LC512_EEprom.h"
#include "user_definitions.h"
EEprom8760TypeDef eeprom;
unsigned int init25LC512(void);
unsigned int MC25LC512_ReleaseDeepPowerDownMode(void);
void MC25LC512_WriteEnableOrDisable(unsigned char EnableOrDisable);//enable for one
void MC25LC512_CS(GPIO_PinState pinState);

void MC25LC512_CS(GPIO_PinState pinState){
	HAL_GPIO_WritePin(eeprom.tpCSPort,eeprom.usCSPin,pinState);
	HAL_Delay(5);
}

void MC25LC512_Read(uint16_t address, unsigned char *data,unsigned short size){
	unsigned char SendOneByte;
	
	MC25LC512_CS(GPIO_PIN_RESET);// Reset The spi Chip //Reset means Enable
	SendOneByte=MC25LCxxx_SPI_READ;//Config the Device
	HAL_SPI_Transmit(eeprom.tphspi,&SendOneByte,1,200);	
	SendOneByte= address>>8;
	HAL_SPI_Transmit(eeprom.tphspi,&SendOneByte,1,200);//High byte of address
	SendOneByte= address & 0x00FF;
	HAL_SPI_Transmit(eeprom.tphspi,&SendOneByte,1,200);//Low byte of address
	HAL_SPI_Receive(eeprom.tphspi , data, size,size*EEPROM_TimeoutMultiplier) ;//Receive Amount of  Data from EEPROM
	MC25LC512_CS(GPIO_PIN_SET);// Reset The spi Chip //Reset means Enable
}

void MC25LC512_WriteEnableOrDisable(unsigned char EnableOrDisable){
	unsigned char SendOneByte=0;

	MC25LC512_CS(GPIO_PIN_RESET);// Reset The spi Chip //Reset means Enable
	if(EnableOrDisable==EEPROM_Enable){
			SendOneByte=MC25LCxxx_SPI_WREN;
	}
	else{
			SendOneByte=MC25LCxxx_SPI_WRDI;
	}
	HAL_SPI_Transmit(eeprom.tphspi , &SendOneByte, 1, 200) ;	
	MC25LC512_CS(GPIO_PIN_SET);// Set The spi Chip //Set means Disable
}
void MC25LC512_Write(uint16_t address, unsigned char *data,unsigned short size){
	unsigned char SendOneByte;
	
	MC25LC512_CS(GPIO_PIN_RESET);// Reset The spi Chip //Reset means Enable
	SendOneByte=MC25LCxxx_SPI_WRITE;
	HAL_SPI_Transmit(eeprom.tphspi,&SendOneByte,1,200);
	SendOneByte= address>>8;
	HAL_SPI_Transmit(eeprom.tphspi,&SendOneByte,1,200);//High byte of address
	SendOneByte= address & 0x00FF;
	HAL_SPI_Transmit(eeprom.tphspi,&SendOneByte,1,200);//Low byte of address
	HAL_SPI_Transmit(eeprom.tphspi , data  , size , size*EEPROM_TimeoutMultiplier) ;
	MC25LC512_CS(GPIO_PIN_SET);// Reset The spi Chip //Reset means Enable
	MC25LC512_WriteEnableOrDisable(EEPROM_Enable);
}
unsigned char MC25LC512_ReadStatusRegister(void){
	unsigned char SendOneByte=0;
	unsigned char ReceiveOneByte;
	
	SendOneByte=MC25LCxxx_SPI_RDSR;
	MC25LC512_CS(GPIO_PIN_RESET);// Reset The spi Chip //Reset means Enable
	HAL_SPI_Transmit(eeprom.tphspi,&SendOneByte,1,200);
	HAL_SPI_Receive(eeprom.tphspi, &ReceiveOneByte, 1,200) ;//Address of Manufaturer id High
	MC25LC512_CS(GPIO_PIN_SET);// Set The spi Chip //Set means Disable
	return ReceiveOneByte;
}	

unsigned int MC25LC512_ReleaseDeepPowerDownMode(void){
	unsigned char SendOneByte;
	
	SendOneByte=MC25LCxxx_SPI_RDID;
	MC25LC512_CS(GPIO_PIN_RESET);// Reset The spi Chip //Reset means Enable	
	HAL_SPI_Transmit(eeprom.tphspi,&SendOneByte,1,200);	
	HAL_SPI_Receive(eeprom.tphspi , &uParser.uc[0], 1,200) ;//Address of Manufaturer id High
	HAL_SPI_Receive(eeprom.tphspi , &uParser.uc[1], 1,200) ;//Address of Manufaturer id Low
	HAL_SPI_Receive(eeprom.tphspi , &uParser.uc[2], 1,200) ;//Manufaturer id	
	MC25LC512_CS(GPIO_PIN_SET);// Set The spi Chip //Set means Disable
	return uParser.ui;
}
unsigned int initEEProm(void){
	eeprom.tphspi=&HSPI_EEPROM;
	eeprom.tpCSPort=HSPI_EEPROM_PORT;
	eeprom.usCSPin=HSPI_EEPROM_PIN;
	return init25LC512();
}
unsigned int init25LC512(void){	
	MC25LC512_CS(GPIO_PIN_SET);// Reset The spi Chip //Reset means Enable
	eeprom.uiDeviceSignature=MC25LC512_ReleaseDeepPowerDownMode();
	eeprom.ucStatusRegister=MC25LC512_ReadStatusRegister();
	MC25LC512_WriteEnableOrDisable(GPIO_PIN_SET);
	return eeprom.uiDeviceSignature;
}
