#include "nb_iot.h"
#include "usart.h"
#include <string.h>

#define NB_RX_BUFFER_SIZE 256

static char nb_rx_buffer[NB_RX_BUFFER_SIZE];
static int socket_id = 0;

static void NB_ClearBuffer(void)
{
    memset(nb_rx_buffer, 0, NB_RX_BUFFER_SIZE);
}

/*
 * @brief  等待NB-IoT模块的响应
 * @param  expected: 响应内容
 * @param  timeout: 超时时间
 * @return NB_Status: 返回操作的状态
 *
 * 该函数用于等待NB-IoT模块的响应，直到接收到预期的响应内容或超时。(暂未用到)
 */
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
    NB_SendAT("AT+QREGSWT=2", "OK", 2000); // 禁用IoT平台的注册功能
    NB_SendAT("AT+CPIN?", "READY", 2000);  // SIM卡状态
    NB_SendAT("AT+CSQ", "OK", 1000);       // 信号质量
    NB_SendAT("AT+CGATT=1", "OK", 3000);   // 附着网络
    if (NB_SendAT("AT+NSOCR=DGRAM,17,0,1", "OK", 1000) == NB_OK) { socket_id = 0; }
}

extern uint8_t ipv4_1, ipv4_2, ipv4_3, ipv4_4;
extern uint16_t port;
/**
 * @brief 发送UDP数据
 *
 * 该函数根据给定的数据，构造一个UDP数据包并通过NB-IoT模块发送出去。
 *
 * @param data 待发送的UDP数据，以十六进制字符串形式表示。
 * @return NB_Status 返回数据包发送的状态，表示发送成功或失败。
 *
 */
NB_Status NB_SendUDPData(char *data)
{
    char cmd[256];
    int len = strlen(data) / 2;
    snprintf(cmd, sizeof(cmd), "AT+NSOST=%d,%d.%d.%d.%d,%d,%d,%s,100", socket_id, ipv4_1, ipv4_2, ipv4_3, ipv4_4, port,
             len, data);
    free(data); // 释放动态分配的内存
    return NB_SendAT(cmd, "OK", 3000);
}

char *char_to_hex(char c)
{
    char *hex = (char *)malloc(3 * sizeof(char));
    sprintf(hex, "%02x", c);
    return hex;
}

char *array_to_hex(const char *array, int size)
{
    char *hex = (char *)malloc((2 * size + 1) * sizeof(char));
    hex[0]    = '\0';
    for (int i = 0; i < size; i++) {
        char *temp = char_to_hex(array[i]);
        strcat(hex, temp);
        free(temp);
    }
    return hex;
}

/**
 * @brief 发送电阻数据
 *
 * 该函数根据给定的电阻值和地址、通道信息，构造一个数据包并通过NB-IoT模块发送出去。
 *
 * @param Addr 设备地址，用于标识数据包的目标或源设备。
 * @param station 通信通道，用于指定数据包传输的频段或路径。
 * @param resistance 待发送的电阻值，根据其大小有不同的编码方式。
 *
 * @return NB_Status 返回数据包发送的状态，表示发送成功或失败。
 *
 */
NB_Status NB_SendResistanceData(uint8_t Addr, uint8_t station, uint8_t control_unit_id, float Resistance)
{
    int prec = (Resistance < 10.0f) ? 3 : (Resistance < 100.0f) ? 2 : 1;
    char NB_Data[256];
    snprintf(NB_Data, sizeof NB_Data,
             "{\"sid\":\"%03d\",\"cid\":\"%08d\",\"ts\":\"\",\"d\":[{\"tag\":\"%03d\",\"value\":%.*f}]\n}", station,
             control_unit_id, Addr, prec, Resistance);
    char *data = array_to_hex(NB_Data, strlen(NB_Data));
    HAL_Delay(500);
    return NB_SendUDPData(data);
}
