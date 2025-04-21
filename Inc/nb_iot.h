#ifndef __NB_IOT_H
#define __NB_IOT_H

#include "main.h"

// 模块返回状态
typedef enum {
    NB_OK = 0,
    NB_ERROR,
    NB_TIMEOUT
} NB_Status;

// 函数声明
void NB_Init(void);
NB_Status NB_SendAT(const char* cmd, const char* expected, uint32_t timeout);
NB_Status NB_ConnectUDP(const char* ip, const char* port);
NB_Status NB_SendUDPData(const char* data);

#endif
