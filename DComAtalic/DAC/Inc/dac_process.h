#ifndef __DAC_PROCESS__
#define __DAC_PROCESS__

#include "stm32f7xx_hal.h"
#include "spi.h"
#include "gpio.h"
#include "dac.h"

void initDac(void);
void setExternalDacValue(unsigned char dac_no, unsigned short value);
void setInternalDacValue(unsigned int dac_channel,unsigned int value);
#endif

