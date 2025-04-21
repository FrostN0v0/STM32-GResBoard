/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file    usart.c
 * @brief   This file provides code for the configuration
 *          of the USART instances.
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
/* Includes ------------------------------------------------------------------*/
#include "usart.h"

/* USER CODE BEGIN 0 */
#include "gpio.h"
/* USER CODE END 0 */

UART_HandleTypeDef huart1;

/* USART1 init function */

void MX_USART1_UART_Init(void)
{

    /* USER CODE BEGIN USART1_Init 0 */

    /* USER CODE END USART1_Init 0 */

    /* USER CODE BEGIN USART1_Init 1 */

    /* USER CODE END USART1_Init 1 */
    huart1.Instance                    = USART1;
    huart1.Init.BaudRate               = 9600;
    huart1.Init.WordLength             = UART_WORDLENGTH_8B;
    huart1.Init.StopBits               = UART_STOPBITS_1;
    huart1.Init.Parity                 = UART_PARITY_NONE;
    huart1.Init.Mode                   = UART_MODE_TX_RX;
    huart1.Init.HwFlowCtl              = UART_HWCONTROL_NONE;
    huart1.Init.OverSampling           = UART_OVERSAMPLING_16;
    huart1.Init.OneBitSampling         = UART_ONE_BIT_SAMPLE_DISABLE;
    huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
    if (HAL_UART_Init(&huart1) != HAL_OK) { Error_Handler(); }
    /* USER CODE BEGIN USART1_Init 2 */

    /* USER CODE END USART1_Init 2 */
}

void HAL_UART_MspInit(UART_HandleTypeDef *uartHandle)
{

    GPIO_InitTypeDef GPIO_InitStruct       = {0};
    RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};
    if (uartHandle->Instance == USART1) {
        /* USER CODE BEGIN USART1_MspInit 0 */

        /* USER CODE END USART1_MspInit 0 */

        /** Initializes the peripherals clock
         */
        PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1;
        PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_HSI;
        if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK) { Error_Handler(); }

        /* USART1 clock enable */
        __HAL_RCC_USART1_CLK_ENABLE();

        __HAL_RCC_GPIOA_CLK_ENABLE();
        /**USART1 GPIO Configuration
        PA9     ------> USART1_TX
        PA10     ------> USART1_RX
        */
        GPIO_InitStruct.Pin       = TXD_1_Pin | RXD_1_Pin;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull      = GPIO_NOPULL;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        /* USER CODE BEGIN USART1_MspInit 1 */

        /* USER CODE END USART1_MspInit 1 */
    }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef *uartHandle)
{

    if (uartHandle->Instance == USART1) {
        /* USER CODE BEGIN USART1_MspDeInit 0 */

        /* USER CODE END USART1_MspDeInit 0 */
        /* Peripheral clock disable */
        __HAL_RCC_USART1_CLK_DISABLE();

        /**USART1 GPIO Configuration
        PA9     ------> USART1_TX
        PA10     ------> USART1_RX
        */
        HAL_GPIO_DeInit(GPIOA, TXD_1_Pin | RXD_1_Pin);

        /* USER CODE BEGIN USART1_MspDeInit 1 */

        /* USER CODE END USART1_MspDeInit 1 */
    }
}

/* USER CODE BEGIN 1 */

#include <stdio.h>
#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif
PUTCHAR_PROTOTYPE
{
    HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xFFFF);
    return ch;
}

void Lora_Control(uint8_t mode)
{
    switch (mode) {
        case 0: // 无线模块模式3，深度休眠模式，M0=1,M1=1
            LoraM0_1;
            LoraM1_1;
            break;
        case 1: // 无线模块模式0，数据传输模式，M0=0,M1=0
            LoraM0_0;
            LoraM1_0;
            break;
        case 2: // 无线模块模式2，配置模式，设置模块地地址和信道，M0=0,M1=1
            LoraM0_0;
            LoraM1_1;
            break;
        case 3: // 无线模块模式1，WOR模式，，M0=1,M1=0
            LoraM0_1;
            LoraM1_0;
            break;
        default:
            break;
    }
}
unsigned int GetCRC16(unsigned char *ptr, unsigned char len)
{
    uint16_t index;
    uint8_t crcl   = 0xFF; // 高CRC字节
    uint8_t crch   = 0xFF; // 低CRC字节
    uint8_t TabH[] = {     // CRC高位字节值表
                      0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
                      0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
                      0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
                      0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
                      0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
                      0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
                      0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
                      0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
                      0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
                      0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
                      0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
                      0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
                      0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
                      0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
                      0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
                      0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40};
    uint8_t TabL[] = {// CRC低位字节值表
                      0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 0x07, 0xC7, 0x05, 0xC5, 0xC4, 0x04,
                      0xCC, 0x0C, 0x0D, 0xCD, 0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09, 0x08, 0xC8,
                      0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A, 0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC,
                      0x14, 0xD4, 0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3, 0x11, 0xD1, 0xD0, 0x10,
                      0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3, 0xF2, 0x32, 0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4,
                      0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A, 0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38,
                      0x28, 0xE8, 0xE9, 0x29, 0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF, 0x2D, 0xED, 0xEC, 0x2C,
                      0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26, 0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0,
                      0xA0, 0x60, 0x61, 0xA1, 0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67, 0xA5, 0x65, 0x64, 0xA4,
                      0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F, 0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68,
                      0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA, 0xBE, 0x7E, 0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C,
                      0xB4, 0x74, 0x75, 0xB5, 0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 0x70, 0xB0,
                      0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92, 0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54,
                      0x9C, 0x5C, 0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B, 0x99, 0x59, 0x58, 0x98,
                      0x88, 0x48, 0x49, 0x89, 0x4B, 0x8B, 0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
                      0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 0x43, 0x83, 0x41, 0x81, 0x80, 0x40};

    while (len--) // 计算指定长度的CRC
    {
        index = crcl ^ *ptr++;
        crcl  = crch ^ TabH[index];
        crch  = TabL[index];
    }

    return ((crch << 8) | crcl);
};

/**
 * @brief 上传Lora数据。
 *
 * 此函数根据给定的地址、信道、电阻和模式上传Lora数据。
 * 它根据模式和电阻值以不同的格式格式化数据。
 *
 * @param Addr 地址。
 * @param channel 信道。
 * @param Resistance 电阻值。
 * @param mode 数据上传模式。
 *             - 如果模式为0，数据以二进制格式上传。
 *             - 如果模式为1，数据以JSON文本格式上传。
 *
 * 此函数执行以下操作：
 * - 根据预定义的范围将电阻值转换为临时值。
 * - 如果模式为0，将数据格式化为二进制数组，计算CRC，并输出。
 * - 如果模式为1，将数据格式化为JSON字符串并输出。
 */
void Lora_Data_UPload(uint8_t Addr, uint8_t channel, float Resistance, uint8_t mode)
{
    uint8_t Lora_Data[8], alarm;
    uint16_t calCRC;
    uint16_t Res_temp;
    if (Resistance < 10) {
        Res_temp = 10000 + Resistance * 1000;
    } else if ((Resistance >= 10) && (Resistance < 100)) {
        Res_temp = 20000 + Resistance * 100;
    } else if ((Resistance >= 100) && (Resistance < 200)) {
        Res_temp = 30000 + Resistance * 10;
    } else {
        Res_temp = 50000;
    }

    alarm = 0;
    if (mode == 0) {
        Lora_Data[0] = Addr;    // 地址
        Lora_Data[1] = channel; // 信道
        Lora_Data[2] = 3;
        Lora_Data[3] = alarm;                                                                 // 报警
        Lora_Data[4] = Res_temp / 256;                                                        // 电阻值高使
        Lora_Data[5] = Res_temp % 256;                                                        // 电阻值低使
        sprintf((char *)Lora_Data, "%c%c%c%c%c", Addr, 3, 2, Res_temp / 256, Res_temp % 256); // channel ,alarm
        calCRC       = GetCRC16(Lora_Data, 5);
        Lora_Data[5] = (calCRC >> 8) & 0xFF; // CRC高位
        Lora_Data[6] = (calCRC) & 0xFF;      // CRC低位

        printf("%c%c%c%c%c%c%c", Lora_Data[0], Lora_Data[1], Lora_Data[2], Lora_Data[3], Lora_Data[4], (calCRC) & 0xFF,
               (calCRC >> 8) & 0xFF); //
    } else if (mode == 1) {
        if (Resistance < 10) {
            printf("{\"ts\":\"\",\"d\":[{\"tag\":\"%03d\",\"value\":%0.3f}]\n}", Addr,
                   Resistance); // 文本格式
        } else if ((Resistance >= 10) && (Resistance < 100)) {
            printf("{\"ts\":\"\",\"d\":[{\"tag\":\"%03d\",\"value\":%0.2f}]\n}", Addr,
                   Resistance); // 文本格式
        } else if ((Resistance >= 100) && (Resistance < 110)) {
            printf("{\"ts\":\"\",\"d\":[{\"tag\":\"%03d\",\"value\":%0.1f}]\n}", Addr,
                   Resistance); // 文本格式
        } else {
            Resistance = 200.0;
            printf("{\"ts\":\"\",\"d\":[{\"tag\":\"%03d\",\"value\":%0.1f}]\n}", Addr,
                   Resistance); // 文本格式
        }
    }
}

/* USER CODE END 1 */
