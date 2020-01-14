#include "internal_flash.h"

// set device macros 

#define STATIC_START_ADDRESS_FOR_FLASH
void parseUnsignedCharArrayToInt(unsigned char * char_array,unsigned int * dataReturn){
//parse unsigned char array to int
//private function
	dataReturn[0] = (uint64_t)char_array[3] << 24 |
									(uint64_t)char_array[2] << 16 |
									(uint64_t)char_array[1] << 8 |
									(uint64_t)char_array[0];
}
void parseIntToUCharArray(unsigned char* char_array,unsigned int *int_array, unsigned short lenght_of_char_arry){
	int i;
	for(i=0;i<lenght_of_char_arry;i++){		
		char_array[(i*4)+0]=int_array[i];
		char_array[(i*4)+1]=int_array[i]>>8;
		char_array[(i*4)+2]=int_array[i]>>16;
		char_array[(i*4)+3]=int_array[i]>>24;
	}
}
void UnlockFlash(void){
	//unlock flash for writing
	HAL_FLASH_Unlock();
}

void LockFlash(void){
	//lock flash to aviod rewriting after this command flash only can be read 
	HAL_FLASH_Lock();
}
FlashStatusTypeDef AdjustSystemParams(EraseWriteFlash *flash){
	//////////////////////adjust class variables for flash read write
	flash->TypeErase=FLASH_TYPEERASE_SECTORS;
	flash->PageAddress=FLASH_USER_START_ADDR;
	//flash->NbPages=(FLASH_USER_END_ADDR - FLASH_USER_START_ADDR )>>7;
	flash->Address=FLASH_USER_START_ADDR;
	//////////////////////////////////////////////////////////////////////////////////
	//////////////////////adjust erase struct parameters//////////////////////////////
	flash->EraseInitStruct.TypeErase=flash->TypeErase;
	//flash->EraseInitStruct.Banks= flash->PageAddress;
	//flash->EraseInitStruct.NbPages=flash->NbPages;
	return FLASH_OK;
}
FlashStatusTypeDef EraseFlash(EraseWriteFlash *flash){
	return (FlashStatusTypeDef)	HAL_FLASHEx_Erase(&flash->EraseInitStruct, &flash->PAGEError);
}
FlashStatusTypeDef WriteFlash(EraseWriteFlash *flash,unsigned char *data, unsigned int lenght){
	unsigned short errorCounter=0;	
	unsigned int uiAddress;
	unsigned int uiDataForFlash;
	int iDataLenghtCounter=0;
	
	UnlockFlash();
	EraseFlash(flash);
	#ifdef STATIC_START_ADDRESS_FOR_FLASH
	uiAddress=FLASH_USER_START_ADDR;
	#else
		address = flash->Address;
	#endif	

	while(uiAddress<FLASH_USER_END_ADDR){			
		parseUnsignedCharArrayToInt(&data[(iDataLenghtCounter*4)%lenght],&uiDataForFlash);	//parse unsigned data array to long 
		if(HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, uiAddress, uiDataForFlash) == HAL_OK){		
			uiAddress=uiAddress+4;//4 is for address width
			iDataLenghtCounter++;
		}else{	
			errorCounter++;
		}
		if(errorCounter>=0x7FFF){
			LockFlash();
			return FLASH_ERROR;
		}
			//HAL_Delay(0);
	}

	errorCounter=0;//reset error counter
	LockFlash();
	return FLASH_OK;
}

FlashStatusTypeDef ReadFlash(EraseWriteFlash *flash, unsigned char *data_array,unsigned int lenght){	
	int data_lenght=lenght/4; // unsinged char lenght 1 byte 4 byte is an integer adress writing 
	int tmpC=0;
	unsigned int tmpDataIntArry;
	uint32_t address;
	
	#ifdef STATIC_START_ADDRESS_FOR_FLASH
	address=FLASH_USER_START_ADDR;
	#else
		address = flash->Address;
	#endif

	while(address<(int)FLASH_USER_END_ADDR){	    
		tmpDataIntArry = (*(__IO uint32_t *)address);
		parseIntToUCharArray(&data_array[tmpC*4],&tmpDataIntArry,1);
		address = address + 4;
		tmpC++;
		if(tmpC>data_lenght){
			 return FLASH_OVER_WRITE;// if length is shorter then page no overwirte ocurs			
		}		
	}		
	return FLASH_OK;	
}	
