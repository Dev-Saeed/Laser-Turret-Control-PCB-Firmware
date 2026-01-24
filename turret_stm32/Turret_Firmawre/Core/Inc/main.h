/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
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
#include "stm32f0xx_hal.h"

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
#define K1_Pin GPIO_PIN_1
#define K1_GPIO_Port GPIOA
#define K2_Pin GPIO_PIN_2
#define K2_GPIO_Port GPIOA
#define K3_Pin GPIO_PIN_3
#define K3_GPIO_Port GPIOA
#define K4_Pin GPIO_PIN_4
#define K4_GPIO_Port GPIOA
#define K5_Pin GPIO_PIN_5
#define K5_GPIO_Port GPIOA
#define K6_Pin GPIO_PIN_6
#define K6_GPIO_Port GPIOA
#define LedB_Pin GPIO_PIN_0
#define LedB_GPIO_Port GPIOB
#define IO8_Pin GPIO_PIN_13
#define IO8_GPIO_Port GPIOB
#define IO7_Pin GPIO_PIN_14
#define IO7_GPIO_Port GPIOB
#define IO6_Pin GPIO_PIN_15
#define IO6_GPIO_Port GPIOB
#define IO5_Pin GPIO_PIN_8
#define IO5_GPIO_Port GPIOA
#define IO4_Pin GPIO_PIN_9
#define IO4_GPIO_Port GPIOA
#define IO3_Pin GPIO_PIN_10
#define IO3_GPIO_Port GPIOA
#define IO2_Pin GPIO_PIN_11
#define IO2_GPIO_Port GPIOA
#define IO1_Pin GPIO_PIN_12
#define IO1_GPIO_Port GPIOA
#define L1_Pin GPIO_PIN_15
#define L1_GPIO_Port GPIOA
#define L2_Pin GPIO_PIN_3
#define L2_GPIO_Port GPIOB
#define L3_Pin GPIO_PIN_4
#define L3_GPIO_Port GPIOB
#define L4_Pin GPIO_PIN_5
#define L4_GPIO_Port GPIOB
#define L5_Pin GPIO_PIN_6
#define L5_GPIO_Port GPIOB
#define L6_Pin GPIO_PIN_7
#define L6_GPIO_Port GPIOB
#define Switch_Pin GPIO_PIN_8
#define Switch_GPIO_Port GPIOB
#define LedA_Pin GPIO_PIN_9
#define LedA_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
