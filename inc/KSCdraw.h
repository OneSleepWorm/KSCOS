#ifndef _KSCbasicdraw_H_
#define _KSCbasicdraw_H_

#include "KSCconfig.h"

#if __USE_LCD__ ==1
#include "KSCdisplay.h"
#include "KSCimg.h"
#include "KSCfont.h"
#include "key.h"

typedef struct {
    BOTTON_CALLBACK bottoncallback;
    KSCCOLOR bk;
    uintxy  width;
    uintxy  height;
    uintxy  ssx;//屏幕左上角X轴位置
    uintxy  ssy;//屏幕左上角Y轴位置
    uint8_t  Mode;
    
}KSC_window;
typedef struct k_draw_device k_draw_device;
typedef void (*SCR_INIT)(void);
typedef void (*SCR_SETCANVAS)(uintxy Gx,uintxy Gy, uintxy width,uintxy height);
typedef void (*SCR_SETCOLORPIXELS)(const KSCCOLOR* color,uint16_t num);
typedef void (*SCR_WINDOW_SETCANVAS)(k_draw_device* dev,KSC_window* screen,uintxy Gx,uintxy Gy, uintxy width,uintxy height);
typedef struct k_draw_device{
    SCR_INIT init;
    SCR_SETCANVAS setcanvas;
    SCR_SETCOLORPIXELS setcolorpixels;
    SCR_WINDOW_SETCANVAS setwindows;
}k_draw_device;

typedef enum KSC_mes{
    KSC_OK = 0,
    KSC_ERR = 1,
    KSC_ERR_OUT_OF_RANGE = 2,

}KSC_mes;

// 更新kinitscreen函数声明，添加背景色参数

void kscreenmount(k_draw_device* dev);
KSC_window* kscreeninit(k_draw_device* dev,uintxy ssx,uintxy ssy,uintxy width,uintxy height,KSCCOLOR bk);
void kscreenfree(k_draw_device* dev,KSC_window* screen);

// 在指定坐标设置像素点
KSC_mes ksetpixel(k_draw_device* dev,KSC_window* screen,KSCCOLOR color,uintxy x,uintxy y);
// 填充矩形区域
KSC_mes kfull(k_draw_device* dev,KSC_window* screen,KSCCOLOR color,uintxy x1,uintxy y1,uintxy w,uintxy h);
// 绘制任意方向线段
KSC_mes kline(k_draw_device* dev,KSC_window* screen,KSCCOLOR color,uintxy x1,uintxy y1,uintxy x2,uintxy y2);
// 绘制矩形边框
KSC_mes kbox(k_draw_device* dev,KSC_window* screen,KSCCOLOR color,uintxy x1,uintxy y1,uintxy w,uintxy h);
// 填充矩形区域
KSC_mes kfillbox(k_draw_device* dev,KSC_window* screen,KSCCOLOR color,uintxy x1,uintxy y1,uintxy width,uintxy height);
// 绘制圆弧（可选方向）
KSC_mes karc(k_draw_device* dev,KSC_window* screen,KSCCOLOR color,uintxy x0,uintxy y0,uint8_t r,uint8_t Anglediraction);
// 绘制完整圆形
KSC_mes kcircle(k_draw_device* dev,KSC_window* screen,KSCCOLOR color,uintxy x0,uintxy y0,uint8_t r);
// 填充圆形区域
KSC_mes kfillcircle(k_draw_device* dev,KSC_window* screen,KSCCOLOR color,uintxy x0,uintxy y0,uint8_t r);
// 绘制圆角矩形
KSC_mes kroundrect(k_draw_device* dev,KSC_window* screen,KSCCOLOR color,uintxy x1,uintxy y1,uintxy width,uintxy height,uint8_t r);
// 绘制填充圆角矩形
KSC_mes kfillroundrect(k_draw_device* dev,KSC_window* screen,KSCCOLOR color,uintxy x1,uintxy y1,uintxy width,uintxy height,uint8_t r);
// 绘制图像
KSC_mes kdrawimage(k_draw_device* dev,KSC_window* screen,const uint16_t* img,uintxy x,uintxy y,uint8_t width,uint8_t height);
// 绘制图像（缩放）
KSC_mes kdrawimagebig(k_draw_device* dev,KSC_window* screen,const uint16_t* img,uintxy x,uintxy y,uint8_t width,uint8_t height,uint8_t scale);
// 绘制二值化图像
KSC_mes kimagebin(k_draw_device* dev,KSC_window* screen,const uint8_t* img,uintxy x,uintxy y
    ,uint8_t width,uint8_t height,KSCCOLOR colorck,KSCCOLOR colorbk);// 绘制单个字符
KSC_mes kchar(k_draw_device* dev,KSC_window* screen,char ch,uintxy x,uintxy y,KSCCOLOR colorck,KSCCOLOR colorbk);
// 绘制字符串
KSC_mes kstring(k_draw_device* dev,KSC_window* screen,const char* str,uintxy x,uintxy y,KSCCOLOR colorck,KSCCOLOR colorbk);
#if __USE_CHINESE__ >0
// 绘制中文字符串
KSC_mes kstringchinese(k_draw_device* dev,KSC_window* screen,const char* str,uintxy x,uintxy y,KSCCOLOR color1,KSCCOLOR color2);
#endif

#endif

#endif
