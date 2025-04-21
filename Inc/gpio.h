/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    gpio.h
  * @brief   This file contains all the function prototypes for
  *          the gpio.c file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#ifndef __GPIO_H__
#define __GPIO_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

void MX_GPIO_Init(void);

/* USER CODE BEGIN Prototypes */
void GPIO_Reset_Init(void);
#define ON  GPIO_PIN_SET
#define OFF GPIO_PIN_RESET

#define Power_0                  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0  , OFF) 
#define Power_1                  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0  ,ON)

#define LoraM0_0                 HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8  ,OFF) 
#define LoraM0_1                 HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8  ,ON)

#define LoraM1_0                 HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9  ,OFF) 
#define LoraM1_1                 HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9  ,ON)

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif
#endif /*__ GPIO_H__ */

