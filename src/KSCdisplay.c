///////////////////////////////////////////////////////////
// program: KSCdisplay
// anthor: 一只瞌睡虫(OneSleepWorm)
// date: 2026.01.03
///////////////////////////////////////////////////////////

#include "../inc/KSCdisplay.h"

uint32_t color16to24(uint16_t color16) {
    uint8_t r5 = (color16 >> 11) & 0x1F;
    uint8_t g6 = (color16 >> 5) & 0x3F;
    uint8_t b5 = (color16 & 0x1F);
    uint8_t r8 = (r5 << 3) | (r5 >> 2);
    uint8_t g8 = (g6 << 2) | (g6 >> 4);
    uint8_t b8 = (b5 << 3) | (b5 >> 2);
    return (b8 << 16) | (g8 << 8) | r8;
}

#if __USE_PC__
#include <graphics.h>
#include <conio.h>
#include <stdio.h>

#define SCALE 3

extern "C" {
void screen_init(void){
    initgraph(TFTx*SCALE,TFTy*SCALE);
    setlinecolor(BLACK);
    HWND hwnd = GetHWnd();
    MoveWindow(hwnd, 300, 100,TFTx*SCALE,TFTy*SCALE+10*SCALE, TRUE);
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
    }
}
void screen_setcolorpixels(const KSCCOLOR* color,uint16_t num){
            while(num--){
                KSCCOLOR ncolor = *color++;
                ncolor = (ncolor)>>8| (ncolor<<8);
                setfillcolor(color16to24(ncolor));
                solidrectangle(sCx*SCALE,sCy*SCALE,sCx*SCALE+SCALE,sCy*SCALE+SCALE);
                movecursor();
            }
          return;
}
}

#endif

#if __USE_ARMCC__
void screen_init(){
    TFT_Init();
}
void screen_setcolorpixels(const KSCCOLOR* color,uint16_t num){
    while(num--){
        KSCCOLOR ncolor = *color++;
        TFT_Setcolor(ncolor);
    }
}

void screen_setcanvas(uintxy Gx,uintxy Gy, uintxy width,uintxy height){
	TFT_Setcanvas(Gx, Gy, Gx+width-1, Gy+height-1);
}

#endif
