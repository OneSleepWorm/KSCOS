#ifndef __KSC_FONT_H
#define __KSC_FONT_H

/*KSC字体字模库，宽8像素，高16像素*/
#include "KSCconfig.h"

//字符集结构体
/** 
 * font_data 字符集数据指针
 * width 字符宽度
 * height 字符高度
 */
typedef const uint8_t* (*KSC_Font_GetChar)(char ch);
typedef uint8_t* (*KSC_Font_GetChinese)(uint32_t add);
typedef struct {
    KSC_Font_GetChinese Getfontfunc; // 字体数据指针
    uint8_t width;            // 字符宽度
    uint8_t height;           // 字符高度
} KSC_FontChinese;

typedef struct {
    KSC_Font_GetChar Getfontfunc; // 获取字符函数指针
    uint8_t width;            // 字符宽度
    uint8_t height;           // 字符高度
} KSC_Font1;

#if SYSTEMFONT == 7
#define _FONT_7X7_
#endif

#if SYSTEMFONT == 8
#define _FONT_8X16_
#endif

extern KSC_Font1 Systemfont0;
#ifdef __USE_CHINESE__
extern KSC_FontChinese SystemfontChinese;
#endif

#endif
