/**
 * @file    demo_3d_cube_incremental.c
 * @brief   增量绘制3D旋转立方体 - 只更新变化的线条，极致性能
 */

#include "KSCbasicdrawN.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

/* 颜色定义 - 炫酷配色 */
#define COLOR_BLACK       0x0000
#define COLOR_WHITE       0xFFFF
#define COLOR_CYAN        0x07FF
#define COLOR_MAGENTA     0xF81F
#define COLOR_YELLOW      0xFFE0
#define COLOR_RED         0xF800
#define COLOR_GREEN       0x07E0
#define COLOR_BLUE        0x001F
#define COLOR_GOLD        0xFE60
#define COLOR_ORANGE      0xFD20
#define COLOR_PURPLE      0x881F

/* 屏幕尺寸 */
#define SCREEN_W 240
#define SCREEN_H 320
#define CUBE_CX 120
#define CUBE_CY 160

/* 安全绘制宏 */
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

#define SAFE_RECT(screen, x1, y1, x2, y2) \
    do { \
        int16_t sx1 = (x1), sy1 = (y1), sx2 = (x2), sy2 = (y2); \
        if(sx1 >= 0 && sx1 < SCREEN_W && sy1 >= 0 && sy1 < SCREEN_H && \
           sx2 >= 0 && sx2 < SCREEN_W && sy2 >= 0 && sy2 < SCREEN_H) { \
            krect(screen, sx1, sy1, sx2, sy2); \
        } \
    } while(0)

/* 3D立方体顶点 (相对坐标) */
typedef struct {
    int16_t x, y, z;
} Point3D;

static Point3D cubeVertices[8] = {
    {-35, -35, -35}, { 35, -35, -35}, { 35,  35, -35}, {-35,  35, -35},  // 背面
    {-35, -35,  35}, { 35, -35,  35}, { 35,  35,  35}, {-35,  35,  35}   // 正面
};

/* 立方体边线 (12条边) */
static int cubeEdges[12][2] = {
    {0,1}, {1,2}, {2,3}, {3,0},  // 背面正方形
    {4,5}, {5,6}, {6,7}, {7,4},  // 正面正方形
    {0,4}, {1,5}, {2,6}, {3,7}   // 连接线
};

/* 立方体状态 */
typedef struct {
    float angleX;
    float angleY;
    float lastAngleX;
    float lastAngleY;
    int16_t projX[8];      /* 当前帧投影坐标 */
    int16_t projY[8];
    int16_t lastProjX[8];   /* 上一帧投影坐标 */
    int16_t lastProjY[8];
    uint8_t edgeChanged[12]; /* 每条边是否需要重绘 */
} RotatingCube;

static RotatingCube cube;

/* 3D投影函数 */
static void projectVertex(Point3D v, float rotX, float rotY, int16_t* sx, int16_t* sy) {
    float x = v.x;
    float y = v.y;
    float z = v.z;
    
    /* 绕X轴旋转 */
    float cosX = cos(rotX);
    float sinX = sin(rotX);
    float y1 = y * cosX - z * sinX;
    float z1 = y * sinX + z * cosX;
    
    /* 绕Y轴旋转 */
    float cosY = cos(rotY);
    float sinY = sin(rotY);
    float x2 = x * cosY + z1 * sinY;
    float z2 = -x * sinY + z1 * cosY;
    
    /* 透视投影 (透视因子) */
    float perspective = 180.0f / (180.0f + z2);
    *sx = CUBE_CX + (int16_t)(x2 * perspective);
    *sy = CUBE_CY + (int16_t)(y1 * perspective);
}

/* 更新立方体投影 */
static void updateCubeProjection(void) {
    /* 保存旧坐标 */
    for(int i = 0; i < 8; i++) {
        cube.lastProjX[i] = cube.projX[i];
        cube.lastProjY[i] = cube.projY[i];
    }
    cube.lastAngleX = cube.angleX;
    cube.lastAngleY = cube.angleY;
    
    /* 更新角度 */
    cube.angleX += 0.03;
    cube.angleY += 0.04;
    if(cube.angleX > M_PI * 2) cube.angleX -= M_PI * 2;
    if(cube.angleY > M_PI * 2) cube.angleY -= M_PI * 2;
    
    /* 计算新投影 */
    for(int i = 0; i < 8; i++) {
        projectVertex(cubeVertices[i], cube.angleX, cube.angleY, 
                     &cube.projX[i], &cube.projY[i]);
    }
}

/* 判断线段是否需要更新 */
static void checkEdgesChanged(void) {
    for(int i = 0; i < 12; i++) {
        int v1 = cubeEdges[i][0];
        int v2 = cubeEdges[i][1];
        
        /* 检查端点是否变化 */
        if(cube.projX[v1] != cube.lastProjX[v1] ||
           cube.projY[v1] != cube.lastProjY[v1] ||
           cube.projX[v2] != cube.lastProjX[v2] ||
           cube.projY[v2] != cube.lastProjY[v2]) {
            cube.edgeChanged[i] = 1;
        } else {
            cube.edgeChanged[i] = 0;
        }
    }
}

/* 擦除变化的旧线段 */
static void eraseOldEdges(KSC_buf* screen) {
    ksetpencolor(screen, COLOR_BLACK, 1);
    
    for(int i = 0; i < 12; i++) {
        if(cube.edgeChanged[i]) {
            int v1 = cubeEdges[i][0];
            int v2 = cubeEdges[i][1];
            SAFE_LINE(screen, cube.lastProjX[v1], cube.lastProjY[v1],
                            cube.lastProjX[v2], cube.lastProjY[v2]);
        }
    }
}

/* 绘制新线段 */
static void drawNewEdges(KSC_buf* screen, uint16_t frame) {
    for(int i = 0; i < 12; i++) {
        if(cube.edgeChanged[i]) {
            int v1 = cubeEdges[i][0];
            int v2 = cubeEdges[i][1];
            
            /* 动态变色 - 让立方体更炫酷 */
            uint16_t color;
            uint8_t hue = (frame * 2 + i * 30) % 360;
            if(hue < 60) color = COLOR_CYAN;
            else if(hue < 120) color = COLOR_MAGENTA;
            else if(hue < 180) color = COLOR_GOLD;
            else if(hue < 240) color = COLOR_ORANGE;
            else if(hue < 300) color = COLOR_RED;
            else color = COLOR_PURPLE;
            
            ksetpencolor(screen, color, 1);
            SAFE_LINE(screen, cube.projX[v1], cube.projY[v1],
                            cube.projX[v2], cube.projY[v2]);
        }
    }
}

/* 绘制顶点光晕 (可选，增加科技感) */
static void drawVertexGlow(KSC_buf* screen, uint16_t frame) {
    for(int i = 0; i < 8; i++) {
        /* 只绘制位置变化的顶点 */
        if(cube.projX[i] != cube.lastProjX[i] || 
           cube.projY[i] != cube.lastProjY[i]) {
            
            /* 擦除旧顶点光晕 */
            ksetpencolor(screen, COLOR_BLACK, 1);
            SAFE_CIRCLE(screen, cube.lastProjX[i], cube.lastProjY[i], 3);
            
            /* 绘制新顶点光晕 */
            uint16_t color = (frame % 60 < 30) ? COLOR_WHITE : COLOR_GOLD;
            ksetpencolor(screen, color, 1);
            SAFE_CIRCLE(screen, cube.projX[i], cube.projY[i], 2);
        }
    }
}

/* 绘制中心光晕 (增强3D感) */
static void drawCenterGlow(KSC_buf* screen, uint16_t frame) {
    static int16_t lastRadius = 0;
    uint8_t radius = 12 + (frame % 8);
    
    if(radius != lastRadius) {
        /* 擦除旧光晕 */
        ksetpencolor(screen, COLOR_BLACK, 1);
        SAFE_CIRCLE(screen, CUBE_CX, CUBE_CY, lastRadius);
        
        /* 绘制新光晕 */
        uint16_t color = (frame % 40 < 20) ? COLOR_CYAN : COLOR_MAGENTA;
        ksetpencolor(screen, color, 1);
        SAFE_CIRCLE(screen, CUBE_CX, CUBE_CY, radius);
        
        lastRadius = radius;
    }
}

/* 初始化立方体 */
static void initCube(void) {
    cube.angleX = 0;
    cube.angleY = 0;
    cube.lastAngleX = 0;
    cube.lastAngleY = 0;
    
    /* 初始投影 */
    for(int i = 0; i < 8; i++) {
        projectVertex(cubeVertices[i], cube.angleX, cube.angleY,
                     &cube.projX[i], &cube.projY[i]);
        cube.lastProjX[i] = cube.projX[i];
        cube.lastProjY[i] = cube.projY[i];
    }
    
    /* 初始所有边都需要绘制 */
    for(int i = 0; i < 12; i++) {
        cube.edgeChanged[i] = 1;
    }
}

/* ========== 主函数 ========== */
void TFT_3D_Cube_Demo(void) {
    KSC_buf* screen = kinitscreen(SCREEN_W, SCREEN_H, NULL, 0, 0);
    if (!screen) return;
    
    /* 初始化随机数 */
    srand(0x1234);
    
    /* 绘制静态星空背景 (只绘制一次) */
    ksetpencolor(screen, COLOR_BLACK, 1);
    kfillrect(screen, 0, 0, SCREEN_W-1, SCREEN_H-1);
    
    /* 一次性绘制所有星星 */
    for(int i = 0; i < 80; i++) {
        uint16_t x = rand() % SCREEN_W;
        uint16_t y = rand() % SCREEN_H;
        uint8_t bright = rand() % 200 + 55;
        uint16_t color = (bright >> 3) << 11 | (bright >> 3) << 6 | (bright >> 3);
        ksetpencolor(screen, color, 1);
        ksetpixel(screen, x, y);
    }
    
    /* 绘制装饰边框 */
    ksetpencolor(screen, COLOR_CYAN, 1);
    krect(screen, 2, 2, SCREEN_W-3, SCREEN_H-3);
    
    /* 绘制标题 */
    ksetpencolor(screen, COLOR_GOLD, 1);
    kstring(screen, "3D ROTATING CUBE", 55, 10);
    ksetpencolor(screen, COLOR_WHITE, 1);
    kstring(screen, "INCREMENTAL MODE", 65, 295);
    
    /* 初始化立方体 */
    initCube();
    
    uint16_t frame = 0;
    
    /* 主循环 - 只更新变化的元素 */
    while(1) {
        /* 1. 更新立方体投影 */
        updateCubeProjection();
        
        /* 2. 检查哪些边需要更新 */
        checkEdgesChanged();
        
        /* 3. 擦除变化的旧边 */
        eraseOldEdges(screen);
        
        /* 4. 绘制新边 (动态颜色) */
        drawNewEdges(screen, frame);
        
        /* 5. 更新顶点光晕 */
        drawVertexGlow(screen, frame);
        
        /* 6. 更新中心光晕 (脉动效果) */
        drawCenterGlow(screen, frame);
        
        /* 显示帧计数器 */
        if(frame % 60 == 0) {
            ksetpencolor(screen, COLOR_BLACK, 1);
            kfillrect(screen, SCREEN_W-50, 25, SCREEN_W-10, 40);
            ksetpencolor(screen, COLOR_GOLD, 1);
            char frameStr[12];
            sprintf(frameStr, "F:%d", frame);
            kstring(screen, frameStr, SCREEN_W-45, 28);
        }
        
        /* 简单延时 - 控制旋转速度 */
        {
            volatile int dly;
            for(dly = 0; dly < 2000; dly++);
        }
        
        frame++;
        
        /* 循环1000帧后重置角度 (避免浮点溢出) */
        if(frame >= 1000) {
            frame = 0;
            cube.angleX = 0;
            cube.angleY = 0;
        }
    }
    
    kfreescreen(screen);
}

int main(void){
    TFT_3D_Cube_Demo();
    return 0;
}