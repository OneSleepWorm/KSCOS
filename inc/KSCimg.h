#ifndef KSCIMG_H
#define KSCIMG_H


#include "KSCconfig.h"

/* @brief 图片属性定义KSC_Img
 * @param width:图片宽度
 * @param height:图片高度
 * @param Colorbit:图片颜色位数
 */
typedef struct  {
    uintxy width;
    uintxy height;
    uint8_t Colorbit;
}KSC_Img;

typedef struct{
    const char* name;
    const uint8_t* data;
    uint8_t width;
    uint8_t height;
}Img_File;

extern KSC_Img Systemimg0;
extern const uint8_t Wechat[512],QQ[512],Setting[512],Video[512],Photo[512]
,Tiktok[512],Qwen[512],Note[512],Game[512],Clock[512],Book[512],Alipay[512];
Img_File kloadimage(char* name);

#endif
