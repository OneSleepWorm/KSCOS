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
    //uint32_t _color16 = color16 & 0xFF|color16<<8;
    // 提取RGB565各分量
    uint8_t r5 = (color16 >> 11) & 0x1F;  // 红色分量（5位）
    uint8_t g6 = (color16 >> 5) & 0x3F;   // 绿色分量（6位）
    uint8_t b5 = (color16 & 0x1F);          // 蓝色分量（5位）
    
    // 将分量扩展到8位
    uint8_t r8 = (r5 << 3) | (r5 >> 2);   // 5位扩展到8位
    uint8_t g8 = (g6 << 2) | (g6 >> 4);   // 6位扩展到8位
    uint8_t b8 = (b5 << 3) | (b5 >> 2);   // 5位扩展到8位
    
    // 组合为24位RGB888格式
    return (b8 << 16) | (g8 << 8) | r8;
}
#if __USE_PC__
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
// #define TFTx 240
// #define TFTy 320
#define SCALE 3

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
    //printf("x,y,color:%d,%d,%d\r\n",sCx,sCy,color);
            //putpixel(sCx,sCy,color16to24(color));
            //setcolor(color16to24(color));
            //uint8_t* ncolor = (uint8_t*)color;
            while(num--){
                KSCCOLOR ncolor = *color++;
                ncolor = (ncolor)>>8| (ncolor<<8);
                setfillcolor(color16to24(ncolor));
                solidrectangle(sCx*SCALE,sCy*SCALE,sCx*SCALE+SCALE,sCy*SCALE+SCALE);
                movecursor();
            }
          return;
}

#endif

#if __USE_ARMCC__
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

//stm32的延时函数参考，使用SysTick定时器实现，通用延时函数
// /**
//   * @brief  微秒级延时
//   * @param  xus 延时时长，范围：0~233015
//   * @retval 无
//   */
// void Delay_us(uint32_t xus)
// {
// 	SysTick->LOAD = 72 * xus;				//设置定时器重装值
// 	SysTick->VAL = 0x00;					//清空当前计数值
// 	SysTick->CTRL = 0x00000005;				//设置时钟源为HCLK，启动定时器
// 	while(!(SysTick->CTRL & 0x00010000));	//等待计数到0
// 	SysTick->CTRL = 0x00000004;				//关闭定时器
// }

// /**
//   * @brief  毫秒级延时
//   * @param  xms 延时时长，范围：0~4294967295
//   * @retval 无
//   */
// void Delay_ms(uint32_t xms)
// {
// 	while(xms--)
// 	{
// 		Delay_us(1000);
// 	}
// }
// //若使用HAL库,毫秒级延时可选用
// #define Delay_ms(xms) HAL_Delay(xms)
//

//这个函数能加快时钟频率，提升绘制速度，但可能会影响计时，需谨慎使用
/*
void SYS_init(unsigned char PLL)
{

	RCC->APB1RSTR = 0x00000000;//复位结束
	RCC->APB2RSTR = 0x00000000;
  	RCC->AHBENR = 0x00000014;  //睡眠模式闪存和SRAM时钟使能.其他关闭.
  	RCC->APB2ENR = 0x00000000; //外设时钟关闭.
  	RCC->APB1ENR = 0x00000000;
	RCC->CR |= 0x00000001;     //使能内部高速时钟HSION

	RCC->CFGR &= 0xF8FF0000;   //复位SW[1:0],HPRE[3:0],PPRE1[2:0],PPRE2[2:0],ADCPRE[1:0],MCO[2:0]
	RCC->CR &= 0xFEF6FFFF;     //复位HSEON,CSSON,PLLON
	RCC->CR &= 0xFFFBFFFF;     //复位HSEBYP
	RCC->CFGR &= 0xFF80FFFF;   //复位PLLSRC, PLLXTPRE, PLLMUL[3:0] and USBPRE
	while(((RCC->CFGR>>2)& 0x03 )!=0x00);
	RCC->CIR = 0x00000000;     //关闭所有中断
	//配置向量表

//	SCB->VTOR = 0x08000000|(0x0 & (u32)0x1FFFFF80);//设置NVIC的向量表偏移寄存器

 	RCC->CR|=0x00010000;  //外部高速时钟使能HSEON
	while(((RCC->CR>>17)&0x00000001)==0);//等待外部时钟就绪
	RCC->CFGR=0X00000400; //APB1=DIV2;APB2=DIV1;AHB=DIV1;
	PLL-=2;//抵消2个单位
	RCC->CFGR|=PLL<<18;   //设置PLL值 2~16
	RCC->CFGR|=1<<16;	  //PLLSRC ON
	RCC->CR|=0x01000000;  //PLLON
	while(!(RCC->CR>>25));//等待PLL锁定
	RCC->CFGR|=0x00000002;//PLL作为系统时钟
	while(((RCC->CFGR>>2)&0x03)!=0x02);   //等待PLL作为系统时钟设置成功
}
*/
