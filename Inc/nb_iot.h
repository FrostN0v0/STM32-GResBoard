#ifndef __NB_IOT_H
#define __NB_IOT_H

#include "main.h"

// 模块返回状态
typedef enum {
    NB_OK = 0,
    NB_ERROR,
    NB_TIMEOUT
} NB_Status;

void NB_Init(void);
NB_Status NB_SendAT(const char *cmd, const char *expected, uint32_t timeout);
NB_Status NB_SendUDPData(const char *data);
NB_Status NB_SendResistanceData(uint8_t Addr, uint8_t station, uint8_t control_unit_id, float resistance);

#endif
