#ifndef __REMOTE_H__
#define __REMOTE_H__

#include "main.h"

#define IR_TIM htim2
#define IR_TIM_CHANNE TIM_CHANNEL_1

#define IR_us_LOGIC_0 560
#define IR_us_LOGIC_1 1640
#define IR_us_START 4480
#define IR_us_REPEAT_START 39350
#define IR_us_REPEAT_END 2220
#define IR_DEVIATION 100	//误差范围


#define IR_CHECK_ADDRESS 1  //1：检查地址位 0：取消
#define IR_CHECK_COMMAND 1  //1：检查数据位 0：取消
#define IR_CHECK_REPEAT 1   //1：检查重复   0：取消
void IR_Init(void);
void IR_CaptureCallback(void);
uint8_t IR_Scanf(uint32_t* data);
uint8_t keyscan(uint32_t temp);

#define ON  GPIO_PIN_SET
#define OFF GPIO_PIN_RESET

#define REMOTE_PORT              GPIOC                            // GPIO端口
#define REMOTE_CLK               __HAL_RCC_GPIOC_CLK_ENABLE()     // GPIO端口时钟
#define REMOTE_PIN               GPIO_PIN_10                     // 定义

#define REMOTE_0()                HAL_GPIO_WritePin(REMOTE_PORT, REMOTE_PIN, OFF) 
#define REMOTE_1()                HAL_GPIO_WritePin(REMOTE_PORT, REMOTE_PIN,ON)

#endif
/***************************************
 * 1        00FFA25D
 * 2        00FF629D
 * 3        00FFE21D
 * 4        00FF22DD
 * 5        00FF02FD
 * 6        00FFC23D
 * 7        00FFE01F
 * 8        00FFA857
 * 9        00FF906F
 * *        00FF6897
 * 0        00FF9867
 * #        00FFB04F
 * up       00FF18E7
 * left     00FF10EF
 * OK       00FF38C7
 * right    00FF5AA5
 * down     00FF4AB5
*****************************************/
/****************************************
 * 放入捕获中断回调函数中使用，按需修改TIM1
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	if (TIM1 == htim->Instance)
	{
		IR_CaptureCallback();
	}
}
******************************************/

