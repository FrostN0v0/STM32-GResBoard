#include "remote.h"
#include "tim.h"
#include "main.h"

#define CHECK_TIME_LOGIC_0() (CaptureBuf >= IR_us_LOGIC_0 - IR_DEVIATION && CaptureBuf <= IR_us_LOGIC_0 + IR_DEVIATION)
#define CHECK_TIME_LOGIC_1() (CaptureBuf >= IR_us_LOGIC_1 - IR_DEVIATION && CaptureBuf <= IR_us_LOGIC_1 + IR_DEVIATION)
#define CHECK_TIME_START()   (CaptureBuf >= IR_us_START - IR_DEVIATION && CaptureBuf <= IR_us_START + IR_DEVIATION)
#define CHECK_TIME_REPEAT_START()                                                                                      \
    (CaptureBuf >= IR_us_REPEAT_START - IR_DEVIATION && CaptureBuf <= IR_us_REPEAT_START + IR_DEVIATION)
#define CHECK_TIME_REPEAT_END()                                                                                        \
    (CaptureBuf >= IR_us_REPEAT_END - IR_DEVIATION && CaptureBuf <= IR_us_REPEAT_END + IR_DEVIATION)

uint16_t CaptureBuf   = 0;   // 存放捕获缓存
uint8_t CaptureFlag   = 1;   // 捕获标志位
uint8_t IRData[4]     = {0}; // 存放数据
uint8_t IRFlag        = 0;   // 红外接收状态
uint8_t IRReceiveFlag = 0;   // 完成接收标志

void IR_Init()
{
    __HAL_TIM_SET_CAPTUREPOLARITY(&IR_TIM, IR_TIM_CHANNE, TIM_INPUTCHANNELPOLARITY_RISING); // 改为上升沿
    HAL_TIM_IC_Start_IT(&IR_TIM, IR_TIM_CHANNE);                                            // 启动输入捕获
}
void IR_CaptureCallback()
{
    if (CaptureFlag) {
        __HAL_TIM_SET_COUNTER(&IR_TIM, 0);
        __HAL_TIM_SET_CAPTUREPOLARITY(&IR_TIM, IR_TIM_CHANNE, TIM_ICPOLARITY_FALLING); // 设置为下降沿捕获
        CaptureFlag = 0;                                                               // 状态标志位置0
    } else {
        CaptureBuf = HAL_TIM_ReadCapturedValue(&IR_TIM, IR_TIM_CHANNE);                         // 获取当前的捕获值.
        __HAL_TIM_SET_CAPTUREPOLARITY(&IR_TIM, IR_TIM_CHANNE, TIM_INPUTCHANNELPOLARITY_RISING); // 改为上升沿
        CaptureFlag = 1;                                                                        // 状态标志位置1
        if (CHECK_TIME_START())                                                                 // 接收到数据开始位
        {
            IRFlag = 1;
            for (uint8_t i = 0; i < 4; i++) {
                IRData[i] = 0; // 清空数据
            }
            return;                           // 退出，等待接收数据
        } else if (CHECK_TIME_REPEAT_START()) // 接收到循环开始位
        {
            IRFlag = 40; // 重复码标记
            return;      // 退出，等待循环第二位
        }

        if (IRFlag && IRFlag <= 32) // 接收数据
        {
            if (CHECK_TIME_LOGIC_1()) // 判断为逻辑1
            {
                IRData[(IRFlag - 1) / 8] |= (1 << (7 - (IRFlag - 1) % 8));
            } else if (!CHECK_TIME_LOGIC_0()) // 判断不是逻辑0
            {
                IRFlag = 0; // 直接退出接收
                return;
            }
            IRFlag++;
            if (IRFlag == 33) // 接收32个，接收完成
            {
                if (IR_CHECK_ADDRESS && IRData[0] != (uint8_t)~IRData[1] ||
                    IR_CHECK_COMMAND && IRData[2] != (uint8_t)~IRData[3]) // 校验
                {
                    return;
                }
                IRReceiveFlag = 1;
                return;
            }
        }
        if (IRFlag == 40) {
            if (CHECK_TIME_REPEAT_END()) {
                IRReceiveFlag = IR_CHECK_REPEAT; // 重复检查
            }
        }
    }
}
uint8_t IR_Scanf(uint32_t *data)
{
    if (IRReceiveFlag)
        IRReceiveFlag = 0;
    else
        return 0;
    *data = 0;
    for (uint8_t i = 0; i < 4; i++) { *data |= IRData[i] << 8 * (3 - i); }
    return 1;
}
uint8_t IR_value; // 键值
uint8_t keyscan(uint32_t temp)
{
    switch (temp) {
        case 0XFFA25D: {
            IR_value = 1; // 1
            break;
        }
        case 0XFF629D: {
            IR_value = 2; // 2
            break;
        }
        case 0XFFE21D: {
            IR_value = 3; // 3
            break;
        }
        case 0XFF22DD: {
            IR_value = 4; // 4
            break;
        }
        case 0XFF02FD: {
            IR_value = 5; // 5
            break;
        }
        case 0XFFC23D: {
            IR_value = 6; // 6
            break;
        }
        case 0XFFE01F: {
            IR_value = 7; // 7
            break;
        }
        case 0XFFA857: {
            IR_value = 8; // 8
            break;
        }
        case 0XFF906F: {
            IR_value = 9; // 9
            break;
        }
        case 0XFF9867: {
            IR_value = 0; // 0
            break;
        }
        case 0XFF6897: {
            IR_value = 10; // SET
            break;
        }
        case 0XFFB04F: {
            IR_value = 11; // DEL
            break;
        }
        case 0XFF38C7: {
            IR_value = 12; // OK
            break;
        }
        case 0XFF18E7: {
            IR_value = 13; // SHANGYI
            break;
        }
        case 0XFF4AB5: {
            IR_value = 14; // XIAYI
            break;
        }
        case 0XFF10EF: {
            IR_value = 15; // ZUOYI
            break;
        }
        case 0XFF5AA5: {
            IR_value = 16; // YOUYI
            break;
        }
        default:
            IR_value = 99;
            break;
    }
    return IR_value;
}
