#include "X9318.h"
#define CPU_FREQUENCY_MHZ 16 // STM32时钟主频
void HAL_Delay_us(uint32_t delay)
{
    int last, curr, val;
    int temp;

    while (delay != 0) {
        temp = delay > 900 ? 900 : delay;
        last = SysTick->VAL;
        curr = last - CPU_FREQUENCY_MHZ * temp;
        if (curr >= 0) {
            do {
                val = SysTick->VAL;
            } while ((val < last) && (val >= curr));
        } else {
            curr += CPU_FREQUENCY_MHZ * 1000;
            do {
                val = SysTick->VAL;
            } while ((val <= last) || (val > curr));
        }
        delay -= temp;
    }
}

void X9318_Init(void) // x9318初始化程序
{
    uint8_t i;
    X9318_CS_L();
    HAL_Delay(20);
    X9318_INC_H();     // 脉冲信号的电平拉高
    HAL_Delay_us(500); // 延时

    X9318_UD_D();            // X9318的雨刷向下
    for (i = 0; i < 99; i++) // n 先调到0
    {
        X9318_INC_H(); // 脉冲信号的电平拉高
        X9318_INC_L(); // 脉冲信号的电平拉低
    }

    X9318_UD_U();           // X9318的雨刷向上
    for (i = 0; i < 2; i++) // n 调到2
    {
        X9318_INC_H(); // 脉冲信号的电平拉高	 HAL_Delay_us(500);							    //延时
        X9318_INC_L(); // 脉冲信号的电平拉低
    }
}
void X9318_UP(void) // 雨刷向上一次
{
    //	X9318_UD_U();												//X9318的雨刷向上
    X9318_INC_H(); // 脉冲信号的电平拉高
    X9318_INC_L(); // 脉冲信号的电平拉低
}

void X9318_DOWN(void) // 雨刷向下一次
{
    //	X9318_UD_D();												//X9318的雨刷向上
    X9318_INC_H(); // 脉冲信号的电平拉高
    X9318_INC_L(); // 脉冲信号的电平拉低
}
void X9318_UP_1(void) // 雨刷向上一次
{
    //	X9318_UD_U();												//X9318的雨刷向上
    X9318_INC_H(); // 脉冲信号的电平拉高
    X9318_INC_L(); // 脉冲信号的电平拉低
}

void X9318_DOWN_1(void) // 雨刷向下一次
{
    //	X9318_UD_D();												//X9318的雨刷向上
    X9318_INC_H(); // 脉冲信号的电平拉高
    X9318_INC_L(); // 脉冲信号的电平拉低
}
uint8_t Enlarge_Control(float adc_vol)
{
    uint8_t Factor, i, flag;

    if (adc_vol < 300 & flag == 0) // 此时信号强度在偏小
    {
        flag = 1; // 标志位清除
        if (Factor >= 4)
            Factor = 4;
        else
            Factor++;
    } else if (adc_vol > 2800 & flag == 0) // 此时信号强度在偏大
    {
        flag = 1;
        if (Factor < 1)
            Factor = 0;
        else
            Factor--;
    }

    else if (flag == 1) {
        switch (Factor) //
        {
            case 0:
                CH1_0();
                X9318_UD_D();
                HAL_Delay(2);
                for (i = 0; i < 99; i++) X9318_DOWN(); // 雨刷向下

                X9318_UD_U();
                HAL_Delay(2);
                for (i = 0; i < 2; i++) X9318_UP(); // 雨刷向上
                flag = 0;
                break;
                /////////////////////////////////////////////
            case 1:
                CH1_0();
                X9318_UD_D();
                HAL_Delay(2);
                for (i = 0; i < 10; i++) X9318_DOWN(); // 雨刷向下

                X9318_UD_U();
                HAL_Delay(2);
                for (i = 0; i < 83; i++) X9318_UP(); // 雨刷向上
                flag = 0;
                break;
                /////////////////////////////////////////////
            case 2:
                CH1_1();
                X9318_UD_D();
                HAL_Delay(2);
                for (i = 0; i < 99; i++) X9318_DOWN(); // 雨刷向下
                X9318_UD_U();
                HAL_Delay(2);
                for (i = 0; i < 2; i++) X9318_UP(); // 雨刷向上
                flag = 0;
                break;
                //////////////////////////////////////////////
            case 3:
                CH1_1();
                X9318_UD_D();
                HAL_Delay(2);
                for (i = 0; i < 99; i++) X9318_DOWN(); // 雨刷向下

                X9318_UD_U();
                HAL_Delay(2);
                for (i = 0; i < 74; i++) X9318_UP(); // 雨刷向上
                flag = 0;
                break;
                //////////////////////////////////////////////
            case 4:
                CH1_1();
                X9318_UD_D();
                HAL_Delay(2);
                for (i = 0; i < 99; i++) {
                    X9318_DOWN_1(); // 雨刷向下
                }
                X9318_UD_U();
                HAL_Delay(2);
                for (i = 0; i < 93; i++) {
                    X9318_UP_1(); // 雨刷向上
                }
                flag = 0;
                break;
                /////////////////////////////////////////////
        }
    }

    return Factor;
}
