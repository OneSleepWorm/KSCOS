#ifndef __DRIVER_GUI_H__
#define __DRIVER_GUI_H__

#include "KSCconfig.h"
#ifdef __USE_ARMCC__
// 颜色宏定义
#define RED   0xF800
#define GREEN 0x07E0
#define BLUE  0x001F
#define WHITE 0xFFFF
#define 	BLACK	0x0000
#define 	YELLOW  0xFFE0
#define 	GRAY0   0xEF7D   	//灰色0 3165 00110 001011 00101
#define 	GRAY1   0x8410      	//灰色1      00000 000000 00000
#define 	GRAY2   0x4208      	//灰色2  1111111111011111


/*
// 系统初始化函数
void SYS_init(unsigned char PLL);

// GPIO初始化函数
void IO_init(void);



// SPI发送函数
void SPI_SendByte(unsigned char byte);

// TFT控制函数
void TFT_SEND_CMD(unsigned char o_command);
void TFT_SEND_DATA(unsigned char o_data);
void TFT_clear(void);
void TFT_full(unsigned int color);
void TFT_GPIO_init(void);
*/
// 延时函数
void delay_us(unsigned int _us_time);
void delay_ms(unsigned int _ms_time);

void TFT_Init(void);
void TFT_Setsursor(uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey);
void TFT_Setcolor(uint16_t color);

#endif

#endif
