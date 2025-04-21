#include "nb_iot.h"
#include "usart.h"
#include <string.h>
#include <stdio.h>

#define NB_RX_BUFFER_SIZE 256
#define SERVER_IP   "123.235.23.38"
#define SERVER_PORT "5683" 

static char nb_rx_buffer[NB_RX_BUFFER_SIZE];



static void NB_ClearBuffer(void) {
    memset(nb_rx_buffer, 0, NB_RX_BUFFER_SIZE);
}

static NB_Status NB_WaitResponse(const char* expected, uint32_t timeout) {
    uint32_t tickstart = HAL_GetTick();
    uint16_t len = 0;

    while ((HAL_GetTick() - tickstart) < timeout) {
        uint8_t ch;
        if (HAL_UART_Receive(&huart1, &ch, 1, 10) == HAL_OK) {
            nb_rx_buffer[len++] = ch;
            nb_rx_buffer[len] = 0;
            if (strstr(nb_rx_buffer, expected)) {
                return NB_OK;
            }
        }
    }

    return NB_TIMEOUT;
}

NB_Status NB_SendAT(const char* cmd, const char* expected, uint32_t timeout) {
    printf("%s\r\n", cmd);
    return NB_WaitResponse(expected, timeout);
}

void NB_Init(void) {
    NB_SendAT("AT", "OK", 1000);
    HAL_Delay(500);
    NB_SendAT("AT+CSQ", "OK", 1000);  // 信号质量
    NB_SendAT("AT+CGATT=1", "OK", 3000);  // 附着网络


}

NB_Status NB_ConnectUDP(const char* ip, const char* port) {
    char cmd[64];
    snprintf(cmd, sizeof(cmd), "AT+NSOCR=DGRAM,17,%s,1", port);
    return NB_SendAT(cmd, "OK", 2000);
}

NB_Status NB_SendUDPData(const char* data) {
    char cmd[128];
    int len = strlen(data);
    snprintf(cmd, sizeof(cmd), "AT+NSOST=0,\"SERVER_IP\",SERVER_PORT,%d,\"%s\"", len, data);
    return NB_SendAT(cmd, "OK", 2000);
}
