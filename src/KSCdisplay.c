///////////////////////////////////////////////////////////
// program: KSCdisplay
// anthor: 一只瞌睡虫(OneSleepWorm)
// date: 2026.01.03
///////////////////////////////////////////////////////////

#include "../inc/KSCdisplay.h"

/**
 * @brief 将16位RGB565格式颜色转换为24位RGB888格式
 * @param color16 16位RGB565格式颜色值
 * @return 24位RGB888格式颜色值
 */
uint32_t color16to24(uint16_t color16) {
    // 提取RGB565各分量
    uint8_t r5 = (color16 >> 11) & 0x1F;  // 红色分量（5位）
    uint8_t g6 = (color16 >> 5) & 0x3F;   // 绿色分量（6位）
    uint8_t b5 = color16 & 0x1F;          // 蓝色分量（5位）
    
    // 将分量扩展到8位
    uint8_t r8 = (r5 << 3) | (r5 >> 2);   // 5位扩展到8位
    uint8_t g8 = (g6 << 2) | (g6 >> 4);   // 6位扩展到8位
    uint8_t b8 = (b5 << 3) | (b5 >> 2);   // 5位扩展到8位
    
    // 组合为24位RGB888格式
    return (r8 << 16) | (g8 << 8) | b8;
}
#ifdef __USE_GCC__
#include <graphics.h>
#include <conio.h>
#include <stdio.h>

// #ifdef __MINGW32__
// extern "C" {
//     FILE* __imp___iob_func(void) {
//         static FILE iob[3];
//         iob[0] = *stdin;
//         iob[1] = *stdout;
//         iob[2] = *stderr;
//         return iob;
//     }
// }
// #endif

void screen_init(uint16_t width,uint16_t height){ 
    initgraph(width,height);
    setlinecolor(BLACK);
    HWND hwnd = GetHWnd();
    MoveWindow(hwnd, 300, 100,width,height, TRUE);
}
static uint16_t sSx,sSy,sEx,sEy,sCx,sCy;
void screen_setcanvas(uintxy Gx,uintxy Gy, uintxy width,uintxy height){

    sSx = Gx;sSy=Gy;sEx=Gx+width-1;sEy=Gy+height-1;sCx=Gx;sCy=Gy;
}
 void movecursor(){
    sCx++;
    if(sCx > sEx){
        sCx = sSx;
        sCy++;
    }//printf("cursor: %d,%d\n",sCx,sCy);
}
void screen_setcolorpixel(KSCCOLOR color){
    //printf("x,y,color:%d,%d,%d\r\n",sCx,sCy,color);
            putpixel(sCx,sCy,color16to24(color));
            movecursor();
          return;
}

#endif

#ifdef __USE_ARMCC__
void screen_init(){
    TFT_Init();
}
void screen_setcolorpixel(KSCCOLOR color){
	TFT_Setcolor(color);
}

void screen_setcanvas(uintxy Gx,uintxy Gy, uintxy width,uintxy height){
	TFT_Setcanvas(Gx, Gy, Gx+width-1, Gy+height-1);
}

   
#endif
