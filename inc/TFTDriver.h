#ifndef _DRIVER_GUI_H_
#define _DRIVER_GUI_H_

#include "KSCconfig.h"
#if __USE_ARMCC__
// 颜色宏定义
#define RED 0xF800
#define GREEN 0x07E0
#define BLUE 0x001F
#define WHITE 0xFFFF
#define BLACK 0x0000
#define YELLOW 0xFFE0

void TFT_Init(void);
void TFT_Setcanvas(uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey);
void TFT_Setcolor(uint16_t color);

#endif

#endif
