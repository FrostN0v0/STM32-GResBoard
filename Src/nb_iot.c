#include "nb_iot.h"
#include "usart.h"
#include <string.h>

#define NB_RX_BUFFER_SIZE 256
#define SERVER_IP         "122.152.221.247" // UDP服务器IP
#define SERVER_PORT       "18888"        // UDP服务器端口

static char nb_rx_buffer[NB_RX_BUFFER_SIZE];
static int socket_id = -1;

static void NB_ClearBuffer(void)
{
    memset(nb_rx_buffer, 0, NB_RX_BUFFER_SIZE);
}

static NB_Status NB_WaitResponse(const char *expected, uint32_t timeout)
{
    uint32_t tickstart = HAL_GetTick();
    uint16_t len       = 0;

    NB_ClearBuffer();

    while ((HAL_GetTick() - tickstart) < timeout && len < NB_RX_BUFFER_SIZE - 1) {
        uint8_t ch;
        if (HAL_UART_Receive(&huart1, &ch, 1, 10) == HAL_OK) {
            nb_rx_buffer[len++] = ch;
            nb_rx_buffer[len]   = '\0';

            if (strstr(nb_rx_buffer, expected)) { return NB_OK; }
        }
    }

    return NB_TIMEOUT;
}


/**
 * @brief 发送AT命令并等待响应
 *
 * 该函数用于向NB-IoT模块发送AT命令，并等待指定的响应。
 * 如果在超时时间内收到预期的响应，则返回成功状态，否则返回超时状态。
 *
 * @param cmd 要发送的AT命令
 * @param expected 预期的响应字符串
 * @param timeout 超时时间（毫秒）
 * 
 * @return NB_Status 返回操作的状态
 * 
 */
NB_Status NB_SendAT(const char *cmd, const char *expected, uint32_t timeout)
{
    printf("%s\r\n", cmd);
    return NB_WaitResponse(expected, timeout);
}


/**
 * @brief 初始化NB-IoT模块
 *
 * 该函数用于初始化NB-IoT模块，配置其工作模式、网络注册状态等。
 * 在初始化过程中，会发送一系列AT命令以确保模块正常工作。
 *
 * @return None
 *
 */
void NB_Init(void)
{
    NB_SendAT("AT", "OK", 1000);
    HAL_Delay(500);
    NB_SendAT("AT+QREGSWT=2", "OK", 2000);    // 禁用IoT平台的注册功能
    NB_SendAT("AT+CPIN?", "READY", 2000);     // SIM卡状态
    NB_SendAT("AT+CSQ", "OK", 1000);          // 信号质量
    NB_SendAT("AT+CGATT=1", "OK", 3000);      // 附着网络
    if (NB_SendAT("AT+NSOCR=DGRAM,17,0,1", "OK", 1000) == NB_OK) { socket_id = 1; }
}

/**
 * @brief 发送UDP数据
 *
 * 该函数根据给定的数据，构造一个UDP数据包并通过NB-IoT模块发送出去。
 *
 * @param data 待发送的UDP数据，以十六进制字符串形式表示。
 * @return NB_Status 返回数据包发送的状态，表示发送成功或失败。
 * 
 */
NB_Status NB_SendUDPData(const char *data)
{
    char cmd[128];
    int len = strlen(data) / 2;
    snprintf(cmd, sizeof(cmd), "AT+NSOST=%d,\"%s\",%s,%d,%s,100", socket_id, SERVER_IP, SERVER_PORT, len, data);
    return NB_SendAT(cmd, "OK", 2000);
}

/**
 * @brief 发送电阻数据
 *
 * 该函数根据给定的电阻值和地址、通道信息，构造一个数据包并通过NB-IoT模块发送出去。
 *
 * @param Addr 设备地址，用于标识数据包的目标或源设备。
 * @param channel 通信通道，用于指定数据包传输的频段或路径。
 * @param resistance 待发送的电阻值，根据其大小有不同的编码方式。
 * 
 * @return NB_Status 返回数据包发送的状态，表示发送成功或失败。
 *
 */
NB_Status NB_SendResistanceData(uint8_t Addr, uint8_t channel, float Resistance)
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
    char data[64];
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
    sprintf(data, "%c%c%c%c%c%c%c", Lora_Data[0], Lora_Data[1], Lora_Data[2], Lora_Data[3], Lora_Data[4],
            (calCRC) & 0xFF,
            (calCRC >> 8) & 0xFF); //
    return NB_SendUDPData(data);
}
