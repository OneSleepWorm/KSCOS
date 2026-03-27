#ifndef _KSCbasicdraw_H_
#define _KSCbasicdraw_H_


#include <stdint.h>
#include <stdbool.h>

//#include "Driver_GUI.h"
#include "KSCconfig.h"
#include "KSCimg.h"
#include "KSCfont.h"
#include "KSCdisplay.h"

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
    KSCCOLOR  pen1;
    KSCCOLOR pen0;
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

KSC_buf* kinitscreen(uintxy width,uintxy height,uint8_t* Buffer,uintxy ssx,uintxy ssy);
void kfreescreen(KSC_buf* screen);
KSC_buf* kgetscreen(void);
KSC_mes ksetscreen(KSC_buf* screen);

KSCCOLOR kgetpencolor(KSC_buf* screen,uint8_t pen);
KSC_mes ksetpencolor(KSC_buf* screen,KSCCOLOR color,uint8_t pen);

// 在指定坐标设置像素点
KSC_mes ksetpixel(KSC_buf* screen,uintxy x,uintxy y);
// 绘制水平线段
KSC_mes khline(KSC_buf* screen,uintxy x1,uintxy y1,uintxy x2);
// 绘制任意方向线段
KSC_mes kline(KSC_buf* screen,uintxy x1,uintxy y1,uintxy x2,uintxy y2);
// 填充矩形区域
KSC_mes kfillrect(KSC_buf* screen,uintxy x1,uintxy y1,uintxy x2,uintxy y2);
// 绘制圆弧（可选方向）
KSC_mes karc(KSC_buf* screen,uintxy x0,uintxy y0,uint8_t r,uint8_t Anglediraction);
// 绘制完整圆形
KSC_mes kcircle(KSC_buf* screen,uintxy x0,uintxy y0,uint8_t r);
// 绘制矩形边框
KSC_mes krect(KSC_buf* screen,uintxy x1,uintxy y1,uintxy w,uintxy h);
// 绘制圆角矩形
KSC_mes kroundrect(KSC_buf* screen,uintxy x1,uintxy y1,uintxy x2,uintxy y2,uint8_t r);
// 绘制填充圆角矩形
KSC_mes kfillroundrect(KSC_buf* screen,uintxy x1,uintxy y1,uintxy x2,uintxy y2,uint8_t r);
// 填充圆形区域
KSC_mes kfillcircle(KSC_buf* screen,uintxy x0,uintxy y0,uint8_t r);
// 绘制图像
KSC_mes kdrawimage(KSC_buf* screen,uintxy x,uintxy y,const uint8_t* img,uint8_t width,uint8_t height);
// 绘制图像（缩放）
KSC_mes kdrawimagebig(KSC_buf* screen,uintxy x,uintxy y,const uint8_t* img
,uint8_t width,uint8_t height,uint8_t scale);
// 绘制单个字符
KSC_mes kchar(KSC_buf* screen,char ch,uintxy x,uintxy y);
// 绘制字符串
KSC_mes kstring(KSC_buf* screen,const char* str,uintxy x,uintxy y);
#if __USE_CHINESE__ >0
// 绘制中文字符串
KSC_mes kstringchinese(KSC_buf* screen,const char* str,uintxy x,uintxy y);
#endif

#endif
