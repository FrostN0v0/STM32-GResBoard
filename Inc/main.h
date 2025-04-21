/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
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
#include "stm32l4xx_hal.h"

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
#define POWER_EN_Pin GPIO_PIN_0
#define POWER_EN_GPIO_Port GPIOC
#define ADC_Pin GPIO_PIN_4
#define ADC_GPIO_Port GPIOA
#define CH1_Pin GPIO_PIN_5
#define CH1_GPIO_Port GPIOA
#define WKUP_Pin GPIO_PIN_5  //GPIOC
#define WKUP_GPIO_Port GPIOC
#define WKUP_EXTI_IRQn EXTI9_5_IRQn
#define ALARM_Pin GPIO_PIN_0
#define ALARM_GPIO_Port GPIOB
#define AO_Pin GPIO_PIN_12
#define AO_GPIO_Port GPIOB
#define SCL_Pin GPIO_PIN_13
#define SCL_GPIO_Port GPIOB
#define SDA_Pin GPIO_PIN_14
#define SDA_GPIO_Port GPIOB
#define CS_Pin GPIO_PIN_15
#define CS_GPIO_Port GPIOB
#define PWM_Pin GPIO_PIN_8
#define PWM_GPIO_Port GPIOA
#define TXD_1_Pin GPIO_PIN_9
#define TXD_1_GPIO_Port GPIOA
#define RXD_1_Pin GPIO_PIN_10
#define RXD_1_GPIO_Port GPIOA
#define REMOTE_IN_Pin GPIO_PIN_15
#define REMOTE_IN_GPIO_Port GPIOA
#define HS0038_EN_Pin GPIO_PIN_10
#define HS0038_EN_GPIO_Port GPIOC
#define RES_Pin GPIO_PIN_11
#define RES_GPIO_Port GPIOC
#define INC_1_Pin GPIO_PIN_5
#define INC_1_GPIO_Port GPIOB
#define U_D_1_Pin GPIO_PIN_6
#define U_D_1_GPIO_Port GPIOB
#define CS_1_Pin GPIO_PIN_7
#define CS_1_GPIO_Port GPIOB
#define M0_IO_Pin GPIO_PIN_8
#define M0_IO_GPIO_Port GPIOB
#define M1_IO_Pin GPIO_PIN_9
#define M1_IO_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
