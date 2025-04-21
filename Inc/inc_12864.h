/*
 * lcd12864.h
 *
 *  Created on: Aug 13, 2021
 *      Author: along
 */
/*
 * ע����ʾ�����泯�ϣ�������һ�࿿���Լ����Ҳ��һ��ΪPin1������һ��ΪPin16
 * Pin7��SCLK
 * Pin8��SDA
 * Pin9��VCC(3.3V)
 * Pin10��GND
 * Pin11�������ѹ����(3.3V)
 * Pin12��Ƭѡ�ź�CS(����Ч)
 * Pin13����λ��RESET
 * Pin14������������л�RS
 *
 * 									LCD12864-->STM32F407ZET6     User Label
 * 		 								  CS-->PA1				   LCD_CS
 * 		 								  RS-->PA2				   LCD_RS
 * 	  								  RESET-->PA3				  LCD_RESET
 * 	   								  SCLK-->PB10				  LCD_SCLK
 * 										  SDA-->PB11				   LCD_SDA
 */


#ifndef INC_LCD12864_H_
#define INC_LCD12864_H_

#include "gpio.h"


#define CS_0 HAL_GPIO_WritePin(GPIOA, LCD_CS_Pin, GPIO_PIN_RESET)
#define CS_1 HAL_GPIO_WritePin(GPIOA, LCD_CS_Pin, GPIO_PIN_SET)
#define RS_0 HAL_GPIO_WritePin(GPIOA, LCD_RS_Pin, GPIO_PIN_RESET)
#define RS_1 HAL_GPIO_WritePin(GPIOA, LCD_RS_Pin, GPIO_PIN_SET)
#define RESET_0 HAL_GPIO_WritePin(GPIOA, LCD_RESET_Pin, GPIO_PIN_RESET)
#define RESET_1 HAL_GPIO_WritePin(GPIOA, LCD_RESET_Pin, GPIO_PIN_SET)
#define SCLK_0 HAL_GPIO_WritePin(GPIOB, LCD_SCLK_Pin, GPIO_PIN_RESET)
#define SCLK_1 HAL_GPIO_WritePin(GPIOB, LCD_SCLK_Pin, GPIO_PIN_SET)
#define SDA_0 HAL_GPIO_WritePin(GPIOB, LCD_SDA_Pin, GPIO_PIN_RESET)
#define SDA_1 HAL_GPIO_WritePin(GPIOB, LCD_SDA_Pin, GPIO_PIN_SET)

#define LCD_CS_Pin                 GPIO_PIN_1
#define LCD_RS_Pin                GPIO_PIN_2 
#define LCD_RESET_Pin               GPIO_PIN_3
#define LCD_SCLK_Pin                GPIO_PIN_10 
#define LCD_SDA_Pin                GPIO_PIN_11


void Delay(unsigned short tick);
void InitLCD(void);
void WriteToLCD(unsigned char data, unsigned char flg);
void ClearScreen(void);
void LCDAddr(unsigned char page,unsigned char column);
void DisplayStr(unsigned short page,unsigned short column,char *text);
void DisplayChinese(unsigned char page,unsigned char column,unsigned char no);


#endif /* INC_LCD12864_H_ */

