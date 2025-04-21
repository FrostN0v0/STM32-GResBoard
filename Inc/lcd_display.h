#ifndef _LCD_DISPLAY_H
#define _LCD_DISPLAY_H
//#ifndef _SPI2_H
//#define _SPI2_H
#include "stm32l4xx_hal.h"
#define HT1621_WR GPIO_PIN_14
//#define HT1621_RD GPIO_PIN_1
#define HT1621_CS GPIO_PIN_13
#define HT1621_DATA GPIO_PIN_12
//#define HT1621_IRQ GPIO_PIN_7

#define WR_H HAL_GPIO_WritePin(GPIOB, HT1621_WR,GPIO_PIN_SET)
#define WR_L HAL_GPIO_WritePin(GPIOB, HT1621_WR,GPIO_PIN_RESET)
#define DATA_H HAL_GPIO_WritePin(GPIOB, HT1621_DATA,GPIO_PIN_SET)
#define DATA_L HAL_GPIO_WritePin(GPIOB, HT1621_DATA,GPIO_PIN_RESET)
#define CS_H HAL_GPIO_WritePin(GPIOB, HT1621_CS,GPIO_PIN_SET)
#define CS_L HAL_GPIO_WritePin(GPIOB, HT1621_CS,GPIO_PIN_RESET)

#define BIAS   0x29            //0b1000 0010 1000  1/3duty 4com
#define SYSDIS 0X00           //0b1000 0000 0000  关闭系统振荡器和LCD偏置发生器
#define SYSEN  0X01           //0b1000 0000 0001 打开系统振荡器
#define LCDOFF 0X02           //0b1000 0000 0010  关闭LCD电压
#define LCDON  0X03           //0b1000 0000 0011  打开LCD偏压
#define XTAL   0x14           //0b1000 0001 0100 外部接时钟
#define RC256  0X18           //0b1000 0001 1000 内部时钟RC256K
#define TONEON  0X09          //0b1000 0000 1001  打开声音输出
#define TONEOFF 0X08           //0b1000 0000 1000 关闭声音输出
#define WDTDIS  0X05        //0b1000 0000 0101  禁止看门狗
#define TONE_4K 0x40        //4k0100 0000 0000  4K声音频率输出
#define TONE_2K 0x60    

#define uint8_t unsigned char
#define uint32_t unsigned int

#define DATA 1
#define COMMAND 0

extern void write_mode(uint8_t mode);
extern void write_add(uint8_t addr);
extern void write_data_4bit(uint8_t data);
extern void write_data_8bit(uint8_t data);
extern void write_command(uint8_t command);
extern void ht1621_init(void);
extern void lcd_all(void);
extern void clearpanel(void);
extern void write_addr_dat_n(unsigned char _addr, unsigned char _dat, unsigned char n);
#endif
