/**
 * @file    demo_cute_drawing.c
 * @brief   可爱简笔画 - 用基本图形画出有趣的图案
 * 
 * 设计理念：
 * - 只用基本图形：圆、线、矩形
 * - 颜色鲜艳明快
 * - 画出小朋友喜欢的图案
 */

#include "../inc/KSCbasicdrawN.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

/* 颜色定义 - 鲜艳明亮 */
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
#define COLOR_SKY_BLUE    0x041F
#define COLOR_GRASS_GREEN 0x03E0

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

/* ========== 画太阳 ========== */
static void drawSun(KSC_buf* screen, int16_t x, int16_t y) {
    int i;
    
    /* 太阳光芒 */
    ksetpencolor(screen, COLOR_YELLOW, 1);
    for(i = 0; i < 12; i++) {
        float angle = i * M_PI * 2 / 12;
        int16_t x2 = x + (int16_t)(25 * cos(angle));
        int16_t y2 = y + (int16_t)(25 * sin(angle));
        SAFE_LINE(screen, x, y, x2, y2);
    }
    
    /* 太阳圆脸 */
    SAFE_FILL_CIRCLE(screen, x, y, 15);
    
    /* 笑脸 */
    ksetpencolor(screen, COLOR_BLACK, 1);
    SAFE_CIRCLE(screen, x-5, y-4, 2);
    SAFE_CIRCLE(screen, x+5, y-4, 2);
    
    /* 嘴巴 */
    for(i = -8; i <= 8; i++) {
        int16_t mx = x + i;
        int16_t my = y + 5 + (int16_t)(abs(i) / 2);
        SAFE_PIXEL(screen, mx, my);
    }
}

/* ========== 画小花 ========== */
static void drawFlower(KSC_buf* screen, int16_t x, int16_t y) {
    int i;
    
    /* 花瓣 */
    ksetpencolor(screen, COLOR_PINK, 1);
    for(i = 0; i < 6; i++) {
        float angle = i * M_PI * 2 / 6;
        int16_t px = x + (int16_t)(12 * cos(angle));
        int16_t py = y + (int16_t)(12 * sin(angle));
        SAFE_FILL_CIRCLE(screen, px, py, 6);
    }
    
    /* 花心 */
    ksetpencolor(screen, COLOR_YELLOW, 1);
    SAFE_FILL_CIRCLE(screen, x, y, 6);
    
    /* 花茎 */
    ksetpencolor(screen, COLOR_GREEN, 1);
    SAFE_LINE(screen, x, y+6, x, y+30);
    
    /* 叶子 */
    SAFE_LINE(screen, x, y+15, x-8, y+20);
    SAFE_LINE(screen, x, y+15, x+8, y+20);
}

/* ========== 画小房子 ========== */
static void drawHouse(KSC_buf* screen, int16_t x, int16_t y) {
    /* 房子身体 */
    ksetpencolor(screen, COLOR_ORANGE, 1);
    SAFE_FILL_RECT(screen, x, y, x+50, y+40);
    
    /* 屋顶 */
    ksetpencolor(screen, COLOR_RED, 1);
    SAFE_LINE(screen, x-5, y, x+25, y-20);
    SAFE_LINE(screen, x+25, y-20, x+55, y);
    SAFE_LINE(screen, x-5, y, x+55, y);
    
    /* 门 */
    ksetpencolor(screen, COLOR_BROWN, 1);
    SAFE_FILL_RECT(screen, x+20, y+20, x+30, y+40);
    
    /* 窗户 */
    ksetpencolor(screen, COLOR_CYAN, 1);
    SAFE_FILL_RECT(screen, x+35, y+15, x+45, y+25);
    ksetpencolor(screen, COLOR_BLACK, 1);
    SAFE_LINE(screen, x+40, y+15, x+40, y+25);
    SAFE_LINE(screen, x+35, y+20, x+45, y+20);
    
    /* 烟囱 */
    ksetpencolor(screen, COLOR_RED, 1);
    SAFE_FILL_RECT(screen, x+40, y-10, x+48, y);
}

/* ========== 画小鱼 ========== */
static void drawFish(KSC_buf* screen, int16_t x, int16_t y) {
    /* 鱼身 */
    ksetpencolor(screen, COLOR_ORANGE, 1);
    SAFE_FILL_CIRCLE(screen, x, y, 12);
    
    /* 眼睛 */
    ksetpencolor(screen, COLOR_WHITE, 1);
    SAFE_FILL_CIRCLE(screen, x+5, y-3, 3);
    ksetpencolor(screen, COLOR_BLACK, 1);
    SAFE_FILL_CIRCLE(screen, x+6, y-3, 1);
    
    /* 鱼鳍 */
    ksetpencolor(screen, COLOR_ORANGE, 1);
    SAFE_LINE(screen, x-8, y-2, x-15, y-5);
    SAFE_LINE(screen, x-8, y+2, x-15, y+5);
    
    /* 尾巴 */
    SAFE_LINE(screen, x-12, y, x-20, y-6);
    SAFE_LINE(screen, x-12, y, x-20, y+6);
    
    /* 鱼鳞纹 */
    ksetpencolor(screen, COLOR_YELLOW, 1);
    for(int i = -8; i <= 8; i+=4) {
        SAFE_PIXEL(screen, x+i, y);
    }
}

/* ========== 画小鸟 ========== */
static void drawBird(KSC_buf* screen, int16_t x, int16_t y) {
    /* 身体 */
    ksetpencolor(screen, COLOR_YELLOW, 1);
    SAFE_FILL_CIRCLE(screen, x, y, 10);
    
    /* 头 */
    SAFE_FILL_CIRCLE(screen, x+10, y-3, 8);
    
    /* 眼睛 */
    ksetpencolor(screen, COLOR_BLACK, 1);
    SAFE_FILL_CIRCLE(screen, x+12, y-5, 2);
    
    /* 嘴巴 */
    SAFE_LINE(screen, x+18, y-5, x+24, y-4);
    SAFE_LINE(screen, x+18, y-5, x+22, y-2);
    
    /* 翅膀 */
    ksetpencolor(screen, COLOR_ORANGE, 1);
    SAFE_LINE(screen, x-2, y, x-12, y-4);
    SAFE_LINE(screen, x-2, y, x-12, y+4);
    
    /* 尾巴 */
    SAFE_LINE(screen, x-8, y+2, x-14, y);
    SAFE_LINE(screen, x-8, y+2, x-14, y+4);
}

/* ========== 画毛毛虫 ========== */
static void drawCaterpillar(KSC_buf* screen, int16_t x, int16_t y) {
    int i;
    ksetpencolor(screen, COLOR_GREEN, 1);
    
    /* 身体圆环 */
    for(i = 0; i < 6; i++) {
        SAFE_FILL_CIRCLE(screen, x - i*10, y, 8);
    }
    
    /* 头 */
    ksetpencolor(screen, COLOR_RED, 1);
    SAFE_FILL_CIRCLE(screen, x+10, y, 8);
    
    /* 眼睛 */
    ksetpencolor(screen, COLOR_WHITE, 1);
    SAFE_FILL_CIRCLE(screen, x+13, y-3, 2);
    SAFE_FILL_CIRCLE(screen, x+7, y-3, 2);
    ksetpencolor(screen, COLOR_BLACK, 1);
    SAFE_FILL_CIRCLE(screen, x+13, y-3, 1);
    SAFE_FILL_CIRCLE(screen, x+7, y-3, 1);
    
    /* 触角 */
    SAFE_LINE(screen, x+10, y-5, x+14, y-12);
    SAFE_LINE(screen, x+10, y-5, x+6, y-12);
}

/* ========== 画小女孩 ========== */
static void drawGirl(KSC_buf* screen, int16_t x, int16_t y) {
    /* 头 */
    ksetpencolor(screen, COLOR_PINK, 1);
    SAFE_FILL_CIRCLE(screen, x, y, 12);
    
    /* 头发 */
    ksetpencolor(screen, COLOR_BROWN, 1);
    SAFE_FILL_RECT(screen, x-12, y-12, x+12, y-5);
    for(int i = -8; i <= 8; i+=4) {
        SAFE_LINE(screen, x+i, y-12, x+i+2, y-18);
    }
    
    /* 眼睛 */
    ksetpencolor(screen, COLOR_WHITE, 1);
    SAFE_FILL_CIRCLE(screen, x-5, y-3, 2);
    SAFE_FILL_CIRCLE(screen, x+5, y-3, 2);
    ksetpencolor(screen, COLOR_BLACK, 1);
    SAFE_FILL_CIRCLE(screen, x-5, y-3, 1);
    SAFE_FILL_CIRCLE(screen, x+5, y-3, 1);
    
    /* 嘴巴 */
    for(int i = -4; i <= 4; i++) {
        SAFE_PIXEL(screen, x+i, y+3);
    }
    
    /* 身体 */
    ksetpencolor(screen, COLOR_RED, 1);
    SAFE_FILL_RECT(screen, x-8, y+5, x+8, y+25);
    
    /* 裙子 */
    ksetpencolor(screen, COLOR_PINK, 1);
    SAFE_LINE(screen, x-12, y+25, x, y+35);
    SAFE_LINE(screen, x+12, y+25, x, y+35);
    SAFE_LINE(screen, x-12, y+25, x+12, y+25);
    
    /* 腿 */
    ksetpencolor(screen, COLOR_BROWN, 1);
    SAFE_LINE(screen, x-5, y+25, x-5, y+35);
    SAFE_LINE(screen, x+5, y+25, x+5, y+35);
    
    /* 手臂 */
    SAFE_LINE(screen, x-8, y+12, x-15, y+18);
    SAFE_LINE(screen, x+8, y+12, x+15, y+18);
}

/* ========== 画大树 ========== */
static void drawTree(KSC_buf* screen, int16_t x, int16_t y) {
    /* 树干 */
    ksetpencolor(screen, COLOR_BROWN, 1);
    SAFE_FILL_RECT(screen, x-8, y, x+8, y+40);
    
    /* 树冠 - 三层圆 */
    ksetpencolor(screen, COLOR_GREEN, 1);
    SAFE_FILL_CIRCLE(screen, x, y-15, 20);
    SAFE_FILL_CIRCLE(screen, x-12, y-5, 15);
    SAFE_FILL_CIRCLE(screen, x+12, y-5, 15);
    
    /* 苹果 */
    ksetpencolor(screen, COLOR_RED, 1);
    SAFE_FILL_CIRCLE(screen, x-5, y-20, 4);
    SAFE_FILL_CIRCLE(screen, x+3, y-25, 4);
    SAFE_FILL_CIRCLE(screen, x-8, y-12, 4);
    SAFE_FILL_CIRCLE(screen, x+7, y-15, 4);
}

/* ========== 画彩虹 ========== */
static void drawRainbow(KSC_buf* screen, int16_t x, int16_t y) {
    uint16_t colors[] = {COLOR_RED, COLOR_ORANGE, COLOR_YELLOW, COLOR_GREEN, COLOR_CYAN, COLOR_BLUE, COLOR_PURPLE};
    int i;
    
    for(i = 0; i < 7; i++) {
        ksetpencolor(screen, colors[i], 1);
        for(int a = 0; a < 180; a++) {
            float rad = a * M_PI / 180;
            int16_t xo = x + (int16_t)((35 - i*3) * cos(rad));
            int16_t yo = y + (int16_t)((35 - i*3) * sin(rad));
            SAFE_PIXEL(screen, xo, yo);
        }
    }
}

/* ========== 主函数 ========== */
void TFT_CuteDrawing(void) {
    KSC_buf* screen = kinitscreen(SCREEN_W, SCREEN_H, NULL, 0, 0);
    if (!screen) return;
    
    /* 天空背景 */
    ksetpencolor(screen, COLOR_SKY_BLUE, 1);
    kfillrect(screen, 0, 0, SCREEN_W-1, SCREEN_H-1);
    
    /* 草地 */
    ksetpencolor(screen, COLOR_GRASS_GREEN, 1);
    kfillrect(screen, 0, 200, SCREEN_W-1, SCREEN_H-1);
    
    /* 画各种可爱图案 */
    drawSun(screen, 200, 40);
    drawRainbow(screen, 180, 120);
    drawTree(screen, 30, 200);
    drawHouse(screen, 150, 210);
    drawFlower(screen, 80, 280);
    drawFlower(screen, 110, 285);
    drawFlower(screen, 50, 290);
    drawBird(screen, 60, 80);
    drawBird(screen, 100, 70);
    drawCaterpillar(screen, 190, 290);
    drawFish(screen, 210, 270);
    drawGirl(screen, 180, 250);
    
    /* 画云朵 */
    ksetpencolor(screen, COLOR_WHITE, 1);
    SAFE_FILL_CIRCLE(screen, 40, 50, 12);
    SAFE_FILL_CIRCLE(screen, 55, 45, 14);
    SAFE_FILL_CIRCLE(screen, 70, 50, 12);
    SAFE_FILL_CIRCLE(screen, 50, 60, 10);
    
    SAFE_FILL_CIRCLE(screen, 140, 30, 10);
    SAFE_FILL_CIRCLE(screen, 155, 25, 12);
    SAFE_FILL_CIRCLE(screen, 170, 30, 10);
    SAFE_FILL_CIRCLE(screen, 150, 38, 9);
    
    /* 画蝴蝶结装饰 */
    ksetpencolor(screen, COLOR_PINK, 1);
    SAFE_FILL_RECT(screen, 115, 265, 125, 275);
    SAFE_FILL_CIRCLE(screen, 110, 270, 5);
    SAFE_FILL_CIRCLE(screen, 130, 270, 5);
    
    /* 画标题 */
    ksetpencolor(screen, COLOR_PURPLE, 1);
    kstring(screen, "CUTE DRAWING", 75, 5);
    ksetpencolor(screen, COLOR_YELLOW, 1);
    kstring(screen, "Happy Day!", 85, 315);
    
    kfreescreen(screen);
}

int main(){
    TFT_CuteDrawing();
    return 0;
}