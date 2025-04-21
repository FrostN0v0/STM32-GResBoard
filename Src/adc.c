/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file    adc.c
 * @brief   This file provides code for the configuration
 *          of the ADC instances.
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
#include "adc.h"

/* USER CODE BEGIN 0 */
#include "X9318.h"

/* USER CODE END 0 */

ADC_HandleTypeDef hadc1;

/* ADC1 init function */
void MX_ADC1_Init(void)
{

    /* USER CODE BEGIN ADC1_Init 0 */

    /* USER CODE END ADC1_Init 0 */

    ADC_ChannelConfTypeDef sConfig = {0};

    /* USER CODE BEGIN ADC1_Init 1 */

    /* USER CODE END ADC1_Init 1 */

    /** Common config
     */
    hadc1.Instance                   = ADC1;
    hadc1.Init.ClockPrescaler        = ADC_CLOCK_ASYNC_DIV1;
    hadc1.Init.Resolution            = ADC_RESOLUTION_12B;
    hadc1.Init.DataAlign             = ADC_DATAALIGN_RIGHT;
    hadc1.Init.ScanConvMode          = ADC_SCAN_DISABLE;
    hadc1.Init.EOCSelection          = ADC_EOC_SINGLE_CONV;
    hadc1.Init.LowPowerAutoWait      = DISABLE;
    hadc1.Init.ContinuousConvMode    = DISABLE;
    hadc1.Init.NbrOfConversion       = 1;
    hadc1.Init.DiscontinuousConvMode = DISABLE;
    hadc1.Init.ExternalTrigConv      = ADC_SOFTWARE_START;
    hadc1.Init.ExternalTrigConvEdge  = ADC_EXTERNALTRIGCONVEDGE_NONE;
    hadc1.Init.DMAContinuousRequests = DISABLE;
    hadc1.Init.Overrun               = ADC_OVR_DATA_PRESERVED;
    hadc1.Init.OversamplingMode      = DISABLE;
    if (HAL_ADC_Init(&hadc1) != HAL_OK) { Error_Handler(); }

    /** Configure Regular Channel
     */
    sConfig.Channel      = ADC_CHANNEL_9;
    sConfig.Rank         = ADC_REGULAR_RANK_1;
    sConfig.SamplingTime = ADC_SAMPLETIME_2CYCLES_5;
    sConfig.SingleDiff   = ADC_SINGLE_ENDED;
    sConfig.OffsetNumber = ADC_OFFSET_NONE;
    sConfig.Offset       = 0;
    if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK) { Error_Handler(); }
    /* USER CODE BEGIN ADC1_Init 2 */

    /* USER CODE END ADC1_Init 2 */
}

void HAL_ADC_MspInit(ADC_HandleTypeDef *adcHandle)
{

    GPIO_InitTypeDef GPIO_InitStruct       = {0};
    RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};
    if (adcHandle->Instance == ADC1) {
        /* USER CODE BEGIN ADC1_MspInit 0 */

        /* USER CODE END ADC1_MspInit 0 */

        /** Initializes the peripherals clock
         */
        PeriphClkInit.PeriphClockSelection    = RCC_PERIPHCLK_ADC;
        PeriphClkInit.AdcClockSelection       = RCC_ADCCLKSOURCE_PLLSAI1;
        PeriphClkInit.PLLSAI1.PLLSAI1Source   = RCC_PLLSOURCE_HSI;
        PeriphClkInit.PLLSAI1.PLLSAI1M        = 2;
        PeriphClkInit.PLLSAI1.PLLSAI1N        = 8;
        PeriphClkInit.PLLSAI1.PLLSAI1P        = RCC_PLLP_DIV7;
        PeriphClkInit.PLLSAI1.PLLSAI1Q        = RCC_PLLQ_DIV2;
        PeriphClkInit.PLLSAI1.PLLSAI1R        = RCC_PLLR_DIV2;
        PeriphClkInit.PLLSAI1.PLLSAI1ClockOut = RCC_PLLSAI1_ADC1CLK;
        if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK) { Error_Handler(); }

        /* ADC1 clock enable */
        __HAL_RCC_ADC_CLK_ENABLE();

        __HAL_RCC_GPIOA_CLK_ENABLE();
        /**ADC1 GPIO Configuration
        PA4     ------> ADC1_IN9
        */
        GPIO_InitStruct.Pin  = ADC_Pin;
        GPIO_InitStruct.Mode = GPIO_MODE_ANALOG_ADC_CONTROL;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        HAL_GPIO_Init(ADC_GPIO_Port, &GPIO_InitStruct);

        /* USER CODE BEGIN ADC1_MspInit 1 */

        /* USER CODE END ADC1_MspInit 1 */
    }
}

void HAL_ADC_MspDeInit(ADC_HandleTypeDef *adcHandle)
{

    if (adcHandle->Instance == ADC1) {
        /* USER CODE BEGIN ADC1_MspDeInit 0 */

        /* USER CODE END ADC1_MspDeInit 0 */
        /* Peripheral clock disable */
        __HAL_RCC_ADC_CLK_DISABLE();

        /**ADC1 GPIO Configuration
        PA4     ------> ADC1_IN9
        */
        HAL_GPIO_DeInit(ADC_GPIO_Port, ADC_Pin);

        /* USER CODE BEGIN ADC1_MspDeInit 1 */

        /* USER CODE END ADC1_MspDeInit 1 */
    }
}

/* USER CODE BEGIN 1 */

/* USER CODE BEGIN 3 */

/***************************
 *函数名称：uint8_t Get_Adc_Average(void)
 *函数功能：获取ADC采样电压
 *返回值：  返回ADC电压
 *备注�?
 ****************************/
float Get_Adc_Average(uint8_t times)
{
    //		uint16_t adc_value=0;						  //ADC采样
    //		float adc_vol=0.0f;								//ADC采样后转换的电压
    //		for(int i = 0; i < 4096; i++)
    //    {
    //        if(i%1024 == 0)
    //        {
    //            /* 使用ADC采样 */
    //            HAL_ADC_Start(&hadc1);	                //启动ADC单次转换
    //            HAL_ADC_PollForConversion(&hadc1, 50);	//等待ADC转换完成
    //            adc_value = HAL_ADC_GetValue(&hadc1); 	//读取ADC转换数据
    //					  adc_vol = ((double)adc_value/4096)*3.3; //转换为电压忿
    //        }
    //    }
    //		return adc_vol;

    uint32_t temp_val = 0;
    uint8_t i;
    float adc_vol = 0.00f;
    for (i = 0; i < times; i++) {
        HAL_ADC_Start(&hadc1);                 // 启动ADC单次转换
        HAL_ADC_PollForConversion(&hadc1, 50); // 等待ADC转换完成
        temp_val += HAL_ADC_GetValue(&hadc1);  // 读取ADC转换数据并累�?
    }
    adc_vol = (float)temp_val / times;
    return adc_vol;

    //		uint32_t sum=0 ,	Average_Adc=0;
    //	  float temp[200],data[200],change;
    //	  uint8_t i,j,t;
    //		for(t=0;t<times;t++)                 //数据清零
    //			temp[t]=0;
    //		for(t=0;t<times;t++)                 //记录数据
    //		{
    //			HAL_ADC_Start(&hadc1);	                //启动ADC单次转换
    //		  HAL_ADC_PollForConversion(&hadc1, 50);	//等待ADC转换完成
    //		  temp[t] = HAL_ADC_GetValue(&hadc1); 	  //读取ADC转换数据
    //			HAL_Delay_us(10);
    //		}
    //	  for(i=0; i<times; i++)               //数据清零
    //	  data[i] = 0;
    //
    //	  for(i=0; i<times; i++)               //复制数据
    //	  data[i] = temp[i];
    //
    //		for(i=0; i<times-1; i++)             //冒泡排序
    //		{
    //			for(j=0; j<times-i-1; j++)
    //			{
    //				if(data[j] > data[j+1])
    //				{
    //					change = data[j];
    //					data[j] = data[j+1];
    //					data[j+1] = change;
    //				}
    //			}
    //		}
    //		for(i=10; i<times-10; i++)          //求和
    //		{
    //			sum  += data[i];
    //		}
    //		Average_Adc=(float )sum/(times-20.00);
    //    return	Average_Adc;
}

/* USER CODE END 1 */
