/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dac.h"
#include "i2c.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "../PROCESS/Inc/process.h"
#include "../PROCESS/Inc/user_definitions.h"
/* USER CODE END Includes */
//Test Commit //TODO test 2 commit
/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void initInterrupts(void);
void systemInit(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
//TODO SPI values must be configurative from user config
//TODO Test Commit 4
//Variable Declerations
unsigned char ucaPLCUartTxData[PLC_UART_TX_BUFFER_LENGHT];
unsigned char ucaPLCUartRxData[PLC_UART_RX_BUFFER_LENGHT];
unsigned char ucUartTimerDataPacketFlag;
unsigned char ucUartPLCTimerPacketFinishedFlag;
unsigned char ucTimer4Counter=0;
//
volatile unsigned char last_enc_val_clk;
volatile unsigned char last_enc_val_dt;
volatile unsigned char uartFirstStartFlag;
//Function Definitions
void mainCheckUartHartBeat( UART_HandleTypeDef *huart){
	if(huart->Instance==HUART_PLC_INSTANCE){
		if(*sSytemVar.sUart.ucpUartPLCDataReadyFlag==UART_FL_SET)
			*sSytemVar.sUart.ucpUartPLCDataReadyFlag=UART_FL_RESET;
		*sSytemVar.sTimer.ucpTimer4Counter =0;// reset timer
	}
}
void mainCheckRotary(_SYS *sys){
	unsigned char enc_val_clk, enc_val_dt;
	unsigned char enc_inc, enc_dec;

	enc_val_clk = (uint8_t) HAL_GPIO_ReadPin(ROTARY_CLK_GPIO_Port,ROTARY_CLK_Pin);
	enc_val_dt = (uint8_t) HAL_GPIO_ReadPin(ROTARY_DT_GPIO_Port,ROTARY_DT_Pin);
	enc_inc = enc_val_clk ^ last_enc_val_dt;
	enc_dec = enc_val_dt ^ last_enc_val_clk;
	if(enc_inc){
		sSytemVar.sPeripheral.iRotaryCounter++;
		sSytemVar.sPeripheral.cRotaryOrientation='R';
	}
	if(enc_dec){
		sSytemVar.sPeripheral.iRotaryCounter--;
		sSytemVar.sPeripheral.cRotaryOrientation='L';
	}
	last_enc_val_clk = enc_val_clk;
	last_enc_val_dt = enc_val_dt;
}
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
	if(GPIO_Pin==ROTARY_CLK_Pin ||GPIO_Pin==ROTARY_DT_Pin){
		mainCheckRotary(&sSytemVar);
	}
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
	if(huart->Instance==HUART_PLC_INSTANCE){
			HAL_UART_Receive_IT(sSytemVar.sUart.hpUartPLC,sSytemVar.sUart.ucpUartPLCRxDataBuffer,PLC_UART_RX_BUFFER_LENGHT);
	}
}
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	if(htim->Instance==TIM4){	// for uart
		if(*sSytemVar.sUart.ucpUartPLCDataReadyFlag==UART_FL_RESET){
			(*sSytemVar.sTimer.ucpTimer4Counter)++;
			if(*sSytemVar.sTimer.ucpTimer4Counter>PLC_PACKET_FINISHED_COUNTER_LENGHT){
				*sSytemVar.sUart.ucpUartPLCPacketFinishedFlag=UART_FL_SET;
				*sSytemVar.sUart.ucpUartPLCDataReadyFlag=UART_FL_SET;
			}
		}
	}
	else if(htim->Instance==TIM5){

	}
	else if(htim->Instance==TIM6){

	}
	else if(htim->Instance==TIM7){

	}
	else if(htim->Instance==TIM8){

	}
}

void initTimer(void){
	//timer 4 for uart packet checking with 10 ms interval for 32 mHz
	htim4.Instance = TIM4;
	htim4.Init.Prescaler = 4;
	htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim4.Init.Period = 63999;
	HAL_TIM_Base_Init(&htim4);

	// timer 5 100 ms for 32 mHz
	htim5.Instance = TIM5;
	htim5.Init.Prescaler = 499;
	htim5.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim5.Init.Period = 63999;
	HAL_TIM_Base_Init(&htim5);

	// timer 6 1 second for 32 mHz
	htim6.Instance = TIM6;
	htim6.Init.Prescaler = 279;
	htim6.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim6.Init.Period = 63999;
	HAL_TIM_Base_Init(&htim6);

	// timer 7 500 ms for 32 mHz
	htim7.Instance = TIM7;
	htim7.Init.Prescaler = 249;
	htim7.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim7.Init.Period = 63999;
	HAL_TIM_Base_Init(&htim7);

	// timer 8 500 ms for 32 mHz
	htim8.Instance = TIM8;
	htim8.Init.Prescaler = 249;
	htim8.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim8.Init.Period = 63999;
	HAL_TIM_Base_Init(&htim8);
}

void initSystemPointerAddress(void){
	//UART
	sSytemVar.sUart.hpUartPLC=&HUART_PLC;
	sSytemVar.sUart.ucpUartPLCDataReadyFlag=&ucUartTimerDataPacketFlag;
	sSytemVar.sUart.ucpUartPLCPacketFinishedFlag=&ucUartPLCTimerPacketFinishedFlag;
	sSytemVar.sUart.ucpUartPLCRxDataBuffer=ucaPLCUartRxData;
	sSytemVar.sUart.ucpUartPLCTxDataBuffer=ucaPLCUartTxData;
	HAL_UART_Receive_IT(sSytemVar.sUart.hpUartPLC,sSytemVar.sUart.ucpUartPLCRxDataBuffer,PLC_UART_RX_BUFFER_LENGHT);
	//TIMER
	sSytemVar.sTimer.ucpTimer4Counter=&ucTimer4Counter;
}

void initInterrupts(){
	//init UARTS
	HAL_UART_Receive_IT(sSytemVar.sUart.hpUartPLC,sSytemVar.sUart.ucpUartPLCRxDataBuffer,PLC_UART_RX_BUFFER_LENGHT);
	//init TIMERS
	HAL_TIM_Base_Start_IT(&htim4);
}

void systemInit(void){
	*sSytemVar.sUart.ucpUartPLCDataReadyFlag=UART_FL_SET;
	initTimer();
	initSystemPointerAddress();
	initInterrupts();
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */
  

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_ADC1_Init();
  MX_DAC_Init();
  MX_I2C1_Init();
  MX_I2C3_Init();
  MX_SPI1_Init();
  MX_TIM1_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  MX_TIM5_Init();
  MX_TIM6_Init();
  MX_TIM7_Init();
  MX_TIM8_Init();
  MX_TIM10_Init();
  MX_TIM11_Init();
  MX_TIM12_Init();
  MX_UART4_Init();
  MX_UART5_Init();
  MX_UART8_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_USART3_UART_Init();
  MX_USART6_UART_Init();
  /* USER CODE BEGIN 2 */
	systemInit();
	prsInit();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  prsLoop();
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /** Configure the main internal regulator output voltage 
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 12;
  RCC_OscInitStruct.PLL.PLLN = 96;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV6;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USART1|RCC_PERIPHCLK_USART2
                              |RCC_PERIPHCLK_USART3|RCC_PERIPHCLK_USART6
                              |RCC_PERIPHCLK_UART4|RCC_PERIPHCLK_UART5
                              |RCC_PERIPHCLK_UART8|RCC_PERIPHCLK_I2C1
                              |RCC_PERIPHCLK_I2C3;
  PeriphClkInitStruct.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
  PeriphClkInitStruct.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
  PeriphClkInitStruct.Usart3ClockSelection = RCC_USART3CLKSOURCE_PCLK1;
  PeriphClkInitStruct.Uart4ClockSelection = RCC_UART4CLKSOURCE_PCLK1;
  PeriphClkInitStruct.Uart5ClockSelection = RCC_UART5CLKSOURCE_PCLK1;
  PeriphClkInitStruct.Usart6ClockSelection = RCC_USART6CLKSOURCE_PCLK2;
  PeriphClkInitStruct.Uart8ClockSelection = RCC_UART8CLKSOURCE_PCLK1;
  PeriphClkInitStruct.I2c1ClockSelection = RCC_I2C1CLKSOURCE_PCLK1;
  PeriphClkInitStruct.I2c3ClockSelection = RCC_I2C3CLKSOURCE_PCLK1;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
