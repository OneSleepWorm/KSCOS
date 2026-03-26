///////////////////////////////////////////////////////////
// program: KSCdisplay
// anthor: 一只瞌睡虫(OneSleepWorm)
// date: 2026.01.03
///////////////////////////////////////////////////////////
//显示模块，用来管理各个不同缓冲区的显示
///////////////////////////////////////////////////////////
//我们将实现:1.与硬件屏幕对接接口;2.将缓冲区数据发送给硬件屏幕;3.支持多缓冲区写入硬件屏幕
//4.实现滚动屏幕效果
///////////////////////////////////////

#include "../inc/KSCdisplay.h"

//#define _USE_GCC_
//#define _USE_TFT_

//#define __main__
//#define K_malloc(size) malloc(size)

//清空缓冲区


#ifdef __USE_GCC__
#include <graphics.h>
#include <conio.h>
#include <stdio.h>
#define GCCx 128
#define GCCy 64

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
// static uint8_t Pix = 3;
// void setPix(uint8_t _pix){
//     Pix = _pix;
// }
// uint8_t getPix(){
//     return Pix;
// }
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
            putpixel(sCx,sCy,color);
            movecursor();
          return;
}
// void Flashbuf(KSC_buf* screen){
//     uint8_t Mx = screen ->fbx;
//     uint8_t My = screen ->fby;
//     for(uintxy y = 0;y < My;y++){
//         for(uintxy x = 0;x < Mx;x++){
//             Flashpixel(x,y,K_GetColor(screen,x,y));
//         }
//     }
// }
//事件
uint8_t EventCallback(ExMessage mes){
    if (peekmessage(&mes, EX_MOUSE|EX_KEY)) {
        switch (mes.message) {
        case WM_LBUTTONDOWN:
            printf("鼠标左键按下 at (%d, %d)\n", mes.x, mes.y);
            //closegraph();
            //return 0;  // 直接退出
            break;
            
            
        case WM_RBUTTONDOWN:
            // 处理右键按下
            break;
        }
        if(mes.message == WM_KEYDOWN){
        switch(mes.vkcode ){
            // case VK_ESCAPE:closegraph();return 0;
            // case VK_SPACE:return 2;break;
            // case VK_UP:return 3;break;
            // case VK_DOWN:return 4;break;
            // case VK_RETURN:return 5;break;
            case VK_UP:return 8;break;
            case VK_DOWN:return 2;break;
            case VK_LEFT:return 4;break;
            case VK_RIGHT:return 6;break;
            case VK_NUMPAD0: return 0x10;break;
            case VK_NUMPAD1: return 0x11;break;
            case VK_NUMPAD2: return 0x12;break;
            case VK_NUMPAD3: return 0x13;break;
            case VK_NUMPAD4: return 0x14;break;
            case VK_NUMPAD5: return 0x15;break;
            case VK_NUMPAD6: return 0x16;break;
            case VK_NUMPAD7: return 0x17;break;
            case VK_NUMPAD8: return 0x18;break;
            case VK_NUMPAD9: return 0x19;break;
        }return 1;
    }
    }return 1;//无事件
}
// void Eventdo(uint8_t mes){
//     switch(mes){
//         case 1:break;
//         case 2:break;
//         case 3:if(downdy)downdy-=7;Flashbufp(menu1,0,0,GCCx/2-1,GCCy/2-1,0,downdy);break;
//         case 4:downdy+=7;Flashbufp(menu1,0,0,GCCx/2-1,GCCy/2-1,0,downdy);break;
//         case 5:break;

//     }
// }

//限定区域刷新缓冲区
// void Flashbufp(KSC_buf* screen,uintxy sx,uintxy sy,uintxy bw,uintxy bh,uintxy bsx,uintxy bsy){
//     for(uintxy y = 0;y < bh;y++){
//         for(uintxy x = 0;x < bw;x++){
//             Flashpixel(x+sx,y+sy,K_GetColor(screen,x+bsx,y+bsy));
//         }
//     }
// }


// void Flashimage(KSC_buf* screen,uintxy sx,uintxy sy,uintxy ex,uintxy ey){
//     if(screen->ColorBit !=16) return;
//     uintxy width = ex-sx; 
//     uintxy height = ey-sy;
//     uintxy rw = screen->fbx;
//     uintxy rh = screen->fby;
//     uint8_t* img =screen->fbuf;
//     for(uintxy y=0;y<height;y++){
//         for(uintxy x=0;x<width;x++){
//             Flashpixel(sx+x,sy+y,Rgb16_24(img[(y*rw+x)*2],img[(y*rw+x)*2+1]));
//         }
//     }
// }

#endif

#ifdef __USE_ARMCC__
void screen_init(){
    TFT_Init();
}
void screen_setcolorpixel(KSCCOLOR color){
	TFT_Setcolor(color);
}

void screen_setcanvas(uintxy Gx,uintxy Gy, uintxy width,uintxy height){
	TFT_Setsursor(Gx, Gy, Gx+width-1, Gy+height-1);
}

   
#endif
