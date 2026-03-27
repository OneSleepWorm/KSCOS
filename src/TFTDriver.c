/* 
 * @anthor: OneSleepWorm
 * @Contact Information: a730b689c636@qq.com
 * @Date: 2026-03-27 10:00:00
 * @LastEditTime: 2026-03-27 10:00:00
 * @FilePath: \src\TFTDriver.c
 * @Description: TFT驱动实现
 * @Version: 1.0.1
 */
//////////////////////////////////////////
/*
 * TFTDriver.c是一个TFT驱动实现文件
 * 在这里，你需要参考商家提供的例程驱动实现三个函数，分别是：
 * TFT_Init()初始化TFT屏幕
 * TFT_Setcanvas()设置画布位置
 * TFT_Setcolor()写入画布颜色
 * 注意，不能去掉条件编译宏，否则可能会影响编译
 * 若驱动中需要延时，建议使用下列的参考函数
 * 引脚尽量使用下列推荐的引脚定义
 * 如果感觉速度慢，可以尝试使用下列加快时钟频率的函数，但需谨慎使用
 */
//////////////////////////////////////////
#include "../inc/TFTDriver.h"
#ifdef __USE_ARMCC__

//推荐引脚定义
/* SCL/SCK:B5
 * SDA:B6
 * RST:B7
 * DC/AO:B8
 * CS:B9
 */

//stm32的延时函数参考
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
// //若使用HAL库的延时函数
// #define Delay_ms(xms) HAL_Delay(xms)

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
// 函数：TFT_Init
// 作用：初始化TFT液晶屏（基于ST7735R驱动）
void TFT_Init(void)
{
    // 初始化TFT液晶屏
}

// 函数：TFT_Setcanvas
// 作用：设置TFT液晶屏的显示区域（写入范围）
// 参数：sx, sy - 起始坐标；ex, ey - 结束坐标
void TFT_Setcanvas(uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey)
{
    // 设置显示区域
}

// 函数：TFT_Setcolor
// 作用：在当前光标位置写入一个16位颜色值
// 参数：color - 16位颜色值（RGB565格式）
void TFT_Setcolor(uint16_t color)
{
    // 写入颜色
}

#endif
