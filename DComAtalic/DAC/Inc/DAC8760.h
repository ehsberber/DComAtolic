#ifndef __DAC8760_H
#define __DAC8760_H
#ifdef __cplusplus
 extern "C" {
#endif
#include "main.h"
#include "spi.h"
#include "gpio.h"


///////////////****************DEF FOR I2C****************************************************//////
//int8_t CounterForI2CToDac8760;
#define I2C_FLAG_FROM_INTERRUPT_SET				1
#define I2C_FLAG_FROM_INTERRUPT_RESET			0
/***************************FUNCTION PROTOTYPES*********************************************/
typedef enum{
	pulusMinus10VoltAnd0To20mA,
	pulusMinus5VoltAnd0To20mA,
	zeroPlus10VoltAnd0To20mA,
	zeroPlus5VoltAnd0To20mA,
	pulusMinus10VoltAnd4To20mA,
	pulusMinus5VoltAnd4To20mA,
	zeroPlus10VoltAnd4To20mA,
	zeroPlus5VoltAnd4To20mA	
}Dac8760Enum;
typedef struct {
	SPI_HandleTypeDef *tphspi;
	GPIO_TypeDef *tpCSPort;
	uint16_t usCSPin;
	unsigned short usData;
	Dac8760Enum configByte;
}Dac8760TypeDef;

void DAC8760_Init(Dac8760TypeDef * dac);

void DAC8760_SetDacValue(Dac8760TypeDef *dac);
//

/*****************************DAC8760 DEFINITIONS**********************************************/
//// This file contains Write/Read commends and the registers.
/*WRITE ADDRESS FUNCTIONS*/
// The operations of the chip determined by the firt byte it could be the 
// following commands 

#define 		 NOP																  			  0x00
#define			 WriteDacDataRegister											  	0x01
#define			 RegisterRead 															  0x02
#define			 WriteControlRegister 											  0x55
#define			 WriteResetRegister 												  0x56
#define			 WriteConfigurationRegister								  	0x57
#define			 WriteDacGainCalibrationRegister 					 	 	0x58
#define			 WriteDacZeroCalibrationRegister		  			  0x59
#define			 WatchDogTimerRegister 										  	0x95
// For read operation user must follow this process
//ADDRESS BYTE -----DATA WORD(2BYTE)
//								BIT 15:BIT6 ---BIT5:BIT0
//0x02						X(don't Care)	 Register Address
/*.REGISTER ADDRESS FOR THE READ FUNCTION*/
#define				 ReadStatusRegister  								  	0x0000
#define				 ReadDACDataRegister										0x0001
#define				 ReadControlRegister										0x0002
#define				 ReadConfigurationRegister							0x000B
#define				 ReadDACGainCalibrationRegister					0x0013
#define				 ReadDACZeroCalibrationRegister					0x0017
/*****************************DAC Voltage & Current Enable Modes ***************************/
// THESE MODES FOR DUAL OR MONO  USAGE OF THE  OUTPUT OF DAC 	INTEGRATED DEVICE
/*CONTROL REGISTER  DUAL&MONO MODES*/
// DUAL MODE CONTROL REG
#define  ControlRegDual_0Vto5V_MSB									0x10		// Zero to 5 volt output for dual control
#define  ControlRegDual_0Vto5V_LSB									0x00

#define  ControlRegDual_0to10V_MSB									0x10		// Zero to 10 volt output for dual control
#define  ControlRegDual_0to10V_LSB									0x01

#define  ControlRegDual_PlusMinus5V_MSB							0x10		// Plus minus  5 volt output for dual control
#define  ControlRegDual_PlusMinus5V_LSB							0x02

#define ControlRegDual_PlusMinus10V_MSB							0x10		// Plus minus 10 volt output for dual control
#define  ControlRegDual_PlusMinus10V_LSB						0x03

//MONO MODE CONTROL REG
#define  ControlRegMono_0Vto5V_MSB									0x10		// Zero to 5 volt output for mono control
#define  ControlRegMono_0Vto5V_LSB									0x00		

#define ControlRegMono_0Vto10V_MSB									0x10		// Zero to 10 volt output for mono control
#define  ControlRegMono_0Vto10V_LSB									0x01		

#define  ControlRegMono_PlusMinus5V_MSB							0x10		// Plus minus 5 volt output for mono control
#define  ControlRegMono_PlusMinus5V_LSB							0x02		

#define  ControlRegMono_PlusMinus10V_MSB						0x10		// Plus minus 10 volt output for mono control
#define  ControlRegMono_PlusMinus10V_LSB						0x03		

#define  ControlRegMono_4to20mA_MSB									0x10		// 4 to 20 mA  output for mono control
#define  ControlRegMono_4to20mA_LSB									0x05		

#define ControlRegMono_0to20mA_MSB									0x10		// 0 to 20 mA  output for mono control
#define  ControlRegMono_0to20mA_LSB									0x06		

#define  ControlRegMono_0to24mA_MSB									0x10		// 0 to 24 mA  output for mono control
#define  ControlRegMono_0to24mA_LSB									0x07		

/*CONFIGURATION REGISTER DUAL&MONO MODES*/
//DUAL MODE CONTROL REG
#define  ConfigurationRegDual_DisablemA_MSB					0x01		// Disable 4-20 mA output for dual control
#define  ConfigurationRegDual_DisablemA_LSB					0x00		

#define ConfigurationRegDual_4to20mA_MSB						0x03		// 4-20 mA output for dual control
#define  ConfigurationRegDual_4to20mA_LSB						0x00		

#define ConfigurationRegDual_0to20mA_MSB						0x05		// 0-20 mA output for dual control
#define  ConfigurationRegDual_0to20mA_LSB						0x00		

#define ConfigurationRegDual_0to24mA_MSB						0x07		// 4-24 mA output for dual control
#define  ConfigurationRegDual_0to24mA_LSB						0x00		
//MONO MODE CONTROL REG
#define  ConfigurationRegMonoForAllModes_MSB				0x00		//This modes for all mono modes configuration
#define ConfigurationRegMonoForAllModes_LSB					0x00
#ifdef __cplusplus
}
#endif
#endif /*__ i2c_H */

