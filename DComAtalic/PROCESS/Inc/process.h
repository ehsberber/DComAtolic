#ifndef __PROCESS__
#define __PROCESS__
#include "main.h"
#include "user_definitions.h"
#include "adc.h"
#include "i2c.h"
#include "tim.h"

void prsInit(void);
void prsLoop(void);
void prsCheckPeripheral(void);
void prsCheckUartPacket(void);
void prsFlushUartAndResetPointer(UART_HandleTypeDef *huart,
								unsigned char *data, unsigned short lenght);
#endif

