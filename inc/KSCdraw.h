#ifndef _KSCbasicdraw_H_
#define _KSCbasicdraw_H_

#include "KSCconfig.h"

#if __USE_LCD__ ==1
#include "KSCdisplay.h"
#include "KSCimg.h"
#include "KSCfont.h"

#if __LITTLE_END_COLOR__ == 0
#define rred 0xF100
#define bblue 0x001F
#define ggreen 0x07E0
#else
#define rred 0x00F1
#define bblue 0x1F00
#define ggreen 0xE007
//#define yyellow 0xE0F1
#define bblack 0x0000
#define wwhite 0xFFFF
#endif

//屏幕缓冲区结构体
/**
 * @brief 屏幕缓冲区结构体
 * 
 * @param fbuf 屏幕缓冲区地址
 * @param pen1 颜色笔1
 * @param pen0 颜色笔0
 * @param width 屏幕宽度
 * @param height 屏幕高度
 * @param Mode 屏幕模式
 * @param ssx 屏幕左上角X轴位置
 * @param ssy 屏幕左上角Y轴位置
 */
typedef struct {
    uint8_t* fbuf;
    uintxy  width;
    uintxy  height;
    uint8_t  Mode;
    uintxy  ssx;//屏幕左上角X轴位置
    uintxy  ssy;//屏幕左上角Y轴位置
}KSC_buf;

typedef enum KSC_mes{
    KSC_OK = 0,
    KSC_ERR = 1,
    KSC_ERR_OUT_OF_RANGE = 2,

}KSC_mes;

KSC_buf* kinitscreen(uintxy ssx,uintxy ssy,uintxy width,uintxy height);
void kfreescreen(KSC_buf* screen);
KSC_buf* kgetscreen(void);
KSC_mes ksetscreen(KSC_buf* screen);

// 在指定坐标设置像素点
KSC_mes ksetpixel(KSC_buf* screen,KSCCOLOR color,uintxy x,uintxy y);
// 填充矩形区域
KSC_mes kfull(KSC_buf* screen,KSCCOLOR color,uintxy x1,uintxy y1,uintxy w,uintxy h);
// 绘制任意方向线段
KSC_mes kline(KSC_buf* screen,KSCCOLOR color,uintxy x1,uintxy y1,uintxy x2,uintxy y2);
// 绘制矩形边框
KSC_mes kbox(KSC_buf* screen,KSCCOLOR color,uintxy x1,uintxy y1,uintxy w,uintxy h);
// 填充矩形区域
KSC_mes kfillbox(KSC_buf* screen,KSCCOLOR color,uintxy x1,uintxy y1,uintxy width,uintxy height);
// 绘制圆弧（可选方向）
KSC_mes karc(KSC_buf* screen,KSCCOLOR color,uintxy x0,uintxy y0,uint8_t r,uint8_t Anglediraction);
// 绘制完整圆形
KSC_mes kcircle(KSC_buf* screen,KSCCOLOR color,uintxy x0,uintxy y0,uint8_t r);
// 填充圆形区域
KSC_mes kfillcircle(KSC_buf* screen,KSCCOLOR color,uintxy x0,uintxy y0,uint8_t r);
// 绘制圆角矩形
KSC_mes kroundrect(KSC_buf* screen,KSCCOLOR color,uintxy x1,uintxy y1,uintxy width,uintxy height,uint8_t r);
// 绘制填充圆角矩形
KSC_mes kfillroundrect(KSC_buf* screen,KSCCOLOR color,uintxy x1,uintxy y1,uintxy width,uintxy height,uint8_t r);
// 绘制图像
KSC_mes kdrawimage(KSC_buf* screen,const uint16_t* img,uintxy x,uintxy y,uint8_t width,uint8_t height);
// 绘制图像（缩放）
KSC_mes kdrawimagebig(KSC_buf* screen,const uint16_t* img,uintxy x,uintxy y,uint8_t width,uint8_t height,uint8_t scale);
// 绘制二值化图像
KSC_mes kimagebin(KSC_buf* screen,const uint8_t* img,uintxy x,uintxy y
    ,uint8_t width,uint8_t height,KSCCOLOR colorck,KSCCOLOR colorbk);// 绘制单个字符
KSC_mes kchar(KSC_buf* screen,char ch,uintxy x,uintxy y,KSCCOLOR colorbk,KSCCOLOR colordefault);
// 绘制字符串
KSC_mes kstring(KSC_buf* screen,const char* str,uintxy x,uintxy y,KSCCOLOR colorck,KSCCOLOR colorbk);
#if __USE_CHINESE__ >0
// 绘制中文字符串
KSC_mes kstringchinese(KSC_buf* screen,const char* str,uintxy x,uintxy y,KSCCOLOR color1,KSCCOLOR color2);
#endif

#endif

#endif
