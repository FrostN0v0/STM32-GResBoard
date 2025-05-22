#ifndef __FLASH_H
#define __FLASH_H
#include "stm32l4xx_hal.h"


/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define LEN						10
/* USER CODE END PD */

extern uint64_t remote_buf[LEN];
void Flash_Test(void);
void remote_data(uint64_t data_0,uint64_t data_1,uint64_t data_2,uint64_t data_3, uint64_t data_4);//偏移地址+数据
//数据存储 偏移地址+数据
void remote_read();   //数据读取
void Flash_Erase(void);

#endif


