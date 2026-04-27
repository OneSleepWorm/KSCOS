////////////////////////////////////////////////////////////////////////
/* @file KSCbasicdraw.c
 * @brief 基本绘图函数
 * @author OneSleepWorm(一只瞌睡虫)
 * @date 2026/1/31
 * @version 1.0.1
 */
/////////////////////////////////////////////////////////////////


#include "../inc/KSCdraw.h"
#include <string.h>

#if __USE_LCD__ ==1

void* r_malloc(size_t size){
    return malloc(size);
}
void r_free(void* ptr){
    free(ptr);
}
void* k_malloc(size_t size){
    return malloc(size);
}
void k_free(void* ptr){
    free(ptr);
}
#if __USE_PC__
// #include <graphics.h>
// #include <conio.h>
// #include <stdio.h>

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

// void screen_init(uint16_t width,uint16_t height){ 
//     initgraph(width,height);
//     setlinecolor(BLACK);
//     HWND hwnd = GetHWnd();
//     MoveWindow(hwnd, 300, 100,width,height, TRUE);
// }
// static uint16_t sSx,sSy,sEx,sEy,sCx,sCy;
// void screen_setcanvas(uintxy Gx,uintxy Gy, uintxy width,uintxy height){

//     sSx = Gx;sSy=Gy;sEx=Gx+width-1;sEy=Gy+height-1;sCx=Gx;sCy=Gy;
// }
//  void movecursor(){
//     sCx++;
//     if(sCx > sEx){
//         sCx = sSx;
//         sCy++;
//     }//printf("cursor: %d,%d\n",sCx,sCy);
// }
// void screen_setcolorpixel(KSCCOLOR color){
//     //printf("x,y,color:%d,%d,%d\r\n",sCx,sCy,color);
//             putpixel(sCx,sCy,color16to24(color));
//             movecursor();
//           return;
// }

#endif

#if __USE_ARMCC__
void screen_init(){
    TFT_Init();
}
void screen_setcolorpixels(uint16_t* colorbuf, uint16_t num){
	TFT_Setcolors(*colorbuf);
}

void screen_setcanvas(uintxy Gx,uintxy Gy, uintxy width,uintxy height){
	TFT_Setcanvas(Gx, Gy, Gx+width-1, Gy+height-1);
}
#endif
#if __USE_ESP32__
void screen_init(){
    TFT_Init();
}
void screen_setcolorpixels(uint16_t* colorbuf, uint16_t num){
	TFT_Setcolors(colorbuf,num);
}
void screen_setcanvas(uintxy Gx,uintxy Gy, uintxy width,uintxy height){
	TFT_Setcanvas(Gx, Gy, Gx+width-1, Gy+height-1);
}
#endif

void memset_16(void* buf, size_t size_bytes, uint16_t pattern) {
    if (size_bytes < sizeof(uint16_t)) return;
    uint16_t* p = (uint16_t*)buf;
    size_t len = size_bytes / sizeof(uint16_t);
    p[0] = pattern;
    size_t i;
    for (i = 1; i < len; i <<= 1) {
        size_t copy_size = (i < len - i) ? i : len - i;
        memcpy(p + i, p, copy_size * sizeof(uint16_t));
    }
}

/* 公共函数：保留原接口，内部进行指针和位宽优化 */
void imgchange(const uint8_t* imgbuf,
               uint16_t* imgbuf2,
               const uint16_t length,//bit位长度
               uint16_t bit,
               const uint16_t* colortable)
{
    switch(bit) {
    case 1: {
        const uint8_t *pSrc = imgbuf;
        uint16_t *pDst = imgbuf2;
        const uint8_t *pSrcEnd = imgbuf + (length >> 3);
        uint32_t *pDst32 = (uint32_t*)pDst;
        
        while (pSrc < pSrcEnd) {
            uint8_t byte = *pSrc++;
            uint16_t c0 = colortable[(byte >> 7) & 0x01];
            uint16_t c1 = colortable[(byte >> 6) & 0x01];
            uint16_t c2 = colortable[(byte >> 5) & 0x01];
            uint16_t c3 = colortable[(byte >> 4) & 0x01];
            uint16_t c4 = colortable[(byte >> 3) & 0x01];
            uint16_t c5 = colortable[(byte >> 2) & 0x01];
            uint16_t c6 = colortable[(byte >> 1) & 0x01];
            uint16_t c7 = colortable[(byte >> 0) & 0x01];
            
            // 32位合并写入（小端）
            //#if __LITTLE_END_COLOR__ > 0
            *pDst32++ = ((uint32_t)c1 << 16) | c0;
            *pDst32++ = ((uint32_t)c3 << 16) | c2;
            *pDst32++ = ((uint32_t)c5 << 16) | c4;
            *pDst32++ = ((uint32_t)c7 << 16) | c6;
            // #else
            // *pDst32++ = ((uint32_t)c0 << 16) | c1;
            // *pDst32++ = ((uint32_t)c2 << 16) | c3;
            // *pDst32++ = ((uint32_t)c4 << 16) | c5;
            // *pDst32++ = ((uint32_t)c6 << 16) | c7;
            // #endif
        }
        pDst = (uint16_t*)pDst32;
        
        uint8_t remain = length & 0x07;
        if (remain) {
            uint8_t byte = *pSrc;
            for (uint8_t j = 0; j < remain; j++) {
                *pDst++ = colortable[(byte >> (7 - j)) & 0x01];
            }
        }
        break;
    }
    
    case 2: {
        const uint8_t *pSrc = imgbuf;
        uint16_t *pDst = imgbuf2;
        const uint8_t *pSrcEnd = imgbuf + (length >> 2);
        uint32_t *pDst32 = (uint32_t*)pDst;
        
        while (pSrc < pSrcEnd) {
            uint8_t byte = *pSrc++;
            uint16_t c0 = colortable[(byte >> 6) & 0x03];
            uint16_t c1 = colortable[(byte >> 4) & 0x03];
            uint16_t c2 = colortable[(byte >> 2) & 0x03];
            uint16_t c3 = colortable[(byte >> 0) & 0x03];
            
            //#if __LITTLE_END_COLOR__ > 0
            *pDst32++ = ((uint32_t)c1 << 16) | c0;
            *pDst32++ = ((uint32_t)c3 << 16) | c2;
            // #else 
            // *pDst32++ = ((uint32_t)c0 << 16) | c1;
            // *pDst32++ = ((uint32_t)c2 << 16) | c3;
            // #endif
        }
        pDst = (uint16_t*)pDst32;
        
        uint8_t remain = length & 0x03;
        if (remain) {
            uint8_t byte = *pSrc;
            for (uint8_t j = 0; j < remain; j++) {
                *pDst++ = colortable[(byte >> (6 - j*2)) & 0x03];
            }
        }
        break;
    }
    
    case 4: {
        const uint8_t *pSrc = imgbuf;
        uint16_t *pDst = imgbuf2;
        const uint8_t *pSrcEnd = imgbuf + (length >> 1);
        
        while (pSrc < pSrcEnd) {
            uint8_t byte = *pSrc++;
            *pDst++ = colortable[(byte >> 4) & 0x0F];
            *pDst++ = colortable[(byte >> 0) & 0x0F];
        }
        
        if (length & 0x01) {
            *pDst++ = colortable[(*pSrc >> 4) & 0x0F];
        }
        break;
    }
    
    default:
        return;
    }
}



#define _abs(x) ((x)<0?-(x):(x))

#define RightUpper 0x01
#define LeftUpper 0x02
#define RightLower 0x04
#define LeftLower 0x08

KSC_buf os_screen = {0};

KSC_mes ksetpixel(KSC_buf* screen,KSCCOLOR color,uintxy x,uintxy y){
    screen_setcanvas(x,y,1,1);
    screen_setcolorpixels(&color,1);
    return KSC_OK;
}
KSC_mes ksetscreen(KSC_buf* screen){
	os_screen = *screen;
	return KSC_OK;
}
KSC_buf* kgetscreen(){
	return &os_screen;
}
//初始化缓冲区
KSC_buf* kinitscreen(uintxy ssx,uintxy ssy,uintxy width,uintxy height,KSCCOLOR bk){
    KSC_buf* screen = (KSC_buf*)r_malloc(sizeof(KSC_buf));
    if(screen == NULL){
        return NULL;
       }

    screen->Mode = 1<<7; //标记结构体为malloc分配
    
    screen->ssx = ssx;
    screen->ssy = ssy;
    screen->width = width;
    screen->height = height;
    screen->bk = bk;
    ksetscreen(screen);
	screen_init();
    // kfull(screen,0xFFFF,0,0,width,240);
    // kfull(screen,0xFFFF,0,240,width,80);
    kfull(screen,bk,0,0,width,height);
    return screen;
}

//释放缓冲区
void kfreescreen(KSC_buf* screen){
    if(screen->Mode & (1<<5)){
        r_free(screen->fbuf);
    }
    
    if(screen->Mode & (1<<7)){
        r_free(screen);
    }
}

//end

//高级图形函数
KSC_mes kfull(KSC_buf* screen,KSCCOLOR color,uintxy x1,uintxy y1,uintxy w,uintxy h){
    screen_setcanvas(x1,y1,w,h);
    uint32_t pixelnum =w*h;
    uint16_t* buf = (uint16_t*)k_malloc(_STATICBUF_SIZE);
    uint16_t staticbuf_pixel = (_STATICBUF_SIZE>>1);
    if(buf == NULL){
        return KSC_ERR;
    }
    memset_16(buf,512,color);
    while(pixelnum>staticbuf_pixel){
        screen_setcolorpixels(buf,staticbuf_pixel);
        pixelnum -= staticbuf_pixel;
    }
    screen_setcolorpixels(buf,pixelnum);

    k_free(buf);
    // log("kfull done");
    return KSC_OK;
}
//画线
KSC_mes kline(KSC_buf* screen,KSCCOLOR color,uintxy x1,uintxy y1,uintxy x2,uintxy y2){
    if(y1 == y2&&x1 == x2){
        kfull(screen,color,x1,y1,x2-x1+1,y2-y1+1);
        return KSC_OK;
    }
    int dx = _abs(x2 - x1);
    int dy = _abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;
    
    while(1) {
        ksetpixel(screen,color, x1, y1);
        
        if (x1 == x2 && y1 == y2) break;
        
        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x1 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y1 += sy;
        }
    }return KSC_OK;
}

//画无填充矩形,用批量画线实现
//新增参数width，表示线宽，
KSC_mes kbox(KSC_buf* screen,KSCCOLOR color,uintxy x1,uintxy y1,uintxy w,uintxy h){
    uint8_t depth =1;
        
    kfull(screen,color,x1,y1,w,depth);
    kfull(screen,color,x1,y1,depth,h);
    kfull(screen,color,x1+w-1,y1,depth,h);
    kfull(screen,color,x1,y1+h-1,w,depth);
    return KSC_OK;
}

KSC_mes kfillbox(KSC_buf* screen,KSCCOLOR color,uintxy x1,uintxy y1,uintxy w,uintxy height){
    kfull(screen,color,x1,y1,w,height);
    return KSC_OK;
}

//
//画圆弧
//修改：支持画部分圆弧
//Anglediraction: 右上0x01;左上0x02;右下0x04;左下0x08;全圆0x0F;
KSC_mes karc(KSC_buf* screen,KSCCOLOR color,uintxy x0,uintxy y0,uint8_t r,uint8_t Anglediraction){
    intxy x = r;
    intxy y = 0;
    int err = 0;
    // if(Anglediraction == 0x0F){
    //     kcircle(screen,color,x0,y0,r);
    //     return KSC_OK;
    // }
    //判断是否合法
    if((Anglediraction&0xF0)!=0) return KSC_ERR;
    while (x >= y) {
        if(Anglediraction & RightLower){
            ksetpixel(screen,color, (uintxy)x0 + x, (uintxy)y0 + y);
            ksetpixel(screen,color, (uintxy)x0 + y, (uintxy)y0 + x);
        }
        if(Anglediraction & LeftLower){
            ksetpixel(screen,color, (uintxy)x0 - y, (uintxy)y0 + x);
            ksetpixel(screen,color, (uintxy)x0 - x, (uintxy)y0 + y);
        }
        if(Anglediraction & LeftUpper){
            ksetpixel(screen,color, (uintxy)x0 - x, (uintxy)y0 - y);
            ksetpixel(screen,color, (uintxy)x0 - y, (uintxy)y0 - x);
        }
        if(Anglediraction & RightUpper){
            ksetpixel(screen,color, x0 + y, y0 - x);
            ksetpixel(screen,color, x0 + x, y0 - y);
        }

        y += 1;
        if (err <= 0) {
            err += 2*y + 1;
        }
        if (err > 0) {
            x -= 1;
            err -= 2*x + 1;
        }
    }
    return KSC_OK;
}
//画圆
KSC_mes kcircle(KSC_buf* screen,KSCCOLOR color,uintxy x0,uintxy y0,uint8_t r){
    // karc(screen,color,x0,y0,r,0x0F);
    // return KSC_OK;
    int x = r;
    int y = 0;
    int err = 0;
    while (x >= y) {
        ksetpixel(screen,color, x0 + x, y0 + y);
        ksetpixel(screen,color, x0 + y, y0 + x);
        ksetpixel(screen,color, x0 - y, y0 + x);
        ksetpixel(screen,color, x0 - x, y0 + y);
        ksetpixel(screen,color, x0 - x, y0 - y);
        ksetpixel(screen,color, x0 - y, y0 - x);
        ksetpixel(screen,color, x0 + y, y0 - x);
        ksetpixel(screen,color, x0 + x, y0 - y);

        y += 1;
        if (err <= 0) {
            err += 2*y + 1;
        }
        if (err > 0) {
            x -= 1;
            err -= 2*x + 1;
        }
    }
    return KSC_OK;
}

KSC_mes kfillcircle(KSC_buf* screen,KSCCOLOR color, uintxy x0, uintxy y0, uint8_t r) {
    if (r == 0) {
        kline(screen,color, x0, y0, x0, y0);
        return KSC_OK;
    }
    
    int x = 0;
    int y = r;
    int d = 3 - 2 * r;  // Bresenham决策参数
    
    // 使用数组存储每行的左边界，最后统一绘制
    // 这样可以确保每一行都被正确处理，无空洞
    int left_bounds[r + 1];  // 存储每行的左边界
    
    // 初始化边界数组
    for (int i = 0; i <= r; i++) {
        left_bounds[i] = r + 1;  // 初始化为不可能的大值
    }
    
    // 第一阶段：计算八分之一圆弧的边界点
    while (x <= y) {
        // 更新四个象限的边界
        // 第一象限 (x, y)
        if (y < left_bounds[x]) left_bounds[x] = y;
        if (x < left_bounds[y]) left_bounds[y] = x;
        
        // 第二象限 (-x, y) 对称到 (y, x)
        if (y < left_bounds[x]) left_bounds[x] = y;
        if (x < left_bounds[y]) left_bounds[y] = x;
        
        // Bresenham算法更新
        if (d < 0) {
            d = d + 4 * x + 6;
        } else {
            d = d + 4 * (x - y) + 10;
            y--;
        }
        x++;
    }
    // 第二阶段：根据边界数组绘制完整的圆
    // 绘制上半圆
    for (int dy = 0; dy <= r; dy++) {
        int dx = left_bounds[dy];
        if (dx <= r) {  // 有效边界
            // 上半部分
            kline(screen,color, x0 - dx, y0 + dy, x0 + dx, y0 + dy);
            // 下半部分（对称）
            if (dy > 0) {  // 避免重复绘制中心线
                kline(screen,color, x0 - dx, y0 - dy, x0 + dx, y0 - dy);
            }
        }
    }
    
    return KSC_OK;
}
KSC_mes kdrawimage(KSC_buf* screen,const uint16_t* img,uintxy x,uintxy y
    ,uint8_t width,uint8_t height){
    screen_setcanvas(x,y,width,height); 
    uint16_t imgsize = width*height;
    screen_setcolorpixels((const uint16_t*)img,imgsize);
    return KSC_OK;
}

// 绘制放大图像
KSC_mes kdrawimagebig(KSC_buf* screen,const uint16_t* img,uintxy x,uintxy y
    ,uint8_t width,uint8_t height,uint8_t scale){
    // 遍历原图像的每个像素
    //uint16_t* img = (uint16_t*)img_file+4;
    for(uint8_t h=0; h<height; h++){
        for(uint8_t w=0; w<width; w++){
            // 计算当前像素在图像数据中的偏移量
            //uint16_t offset = (h * width + w) * COLORBYTE;
            // 提取像素颜色
            //
            //KSCCOLOR ncolor =  img[offset] << 8 |img[offset+1];
            KSCCOLOR ncolor =  *img++;
            // #if __DATA_BIG_IMAGE__ == 1
            //     ncolor =  (ncolor<<8)|(ncolor>>8);
            // #endif
            // 放大显示该像素
            kfull(screen,ncolor,x + w * scale,y + h * scale,scale,scale);
        }
    }
    return KSC_OK;
}

//绘制圆角矩形
KSC_mes kroundrect(KSC_buf* screen,KSCCOLOR color,uintxy x1,uintxy y1,uintxy width,uintxy height,uint8_t r){
    if(r==0){
        kfillbox(screen,color,x1,y1,width,height);
        return KSC_OK;
    }else if(2*r>(width)){
        r=(width)/2;
    }else if(2*r>(height)){
        r=(height)/2;
    }
    uintxy x2 = x1 + width;
    uintxy y2 = y1 + height;
    //画四个角的圆弧
    karc(screen,color,x1 + r, y1 + r, r, LeftUpper);
    karc(screen,color,x2 - r, y1 + r, r, RightUpper);
    karc(screen,color,x1 + r, y2 - r, r, LeftLower);
    karc(screen,color,x2 - r, y2 - r, r, RightLower);
    //画四条边的直线
    kline(screen,color,x1 + r, y1, x2 - r,y1); // 上边
    kline(screen,color,x1 + r, y2, x2 - r,y2); // 下边
    kline(screen,color,x1, y1 + r, x1, y2 - r); // 左边
    kline(screen,color,x2, y1 + r, x2, y2 - r); // 右边
    return KSC_OK;
}
//绘制圆角填充矩形
KSC_mes kfillroundrect(KSC_buf* screen,KSCCOLOR color,uintxy x1,uintxy y1,uintxy width,uintxy height,uint8_t r){
    if(r==0){
        kfillbox(screen,color,x1,y1,width,height);
        return KSC_OK;
    }else if(2*r>(width)){
        r=(width)/2;
    }else if(2*r>(height)){
        r=(height)/2;
    }
    uintxy x2 = x1 + width;
    uintxy y2 = y1 + height;
    //填充中间矩形区域
    kfillbox(screen,color,x1 + r, y1, width-2*r, height+1);
    kfillbox(screen,color,x1, y1 + r, width+1, height-2*r);
    
    //填充四个圆角区域
    kfillcircle(screen,color,x1 + r, y1 + r, r);
    kfillcircle(screen,color,x2 - r, y1 + r, r);
    kfillcircle(screen,color,x1 + r, y2 - r, r);
    kfillcircle(screen,color,x2 - r, y2 - r, r);
    return KSC_OK;
}
// 绘制二值化图像
KSC_mes kimagebin(KSC_buf* screen,const uint8_t* img,uintxy x,uintxy y
    ,uint8_t width,uint8_t height,KSCCOLOR colorck,KSCCOLOR colorbk){
    uint16_t* imgbuf =(uint16_t*)k_malloc(width*height*COLORBYTE);
    if(!imgbuf) return KSC_ERR;
    uint16_t colortable[2] = {colorbk,colorck};
    imgchange(img,imgbuf,width*height,1,colortable);
    //kdrawimage(screen,imgbuf,x,y,width,height);
    kdrawimage(screen,imgbuf,x,y,width,height);
    k_free(imgbuf);
    return KSC_OK;
}

KSC_mes kchar(KSC_buf* screen,char ch,uintxy x,uintxy y,KSCCOLOR colorck,KSCCOLOR colorbk){
	KSC_Font1* font=&Systemfont0;
    const uint8_t* char_bitmap = font->Getfontfunc(ch);
    kimagebin(screen,char_bitmap,x,y,font->width,font->height,colorck,colorbk);
    return KSC_OK;
}
KSC_mes kstring(KSC_buf* screen,const char* str,uintxy x,uintxy y,KSCCOLOR colorck,KSCCOLOR colorbk){
    KSC_Font1* font=&Systemfont0;
    if(screen==NULL)screen=kgetscreen();
    while(*str){
        kchar(screen,*str,x,y,colorck,colorbk);
        #if SYSTEMFONT == 7
        x += font->width-1;
        #else
        x += font->width;
        #endif
        str++;
    }
    return KSC_OK;
}
#if __USE_CHINESE__ >0
//#include "../inc/UTF8_FlashN.h"
KSC_mes kstringchinese(KSC_buf* screen,const char* str,uintxy x,uintxy y,KSCCOLOR color1,KSCCOLOR color2){
    KSC_FontChinese* font = &SystemfontChinese;
    if(!screen) screen=kgetscreen();
    uint8_t* char_bitmap = NULL;
    uint32_t count = utf8get(str, 0,0);
    for(uint8_t j=0;j<count;j++){
        char_bitmap = font->Getfontfunc(str,j);
            kimagebin(screen,x+j*font->width,y
                ,char_bitmap,font->width,font->height,color1,color2);
    }
    return KSC_OK;
}
#endif

#endif
