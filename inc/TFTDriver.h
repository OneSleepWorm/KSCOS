#ifndef _DRIVER_GUI_H_
#define _DRIVER_GUI_H_

#include "KSCconfig.h"
#if __USE_ARMCC__
// 颜色宏定义

void TFT_Init(void);
void TFT_Setcanvas(uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey);
void TFT_Setcolor(uint16_t color);
void TFT_Setcolors(const uint16_t *colorbuf, uint16_t num);
void TFT_clear(uint16_t color);
void spi_transmit(const uint8_t *data, uint16_t len);
#endif

#endif

