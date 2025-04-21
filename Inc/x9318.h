#ifndef _X9318_H
#define _X9318_H
#include "gpio.h"
#include "main.h"

#define ON            GPIO_PIN_SET
#define OFF           GPIO_PIN_RESET

#define X9318_INC_H() HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, ON) // X9318 的INC定义为   GPIOC_Pin_5
#define X9318_INC_L() HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, OFF)
#define X9318_UD_D()  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, ON) // X9318 的UD定义为   GPIOC_Pin_6
#define X9318_UD_U()  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, OFF)
#define X9318_CS_H()  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, ON) // X9318 的CS定义为   GPIOC_Pin_7
#define X9318_CS_L()  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, OFF)

#define CH1_0()       HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, OFF)
#define CH1_1()       HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, ON)
#define Alarm_0()     HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, OFF)
#define Alarm_1()     HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, ON)

void X9318_Init(void);
void X9318_UP(void);
void X9318_DOWN(void);
void X9318_UP_1(void);
void X9318_DOWN_1(void);
void HAL_Delay_us(uint32_t delay);

uint8_t Enlarge_Control(float adc_vol);

#endif
