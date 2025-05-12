/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "rtc.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
#include "ist7918.h"
#include "X9318.h"
#include <math.h>
#include "remote.h"
#include "flash.h"
#include "stm32l4xx_hal.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
extern uint16_t num_i;
extern uint8_t flag_page;
extern float Resistance,old_Resistance;
extern uint16_t lora_flag;
extern uint8_t  Sleep_Time;
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
  float adc_vol;
	double RG=0;
	uint8_t Data_packet[]={0};//Lora发送pack数据包
	float scale[]={6.02,30.83,6.02,19.53,77.47};
	uint32_t temp = 0;//用于接收红外数据
  uint8_t Factor=0;
  uint16_t i=0, flag=0;// 
  uint8_t Lora_TX_BUFF[200];
//	float LCD_RG;
//	uint8_t bai,shi,ge,xiao_1,xiao_2,xiao_3;

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */
  
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART1_UART_Init();
  MX_TIM1_Init();
  MX_ADC1_Init();
  MX_RTC_Init();
  MX_TIM2_Init();
  MX_TIM6_Init();
  /* USER CODE BEGIN 2 */
	HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_1);      //PWM
	HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_1);   //启动输入捕获
  HAL_TIM_Base_Start_IT(&htim6);                //启动定时器TIM6
  LCD_init();                                   //液晶屏初始化
	X9318_Init();                                 //数字电位器初始化
  RTC_TimeShow_1();                             //rtc时间获取
  LCD_Clear() ;                                 //LCD清屏 
	num_i=0;                                       //
//	HAL_PWR_DisableWakeUpPin(PWR_WAKEUP_PIN5);//禁用�?有使用的唤醒�?:PWR_WAKEUP_PIN1 connected to PA.00
//  __HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);//清除�?有相关的唤醒标志
//  HAL_PWR_EnableWakeUpPin(PWR_WAKEUP_PIN5);//启用连接到PC5的WakeUp Pin
 
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  while (1)
  {
//    /* USER CODE END WHILE */

//    /* USER CODE BEGIN 3 */

//	if(flag_recog==1)
//		{
	if (IR_Scanf(&temp))//调试和显示
	{
//			  printf("%X\r\n", temp);//
//			  printf("%d\r\n", keyscan( temp));//得到键值
		Remote_Set( keyscan( temp));
		num_i=5;
	}
else{
				 
	Resistance_Display(Resistance,flag_page);

 if(num_i<=2)	      //     ||(flag_recog==1)//唤醒    每次唤醒 60s
	{ 
		 num_i=2;
		 HAL_Init();
		 SystemClock_Config();   //系统时钟配置
		 Low_power_Wakeup();     //唤醒配置
		 Power_1;                //放大器电源打开
		 REMOTE_1();            //红外接收头电源打开
		 Lora_Control(1);        //Lora进入数据传输模式
	}
	else if(num_i>2 &num_i<30)
	{
		Power_1;                         //放大器电源打开
		__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,100);  //设置PWM占空比，发射
		adc_vol=Get_Adc_Average(150);                     //采样--测量150次
		RG=(double)adc_vol/scale[Factor];                 //计算得到原始信号

		if(Factor==0)
			Resistance=56.638*pow(RG, -1.209);//
		else if (Factor==1)
			Resistance=22.111*pow(RG, -0.961);//
		else if (Factor==2)
			Resistance=499.13*pow(RG, -0.922);//
		else if (Factor==3)
			Resistance=748.16*pow(RG, -0.983);//
		else if (Factor==4)
			Resistance=896.73*pow(RG, -1.037);//

		if(adc_vol<300 && flag==0)		//此时信号强度在偏小
		{
		 flag=1;               		//调节标志位            
		 if(Factor>=4)							
				Factor=4;
		 else 	Factor++;
		}
	  else if(adc_vol>2800 && flag==0) //此时信号强度在偏大
	  {	
		 flag=1; 
		 if(Factor<1)							
			 Factor=0;
		 else 	Factor--;
	  }
		if( flag==1)
		{	
			switch(Factor)
			{
				case 0:
				CH1_0();
				X9318_UD_D();
				HAL_Delay(2);
				 for(i=0;i<99;i++)	
						X9318_DOWN();              // 雨刷向下
				
				X9318_UD_U();
				HAL_Delay(2);
				for(i=0;i<2;i++)	
						X9318_UP();                // 雨刷向上
				 flag=0; 
				break;
				case 1:
				 CH1_0();
				 X9318_UD_D();	
				HAL_Delay(2);	
				 for(i=0;i<10;i++)	
						X9318_DOWN();              // 雨刷向下

				 X9318_UD_U();
				HAL_Delay(2);	
				 for(i=0;i<83;i++)	
						X9318_UP();                // 雨刷向上
				flag=0; 
				break;
				case 2: 
				CH1_1(); 
				 X9318_UD_D();	
				 HAL_Delay(2);			
				 for(i=0;i<99;i++)	
						X9318_DOWN();              // 雨刷向下
				
				 X9318_UD_U();
				 HAL_Delay(2);			
				 for(i=0;i<2;i++)	
						X9318_UP();                // 雨刷向上
				flag=0; 
				break;  
				case 3:
				 CH1_1(); 
				X9318_UD_D();
				HAL_Delay(2);
				 for(i=0;i<99;i++)	
						X9318_DOWN();              // 雨刷向下
				
				X9318_UD_U(); 
				HAL_Delay(2);
				for(i=0;i<74;i++)	
						X9318_UP();                // 雨刷向上
				 flag=0; 
				 break;
				case 4:
				 CH1_1();
				 X9318_UD_D();
				 HAL_Delay(2);			
				 for(i=0;i<99;i++)
					 X9318_DOWN_1();              // 雨刷向下
				
				 X9318_UD_U();
				 HAL_Delay(2);				 
				 for(i=0;i<93;i++)	
					 X9318_UP_1();                // 雨刷向上
				 flag=0; 
				 break;
			}
		}
	}
 	else if(num_i>=30 & num_i<60 )       //30s后，
	{
		Power_0;
		Lora_Control(1);//返回数据传输模式
		if(num_i%5==0 && lora_flag==1)    //
		{
			 lora_flag=0;
			 HAL_Delay(5000);
  	   Lora_Data_UPload( Addr, channel, old_Resistance,1);//发送电阻0：发送16进制数据;1：发送文本数据
		}
	}
 else if( num_i>=60)          //
	{ 
		 Power_1;          //放大器电源打开
		 HAL_Delay(1000);
		 num_i=0;
//     flag_recog=2;  
	 	 Power_0;           //放大器电源关闭
		 REMOTE_0();        //红外遥控电源关闭
		 Lora_Control(0);   //Lora进入深度休眠模式
     Low_power_Sleep(Sleep_Time); //低功耗睡眠配置--设置休眠时间**小时
	}
}
}
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure LSE Drive Capability
  */
  HAL_PWR_EnableBkUpAccess();
  __HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)//输入捕获
{
	if (TIM2 == htim->Instance)
	{
		IR_CaptureCallback();
	}
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
