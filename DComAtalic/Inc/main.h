/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f7xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define PWRKEY_uC_Pin GPIO_PIN_2
#define PWRKEY_uC_GPIO_Port GPIOE
#define RESET_uC_Pin GPIO_PIN_3
#define RESET_uC_GPIO_Port GPIOE
#define GPS_ENTEN_ACTIVE_Pin GPIO_PIN_4
#define GPS_ENTEN_ACTIVE_GPIO_Port GPIOE
#define DO3_Pin GPIO_PIN_5
#define DO3_GPIO_Port GPIOE
#define DO2_Pin GPIO_PIN_6
#define DO2_GPIO_Port GPIOE
#define DO1_Pin GPIO_PIN_15
#define DO1_GPIO_Port GPIOC
#define ADC1__AI_OPAMP1_Pin GPIO_PIN_0
#define ADC1__AI_OPAMP1_GPIO_Port GPIOC
#define DO0_Pin GPIO_PIN_3
#define DO0_GPIO_Port GPIOC
#define ADC1_AI_OPAMP4_Pin GPIO_PIN_0
#define ADC1_AI_OPAMP4_GPIO_Port GPIOA
#define ADC1_AI_OPAMP3_Pin GPIO_PIN_1
#define ADC1_AI_OPAMP3_GPIO_Port GPIOA
#define ADC1_AI_OPAMP2_Pin GPIO_PIN_2
#define ADC1_AI_OPAMP2_GPIO_Port GPIOA
#define ESP_IOPE15_Pin GPIO_PIN_6
#define ESP_IOPE15_GPIO_Port GPIOA
#define POWER_GSM_Pin GPIO_PIN_7
#define POWER_GSM_GPIO_Port GPIOA
#define LD5_Pin GPIO_PIN_4
#define LD5_GPIO_Port GPIOC
#define LD4_Pin GPIO_PIN_5
#define LD4_GPIO_Port GPIOC
#define LD3_Pin GPIO_PIN_0
#define LD3_GPIO_Port GPIOB
#define ROTARY_SW_Pin GPIO_PIN_9
#define ROTARY_SW_GPIO_Port GPIOE
#define DAC2_CS_Pin GPIO_PIN_8
#define DAC2_CS_GPIO_Port GPIOD
#define DAC1_CS_Pin GPIO_PIN_9
#define DAC1_CS_GPIO_Port GPIOD
#define LD1_Pin GPIO_PIN_12
#define LD1_GPIO_Port GPIOD
#define LD1D13_Pin GPIO_PIN_13
#define LD1D13_GPIO_Port GPIOD
#define LD2_Pin GPIO_PIN_14
#define LD2_GPIO_Port GPIOD
#define I2C3_SDA__INTERNAL_Pin GPIO_PIN_9
#define I2C3_SDA__INTERNAL_GPIO_Port GPIOC
#define I2C3_SCL__INTERNAL_Pin GPIO_PIN_8
#define I2C3_SCL__INTERNAL_GPIO_Port GPIOA
#define USART5_CS_Pin GPIO_PIN_9
#define USART5_CS_GPIO_Port GPIOA
#define USART4_CS_Pin GPIO_PIN_10
#define USART4_CS_GPIO_Port GPIOA
#define EEPROM_CS_Pin GPIO_PIN_15
#define EEPROM_CS_GPIO_Port GPIOA
#define USART8_CS_Pin GPIO_PIN_10
#define USART8_CS_GPIO_Port GPIOC
#define DI_0_Pin GPIO_PIN_11
#define DI_0_GPIO_Port GPIOC
#define DI_0_EXTI_IRQn EXTI15_10_IRQn
#define DI_1_Pin GPIO_PIN_12
#define DI_1_GPIO_Port GPIOC
#define DI_1_EXTI_IRQn EXTI15_10_IRQn
#define DI_2_Pin GPIO_PIN_0
#define DI_2_GPIO_Port GPIOD
#define DI_2_EXTI_IRQn EXTI0_IRQn
#define DI_3_Pin GPIO_PIN_1
#define DI_3_GPIO_Port GPIOD
#define DI_3_EXTI_IRQn EXTI1_IRQn
#define DIPSWITCH1_Pin GPIO_PIN_2
#define DIPSWITCH1_GPIO_Port GPIOD
#define ESP_IO_16_Pin GPIO_PIN_3
#define ESP_IO_16_GPIO_Port GPIOD
#define ESP_IO_16_EXTI_IRQn EXTI3_IRQn
#define ESP_IO_17_Pin GPIO_PIN_4
#define ESP_IO_17_GPIO_Port GPIOD
#define ESP_IO_17_EXTI_IRQn EXTI4_IRQn
#define DIPSWITCH0_Pin GPIO_PIN_6
#define DIPSWITCH0_GPIO_Port GPIOD
#define ROTARY_CLK_Pin GPIO_PIN_8
#define ROTARY_CLK_GPIO_Port GPIOB
#define ROTARY_CLK_EXTI_IRQn EXTI9_5_IRQn
#define ROTARY_DT_Pin GPIO_PIN_9
#define ROTARY_DT_GPIO_Port GPIOB
#define ROTARY_DT_EXTI_IRQn EXTI9_5_IRQn
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
