#ifndef _IST7918_H
#define _IST7918_H

#include "main.h"
#include "stm32l4xx.h"

#define IST7918_RS_PORT  GPIOB                        // GPIO端口
#define IST7918_RS_CLK   __HAL_RCC_GPIOB_CLK_ENABLE() // GPIO端口时钟
#define IST7918_RS_PIN   GPIO_PIN_12                  // 定义IST7918的RS管脚

#define IST7918_SCL_PORT GPIOB                        // GPIO端口
#define IST7918_SCL_CLK  __HAL_RCC_GPIOB_CLK_ENABLE() // GPIO端口时钟
#define IST7918_SCL_PIN  GPIO_PIN_13                  // 定义IST7918的SCL管脚

#define IST7918_SDA_PORT GPIOB                        // GPIO端口
#define IST7918_SDA_CLK  __HAL_RCC_GPIOB_CLK_ENABLE() // GPIO端口时钟
#define IST7918_SDA_PIN  GPIO_PIN_14                  // 定义IST7918的SDA管脚

#define IST7918_CS_PORT  GPIOB                        // GPIO端口
#define IST7918_CS_CLK   __HAL_RCC_GPIOB_CLK_ENABLE() // GPIO端口时钟
#define IST7918_CS_PIN   GPIO_PIN_15                  // 定义IST7918的CS管脚

#define IST7918_RES_PORT GPIOC                        // GPIO端口
#define IST7918_RES_CLK  __HAL_RCC_GPIOC_CLK_ENABLE() // GPIO端口时钟
#define IST7918_RES_PIN  GPIO_PIN_11                  // 定义IST7918的RES管脚

#define ON               GPIO_PIN_SET
#define OFF              GPIO_PIN_RESET

#define RS_0()           HAL_GPIO_WritePin(IST7918_RS_PORT, IST7918_RS_PIN, OFF)
#define RS_1()           HAL_GPIO_WritePin(IST7918_RS_PORT, IST7918_RS_PIN, ON)

#define SCL_0()          HAL_GPIO_WritePin(IST7918_SCL_PORT, IST7918_SCL_PIN, OFF)
#define SCL_1()          HAL_GPIO_WritePin(IST7918_SCL_PORT, IST7918_SCL_PIN, ON)

#define SDA_0()          HAL_GPIO_WritePin(IST7918_SDA_PORT, IST7918_SDA_PIN, OFF)
#define SDA_1()          HAL_GPIO_WritePin(IST7918_SDA_PORT, IST7918_SDA_PIN, ON)

#define CS_0()           HAL_GPIO_WritePin(IST7918_CS_PORT, IST7918_CS_PIN, OFF)
#define CS_1()           HAL_GPIO_WritePin(IST7918_CS_PORT, IST7918_CS_PIN, ON)

#define RES_0()          HAL_GPIO_WritePin(IST7918_RES_PORT, IST7918_RES_PIN, OFF)
#define RES_1()          HAL_GPIO_WritePin(IST7918_RES_PORT, IST7918_RES_PIN, ON)

extern uint8_t Addr, thshold, channel, page, Sleep_Time; // 地址，阈值，信道，页，休眠时间
extern float Resistance, old_Resistance;
extern uint8_t data_1[10];
extern uint8_t flag_page, page;
extern float Resistance;
void LCD_init(void);
void LCD_Clear(void);
void showpic(const unsigned char *k);
void write_com(int para);
void write_data(int para);

void LCD_Display(float RG);                              // 信号显示
void Resistance_Display(float Resistance, uint8_t page); // 电阻显示
void Remote_Set(uint8_t temp);                           // 红外遥控显示

void LCD_EN8X16(unsigned char x, unsigned char y, uint8_t num);
void LCD_CH16X16(unsigned char x, unsigned char y, uint8_t num);
void LCD_EN16X32(unsigned char x, unsigned char y, uint8_t num);
#endif
