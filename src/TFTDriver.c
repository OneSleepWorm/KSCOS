/*
 * @Author: a730b689c636@qq.com
 * @Date: 2026-03-27 10:00:00
 * @LastEditTime: 2026-03-27 10:00:00
 * @FilePath: \KSCdraw\src\Driver_GUI.c
 * @Description: 图形界面层实现
 * @Version: 1.0.1
 */
#include "../inc/TFTDriver.h"
#ifdef __USE_ARMCC__

// #define SPI_SCK_0  GPIOB->BRR=0X0020       // 设置sck接口到PB5   清零
// #define SPI_SCK_1  GPIOB->BSRR=0X0020       //置位
// #define SPI_SDA_0  GPIOB->BRR=0X0040       // 设置SDA接口到PB6
// #define SPI_SDA_1  GPIOB->BSRR=0X0040
// #define SPI_RST_0  GPIOB->BRR=0X0080       // 设置RST接口到PB7
// #define SPI_RST_1  GPIOB->BSRR=0X0080
// #define SPI_DC_0   GPIOB->BRR=0X0100      // 设置DC接口到PB8
// #define SPI_DC_1   GPIOB->BSRR=0X0100
// #define SPI_WR_0   GPIOB->BRR=0X0200    // 设置/WR 或CS接口到PB9
// #define SPI_WR_1   GPIOB->BSRR=0X0200

// #define SPI_SCK_0  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET)
// #define SPI_SCK_1  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET)
// #define SPI_SDA_0  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET)
// #define SPI_SDA_1  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET)
// #define SPI_RST_0  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_RESET)
// #define SPI_RST_1  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_SET)
// #define SPI_DC_0   HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_RESET)
// #define SPI_DC_1   HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_SET)
// #define SPI_WR_0   HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_RESET)
// #define SPI_WR_1   HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_SET)

// void SYS_init(unsigned char PLL)
// {

// 	RCC->APB1RSTR = 0x00000000;//复位结束
// 	RCC->APB2RSTR = 0x00000000;
//   	RCC->AHBENR = 0x00000014;  //睡眠模式闪存和SRAM时钟使能.其他关闭.
//   	RCC->APB2ENR = 0x00000000; //外设时钟关闭.
//   	RCC->APB1ENR = 0x00000000;
// 	RCC->CR |= 0x00000001;     //使能内部高速时钟HSION

// 	RCC->CFGR &= 0xF8FF0000;   //复位SW[1:0],HPRE[3:0],PPRE1[2:0],PPRE2[2:0],ADCPRE[1:0],MCO[2:0]
// 	RCC->CR &= 0xFEF6FFFF;     //复位HSEON,CSSON,PLLON
// 	RCC->CR &= 0xFFFBFFFF;     //复位HSEBYP
// 	RCC->CFGR &= 0xFF80FFFF;   //复位PLLSRC, PLLXTPRE, PLLMUL[3:0] and USBPRE
// 	while(((RCC->CFGR>>2)& 0x03 )!=0x00);
// 	RCC->CIR = 0x00000000;     //关闭所有中断
// 	//配置向量表

// //	SCB->VTOR = 0x08000000|(0x0 & (u32)0x1FFFFF80);//设置NVIC的向量表偏移寄存器

//  	RCC->CR|=0x00010000;  //外部高速时钟使能HSEON
// 	while(((RCC->CR>>17)&0x00000001)==0);//等待外部时钟就绪
// 	RCC->CFGR=0X00000400; //APB1=DIV2;APB2=DIV1;AHB=DIV1;
// 	PLL-=2;//抵消2个单位
// 	RCC->CFGR|=PLL<<18;   //设置PLL值 2~16
// 	RCC->CFGR|=1<<16;	  //PLLSRC ON

// 	RCC->CR|=0x01000000;  //PLLON
// 	while(!(RCC->CR>>25));//等待PLL锁定
// 	RCC->CFGR|=0x00000002;//PLL作为系统时钟
// 	while(((RCC->CFGR>>2)&0x03)!=0x02);   //等待PLL作为系统时钟设置成功

// }

// 函数：TFT_Init
// 作用：初始化TFT液晶屏（基于ST7735R驱动）
void TFT_Init(void)
{
}

// 函数：TFT_Setsursor
// 作用：设置TFT液晶屏的显示区域（写入范围）
// 参数：sx, sy - 起始坐标；ex, ey - 结束坐标
void TFT_Setsursor(uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey)
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
