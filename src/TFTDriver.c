/*
 * @Author: 123456789@qq.com
 * @Date: 2023-04-20 10:00:00
 * @LastEditTime: 2023-04-20 10:00:00
 * @FilePath: \OLED_SYSTEM\Driver_GUI.c
 * @Description: 图形界面层实现
 * @Version: 1.0
 * @LastEditors: 123456789@qq.com
 * @Copyright: 123456789@qq.com
 */
#include "../inc/TFTDriver.h"
#if __USE_ARMCC__
#include "stm32f1xx_hal.h"

//#include "stm32f10x.h"
//#include "GUI_BIOS.h"
//#include <stdlib.h>  // For malloc

#define		__COLOR_H__
//#define     RED          0XF800	  //红色
//#define     GREEN        0X07E0	  //绿色
//#define     BLUE         0X001F	  //蓝色
//#define     WHITE        0XFFFF	  //白色

#define TFT_COLUMN_NUMBER 240
#define TFT_LINE_NUMBER 320
#define TFT_COLUMN_OFFSET 0

#define PIC_NUM 28800			//图片数据大小


/**********SPI引脚分配，连接TFT屏，更具实际情况修改*********/

//#define SPI_SCK_0  GPIOB->BRR=0X0020       // 设置sck接口到PB5   清零
//#define SPI_SCK_1  GPIOB->BSRR=0X0020       //置位
//#define SPI_SDA_0  GPIOB->BRR=0X0040       // 设置SDA接口到PB6
//#define SPI_SDA_1  GPIOB->BSRR=0X0040
//#define SPI_RST_0  GPIOB->BRR=0X0080       // 设置RST接口到PB7
//#define SPI_RST_1  GPIOB->BSRR=0X0080
//#define SPI_DC_0   GPIOB->BRR=0X0100      // 设置DC接口到PB8
//#define SPI_DC_1   GPIOB->BSRR=0X0100
//#define SPI_WR_0   GPIOB->BRR=0X0200    // 设置/WR 或CS接口到PB9
//#define SPI_WR_1   GPIOB->BSRR=0X0200


#define SPI_SCK_0  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET)
#define SPI_SCK_1  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET)
#define SPI_SDA_0  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET)
#define SPI_SDA_1  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET)
#define SPI_RST_0  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_RESET)
#define SPI_RST_1  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_SET)
#define SPI_DC_0   HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_RESET)
#define SPI_DC_1   HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_SET)
#define SPI_WR_0   HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_RESET)
#define SPI_WR_1   HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_SET)

const unsigned char  *point;

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

void IO_init(void)
{
	RCC->APB2ENR|=1<<3;    //使能PORTB时钟 
	GPIOB->CRL&=0X000FFFFF;				//将B567口配置为通用推挽输出,最大50MH
	GPIOB->CRL|=0X33300000;				
	GPIOB->CRH&=0XFFFFFF00;				//将B89口配置为通用推挽输出,最大50MH
	GPIOB->CRH|=0X00000033;
	GPIOB->ODR=0XFFFF;
}
// 原 delay_ms 替换为 HAL_Delay()（HAL 库自带，毫秒级）
// 微秒级延时需自己实现（F1 无 HAL_Delay_us，需借助 SysTick）
void delay_us(uint32_t us) {
    uint32_t ticks = (HAL_RCC_GetHCLKFreq() / 1000000) * us;
    uint32_t start = SysTick->VAL;
    while ((SysTick->VAL - start) < ticks);
}
void delay_ms(unsigned int _ms_time)
  {
    HAL_Delay(_ms_time);
  }
/*************SPI配置函数*******************
SCL空闲时低电平，第一个上升沿采样
模拟SPI
******************************************/

/**************************SPI模块发送函数************************************************

 *************************************************************************/
void SPI_SendByte(unsigned  char byte)				//向液晶屏写一个8位数据
{
  
  unsigned char counter;
	SPI_WR_0;
   
  for(counter=0;counter<8;counter++)
  { 
    SPI_SCK_0;	  
    if((byte&0x80)==0)
    {
      SPI_SDA_0;
    }
    else SPI_SDA_1;
    byte=byte<<1;	
    SPI_SCK_1;			
  }
  
  
		SPI_WR_1;				//上拉WR确定数据输入
	SPI_SCK_0;
}

void TFT_SEND_CMD(unsigned char o_command)
  {
    SPI_DC_0;
    //SPI_WR_0;
    SPI_SendByte(o_command);
   // SPI_WR_1;
   
    //SPI_DC_1;
  }
  //向液晶屏写一个8位数据
void TFT_SEND_DATA(unsigned  char o_data)
  { 
    SPI_DC_1;
   // SPI_WR_0;
    SPI_SendByte(o_data);
   // SPI_WR_1;
    
   }
void TFT_clear(void)
  {
    unsigned int ROW,column;
  TFT_SEND_CMD(0x2a);     //Column address set
  TFT_SEND_DATA(0x00);    //start column
  TFT_SEND_DATA(0x00); 
  TFT_SEND_DATA(0x00);    //end column
  TFT_SEND_DATA(0xF0);

  TFT_SEND_CMD(0x2b);     //Row address set
  TFT_SEND_DATA(0x00);    //start row
  TFT_SEND_DATA(0x00); 
  TFT_SEND_DATA(0x01);    //end row
  TFT_SEND_DATA(0x40);
    TFT_SEND_CMD(0x2C);     //Memory write
    for(ROW=0;ROW<TFT_LINE_NUMBER;ROW++)             //ROW loop
      { 
    
          for(column=0;column<TFT_COLUMN_NUMBER;column++)  //column loop
            {
              
				TFT_SEND_DATA(0xFF);
				TFT_SEND_DATA(0xFF);
            }
      }
  }
void TFT_full(unsigned int color)
  {
    unsigned int ROW,column;
    TFT_SEND_CMD(0x2a);     //Column address set
  TFT_SEND_DATA(0x00);    //start column
  TFT_SEND_DATA(0x00); 
  TFT_SEND_DATA(0x00);    //end column
  TFT_SEND_DATA(0xF0);

  TFT_SEND_CMD(0x2b);     //Row address set
  TFT_SEND_DATA(0x00);    //start row
  TFT_SEND_DATA(0x00); 
  TFT_SEND_DATA(0x01);    //end row
  TFT_SEND_DATA(0x40);
    TFT_SEND_CMD(0x2C);     //Memory write
    for(ROW=0;ROW<TFT_LINE_NUMBER;ROW++)             //ROW loop
      { 
    
  for(column=0;column<TFT_COLUMN_NUMBER ;column++) //column loop
          {

			TFT_SEND_DATA(color>>8);
			  TFT_SEND_DATA(color);
          }
      }
  }

void TFT_GPIO_init(void)        ////ST7789
  {
	SPI_SCK_1;			//特别注意！！	  
  SPI_RST_0;
	delay_ms(10);
	SPI_RST_1;
	delay_ms(120);

	TFT_SEND_CMD(0x01);//Software Reset
	delay_ms(120);

    TFT_SEND_CMD(0x11); 			//Sleep Out
	delay_ms(120);               //DELAY120ms
	 	  //-----------------------ST7789V Frame rate setting-----------------//
//************************************************
	TFT_SEND_CMD(0x3A);        //65k mode
	TFT_SEND_DATA(0x05);
	TFT_SEND_CMD(0xC5); 		//VCOM
	TFT_SEND_DATA(0x1A);
	TFT_SEND_CMD(0x36);                 // 屏幕显示方向设置
	TFT_SEND_DATA(0x00);
	//-------------ST7789V Frame rate setting-----------//
	TFT_SEND_CMD(0xb2);		//Porch Setting
	TFT_SEND_DATA(0x05);
	TFT_SEND_DATA(0x05);
	TFT_SEND_DATA(0x00);
	TFT_SEND_DATA(0x33);
	TFT_SEND_DATA(0x33);

	TFT_SEND_CMD(0xb7);			//Gate Control
	TFT_SEND_DATA(0x05);			//12.2v   -10.43v
	//--------------ST7789V Power setting---------------//
	TFT_SEND_CMD(0xBB);//VCOM
	TFT_SEND_DATA(0x3F);

	TFT_SEND_CMD(0xC0); //Power control
	TFT_SEND_DATA(0x2c);

	TFT_SEND_CMD(0xC2);		//VDV and VRH Command Enable
	TFT_SEND_DATA(0x01);

	TFT_SEND_CMD(0xC3);			//VRH Set
	TFT_SEND_DATA(0x0F);		//4.3+( vcom+vcom offset+vdv)

	TFT_SEND_CMD(0xC4);			//VDV Set
	TFT_SEND_DATA(0x20);				//0v

	TFT_SEND_CMD(0xC6);				//Frame Rate Control in Normal Mode
	TFT_SEND_DATA(0X01);			//111Hz

	TFT_SEND_CMD(0xd0);				//Power Control 1
	TFT_SEND_DATA(0xa4);
	TFT_SEND_DATA(0xa1);

	TFT_SEND_CMD(0xE8);				//Power Control 1
	TFT_SEND_DATA(0x03);

	TFT_SEND_CMD(0xE9);				//Equalize time control
	TFT_SEND_DATA(0x09);
	TFT_SEND_DATA(0x09);
	TFT_SEND_DATA(0x08);
	//---------------ST7789V gamma setting-------------//
	TFT_SEND_CMD(0xE0); //Set Gamma
	TFT_SEND_DATA(0xD0);
	TFT_SEND_DATA(0x05);
	TFT_SEND_DATA(0x09);
	TFT_SEND_DATA(0x09);
	TFT_SEND_DATA(0x08);
	TFT_SEND_DATA(0x14);
	TFT_SEND_DATA(0x28);
	TFT_SEND_DATA(0x33);
	TFT_SEND_DATA(0x3F);
	TFT_SEND_DATA(0x07);
	TFT_SEND_DATA(0x13);
	TFT_SEND_DATA(0x14);
	TFT_SEND_DATA(0x28);
	TFT_SEND_DATA(0x30);

	TFT_SEND_CMD(0XE1); //Set Gamma
	TFT_SEND_DATA(0xD0);
	TFT_SEND_DATA(0x05);
	TFT_SEND_DATA(0x09);
	TFT_SEND_DATA(0x09);
	TFT_SEND_DATA(0x08);
	TFT_SEND_DATA(0x03);
	TFT_SEND_DATA(0x24);
	TFT_SEND_DATA(0x32);
	TFT_SEND_DATA(0x32);
	TFT_SEND_DATA(0x3B);
	TFT_SEND_DATA(0x14);
	TFT_SEND_DATA(0x13);
	TFT_SEND_DATA(0x28);
	TFT_SEND_DATA(0x2F);

	TFT_SEND_CMD(0x20); 		//反显
	delay_ms(120); 
	TFT_SEND_CMD(0x29);         //开启显示
  }


void SYS_init1(unsigned char PLL)
{
    uint32_t temp_reg;
    
    // 参数检查
    if (PLL < 2 || PLL > 16) return;
    
    // 1. 保存外设时钟状态
    uint32_t apb1enr = RCC->APB1ENR;
    uint32_t apb2enr = RCC->APB2ENR;
    uint32_t ahbenr = RCC->AHBENR;
    
    // 2. 关闭所有中断
    __disable_irq();
    
    // 3. 如果PLL正在运行，先切换到HSI并关闭PLL
    if (RCC->CR & 0x01000000) {  // PLLON bit
        // 切换到HSI
        RCC->CFGR = (RCC->CFGR & ~0x03) | 0x00;  // SW = HSI
        while ((RCC->CFGR & 0x0C) != 0x00);      // 等待HSI就绪
        
        // 关闭PLL
        RCC->CR &= ~0x01000000;  // 关闭PLLON
        while (RCC->CR & 0x02000000);  // 等待PLLRDY清零
    }
    
    // 4. 使能HSE
    RCC->CR |= 0x00010000;  // HSEON
    while (!(RCC->CR & 0x00020000));  // 等待HSERDY
    
    // 5. 配置分频器
    temp_reg = RCC->CFGR;
    temp_reg &= ~(0x000000F0 | 0x00001C00 | 0x0000E000);  // 清除HPRE, PPRE1, PPRE2
    temp_reg |= 0x00000000;   // AHB不分频 (HPRE)
    temp_reg |= 0x00000000;   // APB2不分频 (PPRE2)
    temp_reg |= 0x00000400;   // APB1二分频 (PPRE1)
    RCC->CFGR = temp_reg;
    
    // 6. 配置PLL倍频
    temp_reg = RCC->CFGR;
    temp_reg &= ~(0x003C0000);           // 清除PLLMULL[3:0]
    temp_reg |= (PLL - 2) << 18;         // 设置PLL倍频值
    temp_reg |= 0x00010000;              // PLLSRC = 1 (使用HSE)
    RCC->CFGR = temp_reg;
    
    // 7. 使能PLL
    RCC->CR |= 0x01000000;  // PLLON
    while (!(RCC->CR & 0x02000000));  // 等待PLLRDY
    
    // 8. 切换到PLL
    RCC->CFGR = (RCC->CFGR & ~0x03) | 0x02;  // SW = PLL
    while ((RCC->CFGR & 0x0C) != 0x08);      // 等待PLL就绪
    
    // 9. 恢复外设时钟
    RCC->APB1ENR = apb1enr;
    RCC->APB2ENR = apb2enr;
    RCC->AHBENR = ahbenr;
    
    // 10. 更新系统时钟变量
    SystemCoreClock = HSE_VALUE * PLL;
    
    // 11. 重新初始化SysTick
    SysTick->CTRL = 0;
    SysTick->LOAD = (SystemCoreClock / 1000) - 1;
    SysTick->VAL = 0;
    SysTick->CTRL = 0x07;  // 使能SysTick中断
    
    // 12. 重新使能中断
    __enable_irq();
}


// 初始化函数实现
void TFT_Init() {
    //这里放入你自己的OLED初始化函数
	SYS_init1(5);
	IO_init();
	SPI_SCK_0;
	SPI_RST_0;
	delay_ms(300);
	SPI_RST_1;
	delay_ms(300);
	TFT_GPIO_init();
	//TFT_init();
}

// 构建篆布函数实现
void TFT_Setcanvas(uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey) {

   TFT_SEND_CMD(0x2a);    //Column address set
  TFT_SEND_DATA(sx>>8);    //start column
  TFT_SEND_DATA(sx);
  TFT_SEND_DATA(ex>>8);    //end column
  TFT_SEND_DATA(ex);

  TFT_SEND_CMD(0x2b);     //Row address set
  TFT_SEND_DATA(sy>>8);    //start row
  TFT_SEND_DATA(sy); 
  TFT_SEND_DATA(ey>>8);    //end row
  TFT_SEND_DATA(ey);
  TFT_SEND_CMD(0x2C);     //Memory write
}

void TFT_Setcolor(uint16_t color){
	TFT_SEND_DATA(color>>8);
  TFT_SEND_DATA(color);
}

#endif
