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
 * 你需要参考商家提供的例程驱动实现三个函数，分别是：
 * void TFT_Init(void)初始化TFT屏幕
 * void TFT_Setcanvas(uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey)设置画布位置
 * void TFT_Setcolor(uint16_t color)写入颜色
 * 引脚推荐定义(自查具体分布)：SCL/SCK:B5; SDA:B6; RST:B7; DC/AO:B8; CS:B9
 * 使用寄存器和HAL/标准库实现，在里面包含main.h/stm32f10x.h
 * 然后去掉驱动里面多余的其他东西包括其他头文件和颜色宏定义，只保留接口函数依赖.
 * 
 * 若标准库驱动中需要延时，建议使用下列的参考函数
 * 如果感觉速度慢，可以尝试使用下列加快时钟频率的函数，但需谨慎使用
 */
//////////////////////////////////////////

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

