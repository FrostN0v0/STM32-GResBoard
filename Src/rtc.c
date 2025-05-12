/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file    rtc.c
 * @brief   This file provides code for the configuration
 *          of the RTC instances.
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
#include "rtc.h"

/* USER CODE BEGIN 0 */
#include "main.h"
#include "X9318.h"
#include "stm32l4xx_hal.h"
#include "remote.h"

/* USER CODE END 0 */

RTC_HandleTypeDef hrtc;

/* RTC init function */
void MX_RTC_Init(void) //
{

    /* USER CODE BEGIN RTC_Init 0 */

    /* USER CODE END RTC_Init 0 */

    RTC_TimeTypeDef sTime = {0};
    RTC_DateTypeDef sDate = {0};

    /* USER CODE BEGIN RTC_Init 1 */

    /* USER CODE END RTC_Init 1 */

    /** Initialize RTC Only
     */
    hrtc.Instance            = RTC;
    hrtc.Init.HourFormat     = RTC_HOURFORMAT_24; //
    hrtc.Init.AsynchPrediv   = 127;               // 异步前置器
    hrtc.Init.SynchPrediv    = 255;               // 同步前置器
    hrtc.Init.OutPut         = RTC_OUTPUT_DISABLE;
    hrtc.Init.OutPutRemap    = RTC_OUTPUT_REMAP_NONE; // 不进行输出引脚重映射
    hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
    hrtc.Init.OutPutType     = RTC_OUTPUT_TYPE_OPENDRAIN;
    if (HAL_RTC_Init(&hrtc) != HAL_OK) //
    {
        Error_Handler();
    }

    /* USER CODE BEGIN Check_RTC_BKUP */

    /* USER CODE END Check_RTC_BKUP */

    /** Initialize RTC and set the Time and Date 初始化TTC并设置日期和时间
     */
    sTime.Hours          = 0x0;
    sTime.Minutes        = 0x0;
    sTime.Seconds        = 0x0;
    sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;  // 0x00000000u
    sTime.StoreOperation = RTC_STOREOPERATION_RESET; // 0x00000000u
    if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD) != HAL_OK) { Error_Handler(); }
    sDate.WeekDay = RTC_WEEKDAY_MONDAY;
    sDate.Month   = RTC_MONTH_JANUARY;
    sDate.Date    = 0x1;
    sDate.Year    = 0x0;

    if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BCD) != HAL_OK) { Error_Handler(); }

    /** Enable the WakeUp              使能唤醒
     */
    if (HAL_RTCEx_SetWakeUpTimer_IT(&hrtc, 5, RTC_WAKEUPCLOCK_CK_SPRE_16BITS) != HAL_OK) { Error_Handler(); }
    /* USER CODE BEGIN RTC_Init 2 */

    /* USER CODE END RTC_Init 2 */
}

void HAL_RTC_MspInit(RTC_HandleTypeDef *rtcHandle) // RTC底层驱动，时钟配置
{

    RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};
    if (rtcHandle->Instance == RTC) {
        /* USER CODE BEGIN RTC_MspInit 0 */

        /* USER CODE END RTC_MspInit 0 */

        /** Initializes the peripherals clock
         */
        PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC;
        PeriphClkInit.RTCClockSelection    = RCC_RTCCLKSOURCE_LSE;
        if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK) { Error_Handler(); }

        /* RTC clock enable */
        __HAL_RCC_RTC_ENABLE();

        /* RTC interrupt Init */
        HAL_NVIC_SetPriority(RTC_WKUP_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(RTC_WKUP_IRQn);
        /* USER CODE BEGIN RTC_MspInit 1 */

        /* USER CODE END RTC_MspInit 1 */
    }
}

void HAL_RTC_MspDeInit(RTC_HandleTypeDef *rtcHandle) // RTC外设的初始化
{

    if (rtcHandle->Instance == RTC) {
        /* USER CODE BEGIN RTC_MspDeInit 0 */

        /* USER CODE END RTC_MspDeInit 0 */
        /* Peripheral clock disable */
        __HAL_RCC_RTC_DISABLE(); // 关闭RTC时钟

        /* RTC interrupt Deinit */
        HAL_NVIC_DisableIRQ(RTC_WKUP_IRQn); //
        /* USER CODE BEGIN RTC_MspDeInit 1 */

        /* USER CODE END RTC_MspDeInit 1 */
    }
}

/* USER CODE BEGIN 1 */

void RTC_TimeShow(void)
{
    RTC_DateTypeDef getSdate;
    RTC_TimeTypeDef getTime;
    /*������ʾʱ���õģ�û����ϸ�о�*/
    /* Get the RTC current Date */
    HAL_RTC_GetDate(&hrtc, &getSdate, RTC_FORMAT_BIN);
    /* Get the RTC current Time */
    HAL_RTC_GetTime(&hrtc, &getTime, RTC_FORMAT_BIN);

    /* Display time Format : Year:Month:Date:WeekDay */
    printf("%02d-%02d-%02d ", getSdate.Year, getSdate.Month, getSdate.Date);
    //	printf("\t\t");
    /* Display time Format : hh:mm:ss */
    printf("%02d:%02d:%02d", getTime.Hours, getTime.Minutes, getTime.Seconds);
    printf("\r\n");
}
RTC_HandleTypeDef RTC_Handler;
/**
  * @brief   要使�? RTC
  闹钟中断，需按照以下顺序操作�?
             1. 配置 NVIC 中的 RTC_Alarm IRQ 通道并将其使能�??
             2. 配置 RTC 以生�? RTC 闹钟（闹�? A 或闹�? B）�??
  * @param  �?
  * @retval �?
  */
void RTC_AlarmSet(void)
{
    RTC_AlarmTypeDef RTC_AlarmStructure;

    /* RTC 闹钟中断配置 */
    HAL_NVIC_SetPriority(RTC_Alarm_IRQn, 1, 2);
    /* 使能RTC闹钟中断 */
    HAL_NVIC_EnableIRQ(RTC_Alarm_IRQn);

    /* 设置闹钟时间 */
    RTC_AlarmStructure.Alarm = RTC_ALARM_A;

    RTC_AlarmStructure.AlarmTime.Hours      = 0X0;
    RTC_AlarmStructure.AlarmTime.Minutes    = 0X1;
    RTC_AlarmStructure.AlarmTime.Seconds    = 0X0;
    RTC_AlarmStructure.AlarmTime.SubSeconds = 0;
    RTC_AlarmStructure.AlarmTime.TimeFormat = RTC_HOURFORMAT12_AM;

    RTC_AlarmStructure.AlarmMask           = RTC_ALARMMASK_NONE;
    RTC_AlarmStructure.AlarmDateWeekDaySel = RTC_ALARMDATEWEEKDAYSEL_WEEKDAY;
    RTC_AlarmStructure.AlarmDateWeekDay    = RTC_WEEKDAY_THURSDAY;
    RTC_AlarmStructure.AlarmSubSecondMask  = RTC_ALARMSUBSECONDMASK_NONE;
    HAL_RTC_SetAlarm_IT(&RTC_Handler, &RTC_AlarmStructure, RTC_FORMAT_BCD);
}

/***************************************************************************
 * �?    能： 低功耗初始化                                                ***
 ****************************************************************************/
void LowPower_Config(void)
{
    __HAL_RCC_PWR_CLK_ENABLE(); // 运行电源管理时钟

    HAL_PWR_DisablePVD();

    // 禁用Flash预存取，降低性能，降低功�?
    __HAL_FLASH_PREFETCH_BUFFER_DISABLE();
    // Sleep模式下，可以Flash进入低功耗模�?
    __HAL_FLASH_SLEEP_POWERDOWN_ENABLE();

    // 关闭VREFINT	�?3uA
    // HAL_PWREx_EnableUltraLowPower();
    // 忽略VREFINT 加快启动速度
    // HAL_PWREx_EnableFastWakeUp();

    // 唤醒后�?�择哪个启动时钟�?
    __HAL_RCC_WAKEUPSTOP_CLK_CONFIG(RCC_STOP_WAKEUPCLOCK_MSI);

    // 降低核心工作电压，需要根据实际工作频率设置，建议在Cubemx内的RCC项目中配�?
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

    // 优先停用不需要的GPIO时钟
    __HAL_RCC_GPIOC_CLK_DISABLE();
    __HAL_RCC_GPIOD_CLK_DISABLE();

    //________________________ L4特用功能
    // Sleep、Stop模式下允许个总线上的外设时钟
#if defined(STM32L4xx_HAL_H)
    // 初始化先设置全部禁用
    RCC->AHB1SMENR = 0x0;
    RCC->AHB2SMENR = 0x0;
    RCC->AHB3SMENR = 0x0;

    RCC->APB1SMENR1 = 0x0; // RTC
    RCC->APB1SMENR2 = 0x0;
    RCC->APB2SMENR  = 0x0;
    //________________________ L4特用功能 Standby和Shutdown保存IO输出电平
#ifdef __BSP_LPUSART__
    __HAL_RCC_LPUART1_CLK_SLEEP_ENABLE();

    // �?要保�?
    // RX上拉高电平，否则造成灌流或�?�FE错误
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.Pin       = GPIO_PIN_3;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_PULLUP;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF8_LPUART1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    // LPUSART 唤醒只能选用 HSI �? LSE
    __HAL_RCC_WAKEUPSTOP_CLK_CONFIG(RCC_STOP_WAKEUPCLOCK_HSI);
#else
    HAL_PWREx_EnableGPIOPullDown(PWR_GPIO_A, PWR_GPIO_BIT_0 | PWR_GPIO_BIT_14 | PWR_GPIO_BIT_9 |
                                                 PWR_GPIO_BIT_10); // 保持key1和SWDCLK的电�?
    HAL_PWREx_EnableGPIOPullUp(PWR_GPIO_A, PWR_GPIO_BIT_13);       // 保持串口和SWDIO的电�?
    HAL_PWREx_EnableGPIOPullUp(PWR_GPIO_B, PWR_GPIO_BIT_1);        // 保持LED电平
    HAL_PWREx_EnablePullUpPullDownConfig();
#endif

#endif
}

/* USER CODE BEGIN 2 */
/**
 * @brief  显示时间和日�?
 */
void RTC_TimeShow_1(void)
{
    RTC_DateTypeDef getSdate;
    RTC_TimeTypeDef getTime;

    /* Get the RTC current Date */
    HAL_RTC_GetDate(&hrtc, &getSdate, RTC_FORMAT_BIN);
    /* Get the RTC current Time */
    HAL_RTC_GetTime(&hrtc, &getTime, RTC_FORMAT_BIN);

    /* Display time Format : Year:Month:Date:WeekDay */
    printf("%02d-%02d-%02d ", getSdate.Year, getSdate.Month, getSdate.Date);
    //	printf("\t\t");
    /* Display time Format : hh:mm:ss */
    printf("%02d:%02d:%02d", getTime.Hours, getTime.Minutes, getTime.Seconds);
    printf("\r\n");
}
extern uint8_t Addr;
uint8_t temp[30] = "***Enter STANDBY Mode***\r\n", i = 0;
uint8_t rtc_temp[40] = "******RTC SET OK******\r\n";
void Low_power_Sleep(uint32_t sleep_time) // 低功耗睡眠配置
{
    HAL_ADCEx_InjectedStop(&hadc1);
    HAL_ADC_Stop(&hadc1);

    HAL_ADC_DeInit(&hadc1);

    HAL_UART_DeInit(&huart1); //
    HAL_TIM_Base_DeInit(&htim1);

    SysTick->CTRL = 0x00; //
    SysTick->VAL  = 0x00; //
    __HAL_RCC_GPIOA_CLK_DISABLE();
    __HAL_RCC_GPIOB_CLK_DISABLE();
    __HAL_RCC_GPIOC_CLK_DISABLE();
    __HAL_RCC_GPIOH_CLK_DISABLE();

    GPIO_Reset_Init();
    HAL_RTCEx_DeactivateWakeUpTimer(&hrtc);
    //				HAL_UART_Transmit(&huart1,rtc_temp,sizeof(rtc_temp),2);
    //				HAL_UART_Transmit(&huart1,temp,sizeof(temp),2);
    HAL_RTCEx_SetWakeUpTimer_IT(&hrtc, sleep_time * 3600 * 3 + Addr * 60,
                                RTC_WAKEUPCLOCK_CK_SPRE_16BITS); // 3600s*xh+地址*60s，防止接收冲突
                                                                 // sleep_time*3600+Addr*60
                                                                 // 休眠时间为原来的两倍
    HAL_PWREx_EnterSTOP2Mode(PWR_STOPENTRY_WFI);                 // 进入STOP2模式
}
void Low_power_Wakeup()
{
    //  __HAL_RCC_GPIOC_CLK_ENABLE();
    //  __HAL_RCC_GPIOH_CLK_ENABLE();
    //  __HAL_RCC_GPIOB_CLK_ENABLE();
    //  __HAL_RCC_GPIOA_CLK_ENABLE();
    //	HAL_Delay��(100);

    // 	SystemClock_Config();
    uint8_t data_1[10], Channel_Data[4] = {0xC0, 0x05, 0x01};
    //	HAL_Init();
    MX_GPIO_Init();
    HAL_UART_MspInit(&huart1);
    MX_USART1_UART_Init();
    MX_TIM1_Init();
    MX_TIM2_Init();
    MX_TIM6_Init();
    MX_ADC1_Init();
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);          // PWM
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 100); // 设置PWM占空比
    HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_1);        // 启动输入捕获
    HAL_TIM_Base_Start_IT(&htim6);                     // 启动定时器TIM6
    X9318_Init();                                      // 数字电位器初始化
    LCD_init();
    //	Channel_Data[3]=(char)channel;
    //	for( uint8_t i=0;i<5;i++)
    //	{
    //	Lora_Control(2);   //进入配置模式
    //	HAL_Delay(1000);
    //	printf("%c%c%c%c",Channel_Data[0],Channel_Data[1],Channel_Data[2],Channel_Data[3]);//配置信道
    //	}
    LCD_Clear(); // 清屏
}

// uint8_t flag_recog;//唤醒标志
extern uint16_t num_i;
extern uint8_t flag_page;
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) // 外部唤醒函数
{
    if (GPIO_Pin == GPIO_PIN_5) {
        num_i = 0;
        //		 flag_recog=1;//外部唤醒标志
        flag_page = 0; // 页数标志清零
    }
}

/* USER CODE END 1 */
