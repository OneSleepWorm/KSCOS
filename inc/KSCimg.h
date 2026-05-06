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
} KSC_Img;

typedef struct{
    char name[16];
    const uint8_t* data;
    uint16_t size;
    uint8_t type;
} Img_File;

#define KSC_IMG_ALIGN __attribute__((aligned(2)))

extern Img_File Systemimg[12];
extern const uint8_t Wechat[520]    KSC_IMG_ALIGN;
extern const uint8_t QQ[520]        KSC_IMG_ALIGN;
extern const uint8_t Setting[520]   KSC_IMG_ALIGN;
extern const uint8_t Video[520]     KSC_IMG_ALIGN;
extern const uint8_t Photo[520]     KSC_IMG_ALIGN;
extern const uint8_t Tiktok[520]    KSC_IMG_ALIGN;
extern const uint8_t Qwen[520]      KSC_IMG_ALIGN;
extern const uint8_t Note[520]      KSC_IMG_ALIGN;
extern const uint8_t Game[520]      KSC_IMG_ALIGN;
extern const uint8_t Clock[520]     KSC_IMG_ALIGN;
extern const uint8_t Book[520]      KSC_IMG_ALIGN;
extern const uint8_t Alipay[520]    KSC_IMG_ALIGN;
extern const uint8_t OneSleepWorm[40*40*2] KSC_IMG_ALIGN;

Img_File kloadimage(char* name);

#endif
