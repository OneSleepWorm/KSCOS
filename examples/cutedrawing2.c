/**
 * @file    demo_desk_scene.c
 * @brief   书桌主题简笔画 - 温馨的学习工作场景
 * 
 * 设计理念：
 * - 用基本图形画出书桌上的各种物品
 * - 色彩鲜艳，细节丰富
 * - 充满生活气息
 */

#include "../inc/KSCbasicdrawN.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

/* 颜色定义 */
#define COLOR_BLACK       0x0000
#define COLOR_WHITE       0xFFFF
#define COLOR_RED         0xF800
#define COLOR_GREEN       0x07E0
#define COLOR_BLUE        0x001F
#define COLOR_YELLOW      0xFFE0
#define COLOR_CYAN        0x07FF
#define COLOR_MAGENTA     0xF81F
#define COLOR_ORANGE      0xFD20
#define COLOR_PINK        0xF81F
#define COLOR_PURPLE      0x881F
#define COLOR_BROWN       0x8200
#define COLOR_GRAY        0x7BEF
#define COLOR_LIGHT_GRAY  0xD69A
#define COLOR_DARK_GRAY   0x39E7
#define COLOR_WOOD        0xEBA0
#define COLOR_GOLD        0xFE60
#define COLOR_SILVER      0xBDD7

/* 屏幕尺寸 */
#define SCREEN_W 240
#define SCREEN_H 320

/* 安全绘制宏 */
#define SAFE_PIXEL(screen, x, y) \
    do { \
        int16_t sx = (x), sy = (y); \
        if(sx >= 0 && sx < SCREEN_W && sy >= 0 && sy < SCREEN_H) { \
            ksetpixel(screen, sx, sy); \
        } \
    } while(0)

#define SAFE_LINE(screen, x1, y1, x2, y2) \
    do { \
        int16_t sx1 = (x1), sy1 = (y1), sx2 = (x2), sy2 = (y2); \
        if(sx1 >= 0 && sx1 < SCREEN_W && sy1 >= 0 && sy1 < SCREEN_H && \
           sx2 >= 0 && sx2 < SCREEN_W && sy2 >= 0 && sy2 < SCREEN_H) { \
            kline(screen, sx1, sy1, sx2, sy2); \
        } \
    } while(0)

#define SAFE_CIRCLE(screen, x, y, r) \
    do { \
        int16_t sx = (x), sy = (y); \
        if(sx >= 0 && sx < SCREEN_W && sy >= 0 && sy < SCREEN_H) { \
            kcircle(screen, sx, sy, r); \
        } \
    } while(0)

#define SAFE_FILL_CIRCLE(screen, x, y, r) \
    do { \
        int16_t sx = (x), sy = (y); \
        if(sx >= 0 && sx < SCREEN_W && sy >= 0 && sy < SCREEN_H) { \
            kfillcircle(screen, sx, sy, r); \
        } \
    } while(0)

#define SAFE_RECT(screen, x1, y1, x2, y2) \
    do { \
        int16_t sx1 = (x1), sy1 = (y1), sx2 = (x2), sy2 = (y2); \
        if(sx1 >= 0 && sx1 < SCREEN_W && sy1 >= 0 && sy1 < SCREEN_H && \
           sx2 >= 0 && sx2 < SCREEN_W && sy2 >= 0 && sy2 < SCREEN_H) { \
            krect(screen, sx1, sy1, sx2, sy2); \
        } \
    } while(0)

#define SAFE_FILL_RECT(screen, x1, y1, x2, y2) \
    do { \
        int16_t sx1 = (x1), sy1 = (y1), sx2 = (x2), sy2 = (y2); \
        if(sx1 >= 0 && sx1 < SCREEN_W && sy1 >= 0 && sy1 < SCREEN_H && \
           sx2 >= 0 && sx2 < SCREEN_W && sy2 >= 0 && sy2 < SCREEN_H) { \
            kfillrect(screen, sx1, sy1, sx2, sy2); \
        } \
    } while(0)

/* ========== 画桌子 ========== */
static void drawDesk(KSC_buf* screen) {
    /* 桌面 - 木质 */
    ksetpencolor(screen, COLOR_WOOD, 1);
    SAFE_FILL_RECT(screen, 0, 200, SCREEN_W-1, 280);
    
    /* 桌面木纹 */
    ksetpencolor(screen, COLOR_BROWN, 1);
    for(int i = 210; i <= 270; i += 15) {
        SAFE_LINE(screen, 10, i, SCREEN_W-10, i);
    }
    
    /* 桌腿 */
    ksetpencolor(screen, COLOR_BROWN, 1);
    SAFE_FILL_RECT(screen, 20, 280, 45, 315);
    SAFE_FILL_RECT(screen, SCREEN_W-45, 280, SCREEN_W-20, 315);
    
    /* 桌边装饰 */
    ksetpencolor(screen, COLOR_GOLD, 1);
    SAFE_LINE(screen, 0, 200, SCREEN_W-1, 200);
    SAFE_LINE(screen, 0, 200, 0, 280);
    SAFE_LINE(screen, SCREEN_W-1, 200, SCREEN_W-1, 280);
}

/* ========== 画电脑 ========== */
static void drawComputer(KSC_buf* screen, int16_t x, int16_t y) {
    /* 显示器边框 */
    ksetpencolor(screen, COLOR_DARK_GRAY, 1);
    SAFE_FILL_RECT(screen, x, y, x+80, y+60);
    
    /* 屏幕 */
    ksetpencolor(screen, COLOR_CYAN, 1);
    SAFE_FILL_RECT(screen, x+3, y+3, x+77, y+57);
    
    /* 屏幕内容 - 笑脸 */
    ksetpencolor(screen, COLOR_BLACK, 1);
    SAFE_FILL_CIRCLE(screen, x+25, y+25, 3);
    SAFE_FILL_CIRCLE(screen, x+55, y+25, 3);
    
    /* 微笑 */
    for(int i = -15; i <= 15; i++) {
        int16_t mx = x+40 + i;
        int16_t my = y+40 + (int16_t)(abs(i)/3);
        SAFE_PIXEL(screen, mx, my);
    }
    
    /* 底座 */
    ksetpencolor(screen, COLOR_GRAY, 1);
    SAFE_FILL_RECT(screen, x+30, y+60, x+50, y+70);
    
    /* 电脑品牌标志 */
    ksetpencolor(screen, COLOR_WHITE, 1);
    kstring(screen, "PC", x+35, y+45);
}

/* ========== 画书本 ========== */
static void drawBook(KSC_buf* screen, int16_t x, int16_t y) {
    /* 书皮 */
    ksetpencolor(screen, COLOR_BLUE, 1);
    SAFE_FILL_RECT(screen, x, y, x+50, y+35);
    
    /* 书页 */
    ksetpencolor(screen, COLOR_WHITE, 1);
    SAFE_FILL_RECT(screen, x+2, y+2, x+48, y+33);
    
    /* 书脊 */
    ksetpencolor(screen, COLOR_YELLOW, 1);
    SAFE_LINE(screen, x+25, y, x+25, y+35);
    
    /* 书名 */
    ksetpencolor(screen, COLOR_BLACK, 1);
    kstring(screen, "BOOK", x+12, y+15);
    
    /* 另一本书叠放 */
    ksetpencolor(screen, COLOR_GREEN, 1);
    SAFE_FILL_RECT(screen, x-8, y-5, x+42, y+30);
    ksetpencolor(screen, COLOR_WHITE, 1);
    SAFE_FILL_RECT(screen, x-6, y-3, x+40, y+28);
}

/* ========== 画键盘 ========== */
static void drawKeyboard(KSC_buf* screen, int16_t x, int16_t y) {
    /* 键盘主体 */
    ksetpencolor(screen, COLOR_DARK_GRAY, 1);
    SAFE_FILL_RECT(screen, x, y, x+100, y+30);
    
    /* 按键 - 小方块阵列 */
    ksetpencolor(screen, COLOR_LIGHT_GRAY, 1);
    for(int i = 0; i < 10; i++) {
        for(int j = 0; j < 3; j++) {
            if(i == 9 && j == 2) continue;
            SAFE_FILL_RECT(screen, x+5+i*9, y+4+j*8, x+12+i*9, y+10+j*8);
        }
    }
    
    /* 空格键 */
    SAFE_FILL_RECT(screen, x+20, y+20, x+70, y+26);
    
    /* 键盘线 */
    ksetpencolor(screen, COLOR_BLACK, 1);
    SAFE_LINE(screen, x+100, y+15, x+115, y+20);
}

/* ========== 画杯子 ========== */
static void drawCup(KSC_buf* screen, int16_t x, int16_t y) {
    /* 杯身 */
    ksetpencolor(screen, COLOR_CYAN, 1);
    SAFE_FILL_RECT(screen, x, y, x+25, y+35);
    
    /* 杯口 */
    ksetpencolor(screen, COLOR_WHITE, 1);
    SAFE_LINE(screen, x-2, y, x+27, y);
    SAFE_LINE(screen, x-2, y, x, y+5);
    SAFE_LINE(screen, x+27, y, x+25, y+5);
    
    /* 把手 */
    ksetpencolor(screen, COLOR_CYAN, 1);
    SAFE_CIRCLE(screen, x+28, y+18, 8);
    ksetpencolor(screen, COLOR_BLACK, 1);
    SAFE_CIRCLE(screen, x+28, y+18, 5);
    
    /* 杯中的饮料 */
    ksetpencolor(screen, COLOR_BROWN, 1);
    SAFE_FILL_RECT(screen, x+3, y+8, x+22, y+32);
    
    /* 热气 */
    ksetpencolor(screen, COLOR_GRAY, 1);
    for(int i = 0; i < 3; i++) {
        SAFE_LINE(screen, x+12+i*3, y-2, x+10+i*4, y-8);
    }
}

/* ========== 画笔 ========== */
static void drawPen(KSC_buf* screen, int16_t x, int16_t y) {
    /* 笔杆 */
    ksetpencolor(screen, COLOR_RED, 1);
    SAFE_FILL_RECT(screen, x, y, x+30, y+5);
    
    /* 笔尖 */
    ksetpencolor(screen, COLOR_GRAY, 1);
    SAFE_FILL_RECT(screen, x+30, y+1, x+35, y+4);
    
    /* 笔帽 */
    ksetpencolor(screen, COLOR_YELLOW, 1);
    SAFE_FILL_RECT(screen, x-3, y, x, y+5);
    
    /* 笔夹 */
    ksetpencolor(screen, COLOR_SILVER, 1);
    SAFE_LINE(screen, x-2, y+1, x-2, y+8);
}

/* ========== 画剪刀 ========== */
static void drawScissors(KSC_buf* screen, int16_t x, int16_t y) {
    /* 剪刀左刃 */
    ksetpencolor(screen, COLOR_SILVER, 1);
    SAFE_LINE(screen, x, y, x-15, y+25);
    SAFE_LINE(screen, x, y, x-5, y+20);
    SAFE_LINE(screen, x-5, y+20, x-15, y+25);
    
    /* 剪刀右刃 */
    SAFE_LINE(screen, x+10, y, x+25, y+25);
    SAFE_LINE(screen, x+10, y, x+15, y+20);
    SAFE_LINE(screen, x+15, y+20, x+25, y+25);
    
    /* 剪刀手柄 */
    ksetpencolor(screen, COLOR_RED, 1);
    SAFE_FILL_CIRCLE(screen, x-10, y+30, 8);
    SAFE_FILL_CIRCLE(screen, x+20, y+30, 8);
    
    /* 中心铆钉 */
    ksetpencolor(screen, COLOR_GOLD, 1);
    SAFE_FILL_CIRCLE(screen, x+5, y+5, 3);
}

/* ========== 画揉成一团的纸 ========== */
static void drawCrumbledPaper(KSC_buf* screen, int16_t x, int16_t y) {
    /* 纸团形状 - 不规则圆 */
    ksetpencolor(screen, COLOR_WHITE, 1);
    SAFE_FILL_CIRCLE(screen, x, y, 12);
    
    /* 褶皱纹理 */
    ksetpencolor(screen, COLOR_GRAY, 1);
    SAFE_LINE(screen, x-8, y-4, x+6, y+2);
    SAFE_LINE(screen, x-3, y-7, x+4, y+5);
    SAFE_LINE(screen, x+5, y-5, x-2, y+6);
    SAFE_LINE(screen, x-5, y+3, x+7, y-2);
    
    /* 阴影 */
    ksetpencolor(screen, COLOR_DARK_GRAY, 1);
    SAFE_LINE(screen, x+8, y+6, x+12, y+8);
    SAFE_LINE(screen, x+6, y+8, x+10, y+10);
}

/* ========== 画牛奶液滴 ========== */
static void drawMilkDrops(KSC_buf* screen) {
    int i;
    int positions[][2] = {
        {180, 245}, {185, 252}, {172, 258}, {190, 265}, {165, 270},
        {195, 275}, {160, 280}, {200, 285}, {155, 290}, {205, 295}
    };
    
    for(i = 0; i < 10; i++) {
        int16_t x = positions[i][0];
        int16_t y = positions[i][1];
        
        ksetpencolor(screen, COLOR_WHITE, 1);
        SAFE_FILL_CIRCLE(screen, x, y, 4);
        
        /* 液滴高光 */
        ksetpencolor(screen, COLOR_CYAN, 1);
        SAFE_PIXEL(screen, x-1, y-1);
        
        /* 液滴拖尾 */
        ksetpencolor(screen, COLOR_WHITE, 1);
        SAFE_LINE(screen, x, y+3, x, y+6);
        
        /* 飞溅的小点 */
        SAFE_PIXEL(screen, x+3, y+2);
        SAFE_PIXEL(screen, x-2, y+4);
    }
}

/* ========== 画时钟 ========== */
static void drawClock(KSC_buf* screen, int16_t x, int16_t y) {
    /* 钟面 */
    ksetpencolor(screen, COLOR_WHITE, 1);
    SAFE_FILL_CIRCLE(screen, x, y, 18);
    ksetpencolor(screen, COLOR_BLACK, 1);
    SAFE_CIRCLE(screen, x, y, 18);
    
    /* 时刻点 */
    for(int i = 1; i <= 12; i++) {
        float angle = (i * 30 - 90) * M_PI / 180;
        int16_t mx = x + (int16_t)(14 * cos(angle));
        int16_t my = y + (int16_t)(14 * sin(angle));
        SAFE_FILL_CIRCLE(screen, mx, my, 2);
    }
    
    /* 指针 */
    ksetpencolor(screen, COLOR_RED, 1);
    SAFE_LINE(screen, x, y, x + 10, y - 8);
    ksetpencolor(screen, COLOR_BLUE, 1);
    SAFE_LINE(screen, x, y, x + 6, y + 12);
    
    /* 中心点 */
    ksetpencolor(screen, COLOR_GOLD, 1);
    SAFE_FILL_CIRCLE(screen, x, y, 3);
}

/* ========== 画相框 ========== */
static void drawPhotoFrame(KSC_buf* screen, int16_t x, int16_t y) {
    /* 相框外框 */
    ksetpencolor(screen, COLOR_BROWN, 1);
    SAFE_FILL_RECT(screen, x, y, x+40, y+35);
    ksetpencolor(screen, COLOR_GOLD, 1);
    SAFE_RECT(screen, x+2, y+2, x+38, y+33);
    
    /* 照片内容 - 简单风景 */
    ksetpencolor(screen, COLOR_CYAN, 1);
    SAFE_FILL_RECT(screen, x+5, y+5, x+35, y+18);
    ksetpencolor(screen, COLOR_GREEN, 1);
    SAFE_FILL_RECT(screen, x+5, y+18, x+35, y+30);
    
    /* 太阳 */
    ksetpencolor(screen, COLOR_YELLOW, 1);
    SAFE_FILL_CIRCLE(screen, x+30, y+8, 4);
    
    /* 相框支架 */
    ksetpencolor(screen, COLOR_BROWN, 1);
    SAFE_LINE(screen, x+15, y+35, x+10, y+42);
    SAFE_LINE(screen, x+25, y+35, x+30, y+42);
}

/* ========== 主函数 ========== */
void TFT_DeskScene(void) {
    KSC_buf* screen = kinitscreen(SCREEN_W, SCREEN_H, NULL, 0, 0);
    if (!screen) return;
    
    /* 墙面背景 - 淡黄色 */
    ksetpencolor(screen, 0xFF9C, 1);
    kfillrect(screen, 0, 0, SCREEN_W-1, SCREEN_H-1);
    
    /* 画书桌 */
    drawDesk(screen);
    
    /* 桌面上的物品 */
    drawComputer(screen, 130, 140);
    drawKeyboard(screen, 120, 175);
    drawBook(screen, 25, 205);
    drawCup(screen, 190, 210);
    drawPen(screen, 85, 265);
    drawPen(screen, 70, 270);
    drawScissors(screen, 45, 260);
    drawCrumbledPaper(screen, 160, 270);
    drawMilkDrops(screen);
    
    /* 墙上装饰 */
    drawClock(screen, 200, 50);
    drawPhotoFrame(screen, 15, 50);
    
    /* 画窗户 */
    ksetpencolor(screen, COLOR_CYAN, 1);
    SAFE_FILL_RECT(screen, 180, 20, 225, 65);
    ksetpencolor(screen, COLOR_BROWN, 1);
    SAFE_RECT(screen, 180, 20, 225, 65);
    SAFE_LINE(screen, 202, 20, 202, 65);
    SAFE_LINE(screen, 180, 42, 225, 42);
    
    /* 窗外风景 */
    ksetpencolor(screen, COLOR_GREEN, 1);
    SAFE_FILL_RECT(screen, 182, 44, 223, 63);
    ksetpencolor(screen, COLOR_YELLOW, 1);
    SAFE_FILL_CIRCLE(screen, 210, 35, 8);
    
    /* 画小地毯 */
    ksetpencolor(screen, COLOR_RED, 1);
    SAFE_FILL_RECT(screen, 50, 285, 190, 295);
    ksetpencolor(screen, COLOR_YELLOW, 1);
    for(int i = 0; i < 7; i++) {
        SAFE_LINE(screen, 55 + i*20, 286, 60 + i*20, 294);
    }
    
    /* 标题 */
    ksetpencolor(screen, COLOR_BLUE, 1);
    kstring(screen, "MY DESK", 90, 5);
    ksetpencolor(screen, COLOR_BROWN, 1);
    kstring(screen, "Study Time!", 85, 310);
    
    kfreescreen(screen);
}

int main(void) {
    TFT_DeskScene();
    return 0;
}