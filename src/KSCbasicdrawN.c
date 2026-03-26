////////////////////////////////////////////////////////////////////////
// @file KSCbasicdraw.c
// @brief 基本绘图函数
// @author OneSleepWorm(一只瞌睡虫)
// @date 2026/1/31
//////////////////////////////////////////////////////////////////
// detail:
//  1. 基本绘图函数
//  2. 包括矩形、圆角矩形、填充矩形、圆角填充矩形、线、点、圆、填充圆等
//  3. 需要包含KSCdisplay.h
//  4. 在Work1.h的先例上，修改推出了无需大缓冲区的绘图函数，适用于资源受限的嵌入式系统
//  5. 相对的缺陷，无法获取画布任意像素点的颜色值，不稳定可能失效的动态缓冲区
//  6. 推荐自主准备静态内存池，避免动态内存分配失败导致的绘图失败，并预设内存大小
//  7. 即时刷新，无需调用Flashbuf函数
///////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
/*  program:    屏幕缓冲区映射
 *  author:     一只瞌睡虫(OneSleepWorm)
 *  version:    V1.0.0
 *  Firstdate:  2025/11/12__0:02
 *  Lastdate:   2025/11/16__21:16
 */
/////////////////////////////////////////////////////////////////

/*  version:    V2.0.0
 *  Firstdate:  2025/12/07__23:00
 *  Lastdate:   2025/12/17__23:20
 */ 
//////////////////////////////////////////////////////////////////
/* V1.0.0 更新；基本实现了基本绘图功能，后续会添加更多功能
 * V2.0.0 更新：像素不再局限于1bit，可以处理2bit，在屏幕结构体中设置像素宽度和像素调色盘
 * V2.1.0 更新：重构了基于旧画线的画线函数
 * V2.1.1 更新：修复了矩形绘制函数的bug，现在不能设置线宽，默认1像素
 * V2.2.0 更新：优化了函数
 */

////////////////////////////////////////////////////////////////
#include "../inc/KSCbasicdrawN.h"



#define _abs(x) ((x)<0?-(x):(x))
#define MAX_Points 64

#define RightUpper 0x01
#define LeftUpper 0x02
#define RightLower 0x04
#define LeftLower 0x08

KSC_buf os_screen = {0};

/**
 * @brief 将16位RGB565格式颜色转换为24位RGB888格式
 * @param color16 16位RGB565格式颜色值
 * @return 24位RGB888格式颜色值
 */
uint32_t kconvert16to24(uint16_t color16) {
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

//比特、像素处理函数
/**
 * 获取uint8_t类型数组中指定bit位的值
 * @param array uint8_t类型的数组
 * @param bitPosition 要查询的bit位位置(从0开始计数)
 * @return 对应bit位的值(0或1)
 */
uint8_t getBitValue(const uint8_t* array, uint16_t bitPosition) {
    // 计算bit位所在的字节索引
    uint16_t byteIndex = bitPosition / 8;
    // 计算在该字节中的bit位偏移量
    uint8_t bitOffset = 7-bitPosition % 8;
    // 返回对应bit位的值(0或1)
    return (array[byteIndex] >> bitOffset) & 0x01;
}
// uint8_t K_GetByte_Offset(uintxy x){
//     switch(COLORBIT){
//         //这部分有点玄乎，其实，就是取掩码+合理右移,
//         case 1:  return (7-(x&0x07));break;
//         case 2:  return (6-((x&0x03)<<1));break;
//         case 4:  return ((4-((x&0x01)<<2)));break;
//         case 8:  return 0;break;
//         default: return 0xFF;
//     }
// }
void screen_setcanva_s(KSC_buf* screen,uintxy x,uintxy y,uintxy width,uintxy height){
    screen_setcanvas(x+screen->ssx,y+screen->ssy,width,height);
}
KSC_mes ksetpixel(KSC_buf* screen,uintxy x,uintxy y){
    screen_setcanva_s(screen,x,y,1,1);
    screen_setcolorpixel(screen->pen1);
    return KSC_OK;
}
KSC_mes ksetpixel_s(KSC_buf* screen,const uintxy x, const uintxy y, /*__restrict */ const uint8_t*bits,uint16_t num){
    screen_setcanva_s(screen,x,y,num,1);
    //printf("%d\n",num);
    //Sleep(100);
    KSCCOLOR bit1_value = screen->pen1;
    KSCCOLOR bit0_value = screen->pen0;
            for(uint16_t i=0;i<num;i++){
                uint8_t bit = getBitValue(bits,i);
                screen_setcolorpixel(bit?bit1_value:bit0_value); 
            }
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
KSC_buf* kinitscreen(uintxy width,uintxy height,uint8_t* Buffer,uintxy ssx,uintxy ssy){
    KSC_buf* screen = (KSC_buf*)K_malloc(sizeof(KSC_buf));
    screen->Mode = 1<<7; //标记结构体为malloc分配
    if(Buffer == NULL){
        //分配屏幕缓冲区内存
        screen->fbuf = (uint8_t*)K_malloc(width/8);
        screen->Mode |= 1<<5; //标记fbuf为malloc分配
    }else{ screen->fbuf = Buffer;}
    screen->pen1 = WHITE;
    screen->pen0 = BLACK;
    screen->ssx = ssx;
    screen->ssy = ssy;
    //screen->pen0 = screen->color_table->color0;
    //screen->f_size = width*height/8*COLORBIT;
    screen->width = width;
    screen->height = height;
    ksetscreen(screen);
	screen_init();
    kfillrect(screen,0,0,width,height);
    screen->pen1 = GREEN;
    return screen;
}

//释放缓冲区
void kfreescreen(KSC_buf* screen){ 
    if(screen->Mode & (1<<5)){
        free(screen->fbuf);
    }
    
    if(screen->Mode & (1<<7)){
        free(screen);
    }
}


KSCCOLOR kgetpencolor(KSC_buf* screen,uint8_t pen){
    KSCCOLOR Color = 0;
    switch(pen){
        case 0: Color = screen->pen0;break;
        case 1: Color = screen->pen1;break;
        default: Color = screen->pen0;break;
    }return Color;
}
KSC_mes ksetpencolor(KSC_buf* screen,KSCCOLOR color,uint8_t pen){
    if(pen == 1){screen->pen1 = color;}
    else if(pen == 0){screen->pen0 = color;}
    else{screen->pen1 = color;return KSC_ERR;}
    return KSC_OK;
}

//end
//高级图形函数

//画线简单版，只能画水平线，但是快速
KSC_mes khline(KSC_buf* screen,uintxy x1,uintxy y1,uintxy x2){
    uintxy dx = _abs(x2 - x1)+1;
    uint8_t bits[TFTx/8*COLORBIT] = {0xFF};
    for(uintxy i=0;i<=dx/8+1;i++){
        bits[i] = 0xFF;
    }
    //printf("画线开始\n");
    ksetpixel_s(screen,x1,y1,bits,dx);
    //getch();
    //printf("画线完成\n");
    return KSC_OK;
}

//画线
KSC_mes kline(KSC_buf* screen,uintxy x1,uintxy y1,uintxy x2,uintxy y2){
    if(y1 == y2){
        khline(screen,x1,y1,x2);
        return KSC_OK;
    }
    int dx = _abs(x2 - x1);
    int dy = _abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;
    
    while(1) {
        ksetpixel(screen, x1, y1);
        
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
//两点结构体
typedef struct {
    uintxy x1;
    uintxy y1;
    uintxy x2;
    uintxy y2;
}KSC_Points;
//批量画线
KSC_mes klines(KSC_buf* screen,KSC_Points* points,uint8_t num){
    for(uint8_t i=0;i<num;i++){
        kline(screen,points[i].x1,points[i].y1,points[i].x2,points[i].y2);
    }
    return KSC_OK;
}

//画无填充矩形,用批量画线实现
//新增参数width，表示线宽，
KSC_mes krect(KSC_buf* screen,uintxy x1,uintxy y1,uintxy w,uintxy h){
    //uintxy Dw = width -1;
    uintxy x2 = x1 + w - 1;
    uintxy y2 = y1 + h - 1;
    KSC_Points rectlines[4] = {
        
        {(uintxy)(x2), y1, x2, y2}, // 右边
        {x1, (uintxy)(y2), x2, y2}, // 下边
        {x1, y1, (uintxy)(x1), y2}, // 左边
        {x1, y1, x2, (uintxy)(y1)}  // 上边
    };
    klines(screen, rectlines, 4);
    return KSC_OK;
}
//画有填充矩形,用批量画线实现
KSC_mes kfillrect(KSC_buf* screen,uintxy x1,uintxy y1,uintxy x2,uintxy y2){
    for(uintxy y = y1; y <= y2; y++){
        khline(screen,x1,y,x2);
    }
    return KSC_OK;
}

//
//画圆弧
//修改：支持画部分圆弧
//Anglediraction: 右上0x01;左上0x02;右下0x04;左下0x08;全圆0x0F;
KSC_mes karc(KSC_buf* screen,uintxy x0,uintxy y0,uint8_t r,uint8_t Anglediraction){
    intxy x = r;
    intxy y = 0;
    int err = 0;
    if(Anglediraction == 0x0F){
        kcircle(screen,x0,y0,r);
        return KSC_OK;
    }
    //判断是否合法
    switch(Anglediraction){
        case RightLower:if(!(x0+r<screen->width && y0+r<screen->height)){return KSC_ERR;}break;
        case LeftLower:if(!(x0>=r && y0+r<screen->height)){return KSC_ERR;}break;
        case RightUpper:if(!(x0+r<screen->width && y0>=r)){return KSC_ERR;}break;
        case LeftUpper:if(!(x0>=r && y0>=r)){return KSC_ERR;}break;
        default:
            return KSC_ERR;
    }
    while (x >= y) {
        if(Anglediraction & RightLower){
            ksetpixel(screen, (uintxy)x0 + x, (uintxy)y0 + y);
            ksetpixel(screen, (uintxy)x0 + y, (uintxy)y0 + x);
        }
        if(Anglediraction & LeftLower){
            ksetpixel(screen, (uintxy)x0 - y, (uintxy)y0 + x);
            ksetpixel(screen, (uintxy)x0 - x, (uintxy)y0 + y);
        }
        if(Anglediraction & LeftUpper){
            ksetpixel(screen, (uintxy)x0 - x, (uintxy)y0 - y);
            ksetpixel(screen, (uintxy)x0 - y, (uintxy)y0 - x);
        }
        if(Anglediraction & RightUpper){
            ksetpixel(screen, x0 + y, y0 - x);
            ksetpixel(screen, x0 + x, y0 - y);
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
KSC_mes kcircle(KSC_buf* screen,uintxy x0,uintxy y0,uint8_t r){

    int x = r;
    int y = 0;
    int err = 0;
    while (x >= y) {
        ksetpixel(screen, x0 + x, y0 + y);
        ksetpixel(screen, x0 + y, y0 + x);
        ksetpixel(screen, x0 - y, y0 + x);
        ksetpixel(screen, x0 - x, y0 + y);
        ksetpixel(screen, x0 - x, y0 - y);
        ksetpixel(screen, x0 - y, y0 - x);
        ksetpixel(screen, x0 + y, y0 - x);
        ksetpixel(screen, x0 + x, y0 - y);

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

KSC_mes kfillcircle(KSC_buf* screen, uintxy x0, uintxy y0, uint8_t r) {
    if (r == 0) {
        khline(screen, x0, y0, x0);
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
            khline(screen, x0 - dx, y0 + dy, x0 + dx);
            // 下半部分（对称）
            if (dy > 0) {  // 避免重复绘制中心线
                khline(screen, x0 - dx, y0 - dy, x0 + dx);
            }
        }
    }
    
    return KSC_OK;
}
KSC_mes kdrawimage(KSC_buf* screen,uintxy x,uintxy y,const uint8_t* img
    ,uint8_t width,uint8_t height){
    screen_setcanva_s(screen,x,y,width,height);
    uint16_t imgsize = width*height*COLORBYTE;
    for(uint16_t j=0;j<imgsize;j+=COLORBYTE){
        KSCCOLOR ncolor = img[j] << 8 | img[j+1];
		#if __USE_GCC__
        ncolor=kconvert16to24(ncolor);
		#endif
		screen_setcolorpixel(ncolor);
        // printf("%d\n",j);
        // Sleep(50);
    }
    return KSC_OK;
}

// 绘制放大图像
KSC_mes kdrawimagebig(KSC_buf* screen,uintxy x,uintxy y,const uint8_t* img,uint8_t width,uint8_t height,uint8_t scale){
    // 遍历原图像的每个像素
    for(uint8_t h=0; h<height; h++){
        for(uint8_t w=0; w<width; w++){
            // 计算当前像素在图像数据中的偏移量
            uint16_t offset = (h * width + w) * COLORBYTE;
            // 提取像素颜色
            KSCCOLOR ncolor = img[offset] << 8 | img[offset+1];
			#ifdef __USE_GCC__
            ncolor = kconvert16to24(ncolor);
            #endif
            // 放大显示该像素
            for(uint8_t sh=0; sh<scale; sh++){
                for(uint8_t sw=0; sw<scale; sw++){
                    // 计算放大后的像素位置
                    uintxy scaled_x = x + w * scale + sw;
                    uintxy scaled_y = y + h * scale + sh;
                    // 设置当前画布位置
                    screen_setcanva_s(screen,scaled_x, scaled_y, 1,1);
                    // 绘制放大后的像素
                    screen_setcolorpixel(ncolor);
                }
            }
        }
    }
    return KSC_OK;
}

//绘制圆角矩形
KSC_mes kroundrect(KSC_buf* screen,uintxy x1,uintxy y1,uintxy x2,uintxy y2,uint8_t r){
    //画四个角的圆弧
    karc(screen,x1 + r, y1 + r, r, LeftUpper);
    karc(screen,x2 - r, y1 + r, r, RightUpper);
    karc(screen,x1 + r, y2 - r, r, LeftLower);
    karc(screen,x2 - r, y2 - r, r, RightLower);
    //画四条边的直线
    khline(screen,x1 + r, y1, x2 - r); // 上边
    khline(screen,x1 + r, y2, x2 - r); // 下边
    kline(screen,x1, y1 + r, x1, y2 - r); // 左边
    kline(screen,x2, y1 + r, x2, y2 - r); // 右边
    return KSC_OK;
}
//绘制圆角填充矩形
KSC_mes kfillroundrect(KSC_buf* screen,uintxy x1,uintxy y1,uintxy x2,uintxy y2,uint8_t r){
    //填充中间矩形区域
    kfillrect(screen,x1 + r, y1, x2 - r, y2);
    kfillrect(screen,x1, y1 + r, x2, y2 - r);
    
    //填充四个圆角区域
    kfillcircle(screen,x1 + r, y1 + r, r);
    kfillcircle(screen,x2 - r, y1 + r, r);
    kfillcircle(screen,x1 + r, y2 - r, r);
    kfillcircle(screen,x2 - r, y2 - r, r);
    return KSC_OK;
}
// 绘制二值化图像
KSC_mes kimagebin(KSC_buf* screen,uintxy x,uintxy y,const uint8_t* img,uint8_t width,uint8_t height){
    screen_setcanva_s(screen,x,y,width,height);
    for(uint8_t i = 0; i < height; i++) {
        ksetpixel_s(screen,x,y+i,img + i*((width + 7) / 8), width);
    }
    return KSC_OK;
}

KSC_mes kchar(KSC_buf* screen,char ch,uintxy x,uintxy y){
	KSC_Font1* font=&Systemfont0;
    const uint8_t* char_bitmap = font->Getfontfunc(ch);
    kimagebin(screen,x,y,char_bitmap,font->width,font->height);
    return KSC_OK;
}
KSC_mes kstring(KSC_buf* screen,const char* str,uintxy x,uintxy y){
    KSC_Font1* font=&Systemfont0;
    while(*str){
        kchar(screen,*str,x,y);
        x += font->width; // 每个字符宽度为8像素
        str++;
    }
    
    return KSC_OK;
}
#if __USE_CHINESE__ ==1
#include "../inc/UTF8_FlashN.h"
KSC_mes kstringchinese(KSC_buf* screen,const char* str,uintxy x,uintxy y){
    KSC_FontChinese* font = &SystemfontChinese;
    #ifdef SYSTEMCHINESEFONT
    uint32_t addr_array[32] = {0};
    uint8_t* char_bitmap = NULL;
    // uint8_t testdata[32]={//"业"//26
    // 0x00,0x00,0x00,0x00,0x00,0x00,0x02,0xC0,0x06,0xC0,0x26,0xC8,0x36,0xC8,0x16,0xD8,
    // 0x0E,0xF0,0x06,0xC0,0x7F,0xFF,0x70,0x7E,0x00,0x04,0x00,0x00,0x00,0x00,0x00,0x00};
    uint8_t addr_count = utf8_str_to_addr_array((char*)str, addr_array);
    //font->Getfontfunc(str[0]);
    for(uint8_t j=0;j<addr_count;j++){
        char_bitmap = font->Getfontfunc(addr_array[j]);
        //char_bitmap = testdata;
        for(uint8_t i = 0; i < font->height; i++) {
            ksetpixel_s(screen,x+j*font->width,y+i
                ,char_bitmap+i*((font->width+7)/8),font->width);
            
            //printf("data:%02X,%02X\n",char_bitmap[i*2],char_bitmap[i*2+1]);
        }
        printf("\n");
    }
    #endif
    return KSC_OK;
}
#endif
