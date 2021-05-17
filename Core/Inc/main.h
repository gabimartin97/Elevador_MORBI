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
#include "stm32f1xx_hal.h"

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

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LED_Pin GPIO_PIN_13
#define LED_GPIO_Port GPIOC
#define LED2_Pin GPIO_PIN_14
#define LED2_GPIO_Port GPIOC
#define LCD_D4_Pin GPIO_PIN_4
#define LCD_D4_GPIO_Port GPIOA
#define LCD_D5_Pin GPIO_PIN_5
#define LCD_D5_GPIO_Port GPIOA
#define LCD_D6_Pin GPIO_PIN_6
#define LCD_D6_GPIO_Port GPIOA
#define LCD_D7_Pin GPIO_PIN_7
#define LCD_D7_GPIO_Port GPIOA
#define Sensor_rejillas_Pin GPIO_PIN_1
#define Sensor_rejillas_GPIO_Port GPIOB
#define Fc_Inf_Pin GPIO_PIN_10
#define Fc_Inf_GPIO_Port GPIOB
#define Fc_Sup_Pin GPIO_PIN_11
#define Fc_Sup_GPIO_Port GPIOB
#define PUL_Pin GPIO_PIN_12
#define PUL_GPIO_Port GPIOB
#define DIR_Pin GPIO_PIN_13
#define DIR_GPIO_Port GPIOB
#define ENA_Pin GPIO_PIN_14
#define ENA_GPIO_Port GPIOB
#define LCD_RS_Pin GPIO_PIN_8
#define LCD_RS_GPIO_Port GPIOA
#define LCD_RW_Pin GPIO_PIN_9
#define LCD_RW_GPIO_Port GPIOA
#define LCD_EN_Pin GPIO_PIN_10
#define LCD_EN_GPIO_Port GPIOA
#define PULSADOR1_Pin GPIO_PIN_5
#define PULSADOR1_GPIO_Port GPIOB
#define PULSADOR2_Pin GPIO_PIN_7
#define PULSADOR2_GPIO_Port GPIOB
#define PULSADOR3_Pin GPIO_PIN_8
#define PULSADOR3_GPIO_Port GPIOB
#define PULSADOR4_Pin GPIO_PIN_9
#define PULSADOR4_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
