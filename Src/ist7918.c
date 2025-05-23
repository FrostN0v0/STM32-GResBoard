#include "ist7918.h"
#include "word.h"
#include "X9318.h"
#include "flash.h"
#include "remote.h"
#include "usart.h"
#include "stdio.h"

void LCD_init(void) // LCD初始化
{
    RES_1();
    write_com(0xb1); // ct
    write_com(0x23); // 对比度设定根据需要设定0X20

    write_com(0x30); // Bias
    write_com(0x00);

    write_com(0x31); // vck select
    write_com(0x11); // 11

    write_com(0xfc); // 0xfc 2倍压

    write_com(0x90); // duty
    write_com(0x40); // 40H=64Duty

    write_com(0x33); // Power control
    write_com(0x2d); // 2d		2f	0x2e

    write_com(0x6c); // 6c

    write_com(0xb2); // FR
    write_com(0x10); //
    write_com(0x00);

    write_com(0x74); // SET windowns AY	Com=64
    write_com(0x00);
    write_com(0x07); //

    write_com(0x75); // SET windowns AX Seg = 128
    write_com(0x00); // Start
    write_com(0x7f); // End

    write_com(0xc0); // SET AX Add
    write_com(0x00);
    write_com(0x07); // End

    write_com(0x01); // SET AY Add
    write_com(0x00); // Start 0000H
    write_com(0x7f); // End

    write_com(0x3d); // display on

    remote_read();                // 读取flash
    Addr         = remote_buf[0]; // 获取地址
    station      = remote_buf[1]; // 获取信道
    thshold      = remote_buf[2]; // 获取阈值
    Sleep_Time   = remote_buf[3]; // 获取休眠时间
    control_unit = remote_buf[4]; // 获取控制单元ID
}
void LCD_Clear() // 清屏
{
    int i, j;

    write_com(0x74); // SET windowns AY	Com=64
    write_com(0x00);
    write_com(0x07); //

    write_com(0x75); // SET windowns AX Seg = 128
    write_com(0x00); // Start
    write_com(0x7f); // End

    write_com(0xc0); // SET AX Add
    write_com(0x00);
    write_com(0x07); // End

    write_com(0x01); // SET AY Add
    write_com(0x00); // Start 0000H
    write_com(0x7f); // End

    for (i = 0; i < 8; i++) {
        for (j = 0; j < 128; j++) { write_data(0x00); }
    }
}
void LCD_EN8X16(unsigned char x, unsigned char y, uint8_t num)
{
    //	unsigned char EN8X16[32]
    int j;
    write_com(0x74);      // SET windowns AY	Com=64
    write_com(2 * x - 2); //
    write_com(2 * x - 1); //

    write_com(0x75);      // SET windowns AX Seg = 128
    write_com(8 * y);     // Start
    write_com(8 * y + 7); // End

    write_com(0xc0); // SET AX Add
    write_com(8 * y);

    write_com(0x01);      // SET AY Add
    write_com(2 * x - 2); // Start 0000H

    for (j = 0; j < 8; j++) { write_data(EN8x16[(num * 16) + j]); }
    for (j = 0; j < 8; j++) { write_data(EN8x16[(num * 16) + 8 + j]); }
}
void LCD_CH16X16(unsigned char x, unsigned char y, uint8_t num)
{
    int j;
    write_com(0x74);      // SET windowns AY	Com=64
    write_com(2 * x - 2); //
    write_com(2 * x - 1); //

    write_com(0x75);        // SET windowns AX Seg = 128
    write_com(16 * y);      // Start
    write_com(16 * y + 15); // End

    write_com(0xc0); // SET AX Add
    write_com(16 * y);

    write_com(0x01);      // SET AY Add
    write_com(2 * x - 2); // Start 0000H

    for (j = 0; j < 8; j++) { write_data(CH16x16[(num * 32) + j]); }
    for (j = 0; j < 8; j++) { write_data(CH16x16[(num * 32) + 8 + j]); }
    for (j = 0; j < 8; j++) { write_data(CH16x16[(num * 32) + 16 + j]); }
    for (j = 0; j < 8; j++) { write_data(CH16x16[(num * 32) + 24 + j]); }
}
void LCD_EN16X32(unsigned char x, unsigned char y, uint8_t num)
{
    int j;
    write_com(0x74);      // SET windowns AY	Com=64
    write_com(4 * x - 2); //
    write_com(4 * x + 1); //

    write_com(0x75);        // SET windowns AX Seg = 128
    write_com(16 * y);      // Start
    write_com(16 * y + 15); // End

    write_com(0xc0); // SET AX Add
    write_com(16 * y);

    write_com(0x01);      // SET AY Add
    write_com(4 * x - 2); // Start 0000H

    for (j = 0; j < 8; j++) { write_data(EN16x32[(num * 64) + j]); }
    for (j = 0; j < 8; j++) { write_data(EN16x32[(num * 64) + 8 + j]); }
    for (j = 0; j < 8; j++) { write_data(EN16x32[(num * 64) + 16 + j]); }
    for (j = 0; j < 8; j++) { write_data(EN16x32[(num * 64) + 24 + j]); }
    for (j = 0; j < 8; j++) { write_data(EN16x32[(num * 64) + 32 + j]); }

    for (j = 0; j < 8; j++) { write_data(EN16x32[(num * 64) + 40 + j]); }
    for (j = 0; j < 8; j++) { write_data(EN16x32[(num * 64) + 48 + j]); }
    for (j = 0; j < 8; j++) { write_data(EN16x32[(num * 64) + 56 + j]); }
}
//========================================================
void showpic(const unsigned char *k)
{
    int i, j;
    write_com(0x74); // SET windowns AY	Com=64
    write_com(0x04);
    write_com(0x05); //

    write_com(0x75); // SET windowns AX Seg = 128
    write_com(0x70); // Start
    write_com(0x77); // End

    write_com(0xc0); // SET AX Add
    write_com(0x70);

    write_com(0x01); // SET AY Add
    write_com(0x04); // Start 0000H
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 8; j++) { write_data(*k++); }
    }
}

//========================================================
void write_com(int cmd)
{
    int i;
    CS_0();
    RS_0();
    for (i = 0; i < 8; i++) {
        SCL_0();
        if ((cmd << i) & 0x80) {
            SDA_1();
        } else {
            SDA_0();
        }
        SCL_1();
    }
    RS_1();
    //			  CS_1();
}
void write_data(int dat)
{
    int i;
    CS_0();
    RS_1();

    for (i = 0; i < 8; i++) {
        SCL_0();
        if ((dat << i) & 0x80) {
            SDA_1();
        } else {
            SDA_0();
        }
        SCL_1();
    }
    CS_1();
}

uint8_t line = 1, page = 0, flag_page = 0; // 图标显示行，页面，页面标志位
uint16_t Addr = 0, thshold = 0, Sleep_Time = 0, control_unit = 0, ipv4_1 = 0, ipv4_2 = 0, ipv4_3 = 0, ipv4_4 = 0,
         flag_send = 0, station = 0, port = 0; // 地址，阈值，休眠时间, 控制单元, ip地址, 发送标志位
uint8_t addr_num, thshold_num, station_num, sleep_num, control_unit_num, ipv4_num,
    port_num; // 地址，阈值

float Resistance;

typedef struct {
    uint16_t *value; // 指向 Addr、station、thshold、… 的指针
    uint8_t *cnt;    // 指向 addr_num、station_num、… 的指针
    uint8_t maxCnt;  // 最大位数
    uint32_t maxVal; // 钳位 > 此值
} NumField;

static NumField page1Fields[] = {
    {&Addr, &addr_num, 2, 99},
    {&station, &station_num, 3, 999},
    {&thshold, &thshold_num, 2, 99},
    {&Sleep_Time, &sleep_num, 2, 24},
};
static NumField page2Fields[] = {
    {&control_unit, &control_unit_num, 2, 99},
    {&ipv4_1, &ipv4_num, 3, 255},
    {&ipv4_2, &ipv4_num, 3, 255},
    {&ipv4_3, &ipv4_num, 3, 255},
    {&ipv4_4, &ipv4_num, 3, 255},
    {&port, &port_num, 5, 65535},
};

static void handle_numeric_input(NumField *f, uint8_t key)
{
    if (key <= 9) {
        (*f->cnt)++;
        if (*f->cnt > f->maxCnt) *f->cnt = f->maxCnt;
        *f->value = (*f->cnt == 1) ? key : (*f->value * 10 + key);
        if (*f->value > f->maxVal) {
            *f->value = f->maxVal;
            *f->cnt   = f->maxCnt;
        }
    } else if (key == 11 && *f->cnt > 0) { // DEL
        (*f->cnt)--;
        *f->value /= 10;
    }
}
void Remote_Set(uint8_t temp)
{
    LCD_Clear(); // 清屏
    uint64_t data_1[10];
    uint8_t data_length = 0;
    switch (temp) {
        case 10: // 按下设置键，进入设置页面
        {
            if (page < 2) {
                page++;
                flag_page = page;
            } else {
                page      = 1;
                flag_page = 1;
            }
            line = 1;
            LCD_Clear();
            break;
        }
        case 12: // 按下确定键，返回首页
        {
            if (page != 0 && flag_page != 0) {
                page      = 0;
                flag_page = 0;
                LCD_Clear();
                data_1[data_length++] = Addr;         // 地址
                data_1[data_length++] = station;      // 站点
                data_1[data_length++] = thshold;      // 电阻阈值
                data_1[data_length++] = Sleep_Time;   // 休眠时间
                data_1[data_length++] = control_unit; // 控制单元ID
                data_1[data_length++] = ipv4_1;       // ip地址
                data_1[data_length++] = ipv4_2;       // ip地址
                data_1[data_length++] = ipv4_3;       // ip地址
                data_1[data_length++] = ipv4_4;       // ip地址
                data_1[data_length++] = port;         // 端口号
                Flash_Erase();                        // 擦除+解锁
                remote_data(data_1, data_length);     // 写入数据
            }
            break;
        }
        case 13: // 向上移动
        {
            if (line >= 2 && (page == 1 || page == 2)) line--;
            break;
        }
        case 14: // 向下移动
        {
            if (page == 1 && line <= 4) {
                line++;
            } else if (page == 2 && line <= 6) {
                line++;
            }
            break;
        }
    }
    if (page == 1 && line >= 1 && line <= 4) {
        handle_numeric_input(&page1Fields[line - 1], temp);
    } else if (page == 2 && line >= 1 && line <= 6) {
        handle_numeric_input(&page2Fields[line - 1], temp);
    }
}
float LCD_RG, old_Resistance, Renew;
uint8_t bai, shi, ge, xiao_1, xiao_2, xiao_3;
void Resistance_Display(float Resistance, uint8_t page) // 电阻显示
{
    if (page == 1) {
        LCD_CH16X16(line, 0, 10); // 指示标志

        LCD_CH16X16(1, 1, 11);        // 通
        LCD_CH16X16(1, 2, 12);        // 讯
        LCD_CH16X16(1, 3, 2);         // 地
        LCD_CH16X16(1, 4, 3);         // 址
        LCD_EN8X16(1, 10, 11);        //:
        LCD_EN8X16(1, 12, Addr / 10); //
        LCD_EN8X16(1, 13, Addr % 10); //

        LCD_CH16X16(2, 1, 11); // 通
        LCD_CH16X16(2, 2, 12); // 讯
        LCD_CH16X16(2, 3, 4);  // 站
        LCD_CH16X16(2, 4, 5);  // 点
        LCD_EN8X16(2, 10, 11);
        LCD_EN8X16(2, 12, station / 100);
        LCD_EN8X16(2, 13, (station % 100) / 10);
        LCD_EN8X16(2, 14, station % 10);

        LCD_CH16X16(3, 1, 0);  // 电
        LCD_CH16X16(3, 2, 1);  // 阻
        LCD_CH16X16(3, 3, 6);  // 阈
        LCD_CH16X16(3, 4, 7);  // 值
        LCD_EN8X16(3, 10, 11); //:
        LCD_EN8X16(3, 12, thshold / 10);
        LCD_EN8X16(3, 13, thshold % 10);

        LCD_CH16X16(4, 1, 15); // 休
        LCD_CH16X16(4, 2, 16); // 眠
        LCD_CH16X16(4, 3, 17); // 时
        LCD_CH16X16(4, 4, 18); // 间
        LCD_EN8X16(4, 10, 11); //:
        LCD_EN8X16(4, 12, Sleep_Time / 10);
        LCD_EN8X16(4, 13, Sleep_Time % 10);
        LCD_EN8X16(4, 15, 17);
    } else if (page == 2) {
        if (line >= 2 && line <= 5) {
            LCD_CH16X16(3, 0, 10);
            LCD_EN8X16(2, line * 4 - 5, 23); // 指示标志
        } else if (line > 5) {
            LCD_CH16X16(4, 0, 10);
        } else {
            LCD_CH16X16(line, 0, 10); // 指示标志
        }

        LCD_CH16X16(1, 1, 11); // 通
        LCD_CH16X16(1, 2, 12); // 讯
        LCD_CH16X16(1, 3, 19); // 主
        LCD_CH16X16(1, 4, 20); // 控
        LCD_EN8X16(1, 10, 11); //:
        LCD_EN8X16(1, 12, control_unit / 10);
        LCD_EN8X16(1, 13, control_unit % 10);

        LCD_EN8X16(2, 1, 18);
        LCD_EN8X16(2, 2, 19); // IP

        LCD_EN8X16(3, 1, ipv4_1 / 100); // 地址
        LCD_EN8X16(3, 2, (ipv4_1 % 100) / 10);
        LCD_EN8X16(3, 3, ipv4_1 % 10);
        LCD_EN8X16(3, 4, 10); // .
        LCD_EN8X16(3, 5, ipv4_2 / 100);
        LCD_EN8X16(3, 6, (ipv4_2 % 100) / 10);
        LCD_EN8X16(3, 7, ipv4_2 % 10);
        LCD_EN8X16(3, 8, 10); // .
        LCD_EN8X16(3, 9, ipv4_3 / 100);
        LCD_EN8X16(3, 10, (ipv4_3 % 100) / 10);
        LCD_EN8X16(3, 11, ipv4_3 % 10);
        LCD_EN8X16(3, 12, 10); // .
        LCD_EN8X16(3, 13, ipv4_4 / 100);
        LCD_EN8X16(3, 14, (ipv4_4 % 100) / 10);
        LCD_EN8X16(3, 15, ipv4_4 % 10);

        LCD_EN8X16(4, 1, 19);
        LCD_EN8X16(4, 2, 20);
        LCD_EN8X16(4, 3, 21);
        LCD_EN8X16(4, 4, 22);            // 端口号
        LCD_EN8X16(4, 5, 11);            //:
        LCD_EN8X16(4, 10, port / 10000); // 地址
        LCD_EN8X16(4, 11, (port % 10000) / 1000);
        LCD_EN8X16(4, 12, (port % 1000) / 100);
        LCD_EN8X16(4, 13, (port % 100) / 10);
        LCD_EN8X16(4, 14, port % 10);

    } else if (page == 0) {
        remote_read();                // 读取flash
        Addr         = remote_buf[0]; // 获取地址
        station      = remote_buf[1]; // 获取信道
        thshold      = remote_buf[2]; // 获取阈值
        Sleep_Time   = remote_buf[3]; // 获取休眠时间
        control_unit = remote_buf[4]; // 获取控制单元ID
        ipv4_1       = remote_buf[5];
        ipv4_2       = remote_buf[6];
        ipv4_3       = remote_buf[7];
        ipv4_4       = remote_buf[8];
        port         = remote_buf[9];
        if (Addr > 255 && station > 84 && thshold > 99 && Sleep_Time > 24 && control_unit > 99) {
            uint64_t data_2b_saved[10];
            data_2b_saved[0] = Addr = 1;
            data_2b_saved[1] = station = 23;
            data_2b_saved[2] = thshold = 10;
            data_2b_saved[3] = Sleep_Time = 1;
            data_2b_saved[4] = control_unit = 1;
            remote_data(data_2b_saved, 5); // FLASH中写入数据
        }
        LCD_CH16X16(4, 0, 2); // 地
        LCD_CH16X16(4, 1, 3); // 址
        LCD_EN8X16(4, 4, 11); //:

        LCD_EN8X16(4, 5, Addr / 10); // 1
        LCD_EN8X16(4, 6, Addr % 10); // 2

        LCD_CH16X16(4, 4, 4);                    // 信
        LCD_CH16X16(4, 5, 5);                    // 道
        LCD_EN8X16(4, 12, 11);                   //:
        LCD_EN8X16(4, 13, station / 100);        //
        LCD_EN8X16(4, 14, (station % 100) / 10); //
        LCD_EN8X16(4, 15, station % 10);         //

        LCD_CH16X16(1, 0, 0);  // 电
        LCD_CH16X16(1, 1, 1);  // 阻
        LCD_CH16X16(1, 6, 13); // 电池0
        LCD_CH16X16(1, 7, 14); // 电池1
        /**************数据防抖处理********************/
        Renew = (float)Resistance * 100 / old_Resistance;
        if ((Renew > 102) || (Renew < 98)) {
            old_Resistance = Resistance;
        } else if (old_Resistance < 10) {
            LCD_RG = old_Resistance * 1000;
            ge     = (uint16_t)LCD_RG / 1000;
            xiao_1 = (uint16_t)LCD_RG % 1000 / 100;
            xiao_2 = (uint16_t)LCD_RG % 100 / 10;
            xiao_3 = (uint16_t)LCD_RG % 10;

            LCD_EN16X32(1, 1, ge);     // 个位
            LCD_EN16X32(1, 2, 10);     // 小数点
            LCD_EN16X32(1, 3, xiao_1); // 0.1
            LCD_EN16X32(1, 4, xiao_2); // 0.01
            LCD_EN16X32(1, 5, xiao_3); // 0.001
            LCD_EN16X32(1, 6, 11);     // Ω
        } else if (old_Resistance >= 10 & old_Resistance < 100) {
            LCD_RG = old_Resistance * 100;
            shi    = (uint16_t)LCD_RG / 1000;
            ge     = (uint16_t)LCD_RG % 1000 / 100;
            xiao_1 = (uint16_t)LCD_RG % 100 / 10;
            xiao_2 = (uint16_t)LCD_RG % 10;

            LCD_EN16X32(1, 1, shi);    // shi
            LCD_EN16X32(1, 2, ge);     // ge
            LCD_EN16X32(1, 3, 10);     //.
            LCD_EN16X32(1, 4, xiao_1); // 0.1
            LCD_EN16X32(1, 5, xiao_2); // 0.01
            LCD_EN16X32(1, 6, 11);     // Ω
        }

        else if (old_Resistance >= 100) {
            LCD_EN16X32(1, 1, 15); //-
            LCD_EN16X32(1, 2, 14); //
            LCD_EN16X32(1, 3, 12); // O
            LCD_EN16X32(1, 4, 13); // L
            LCD_EN16X32(1, 5, 14); //
            LCD_EN16X32(1, 6, 15); //-
        }
        if (thshold > old_Resistance)
            Alarm_0(); // 不报警
        else
            Alarm_1(); // 阈值报警
    }
}

void LCD_Display(float RG)
{
    uint16_t LCD_RG;
    uint8_t bai, shi, ge, xiao_1, xiao_2, xiao_3;

    if (RG >= 100) {
        LCD_RG = RG * 100;
        bai    = (uint16_t)LCD_RG / 10000;
        shi    = (uint16_t)LCD_RG % 10000 / 1000;
        ge     = (uint16_t)LCD_RG % 1000 / 100;
        xiao_1 = (uint16_t)LCD_RG % 100 / 10;
        xiao_2 = (uint16_t)LCD_RG % 10;

        LCD_EN16X32(1, 1, bai);
        LCD_EN16X32(1, 2, shi);
        LCD_EN16X32(1, 3, ge);
        LCD_EN16X32(1, 4, 10);
        LCD_EN16X32(1, 5, xiao_1);
        LCD_EN16X32(1, 6, xiao_2);
    } else if (RG >= 10 & RG < 100) {
        LCD_RG = RG * 100;
        bai    = (uint16_t)LCD_RG / 10000;
        shi    = (uint16_t)LCD_RG % 10000 / 1000;
        ge     = (uint16_t)LCD_RG % 1000 / 100;
        xiao_1 = (uint16_t)LCD_RG % 100 / 10;
        xiao_2 = (uint16_t)LCD_RG % 10;

        LCD_EN16X32(1, 1, bai);
        LCD_EN16X32(1, 2, shi);
        LCD_EN16X32(1, 3, ge);
        LCD_EN16X32(1, 4, 10);
        LCD_EN16X32(1, 5, xiao_1);
        LCD_EN16X32(1, 6, xiao_2);
    } else if (RG < 10) {
        LCD_RG = RG * 100;
        bai    = (uint16_t)LCD_RG / 10000;
        shi    = (uint16_t)LCD_RG % 10000 / 1000;
        ge     = (uint16_t)LCD_RG % 1000 / 100;
        xiao_1 = (uint16_t)LCD_RG % 100 / 10;
        xiao_2 = (uint16_t)LCD_RG % 10;

        LCD_EN16X32(1, 1, bai);
        LCD_EN16X32(1, 2, shi);
        LCD_EN16X32(1, 3, ge);
        LCD_EN16X32(1, 4, 10);
        LCD_EN16X32(1, 5, xiao_1);
        LCD_EN16X32(1, 6, xiao_2);
    }
}