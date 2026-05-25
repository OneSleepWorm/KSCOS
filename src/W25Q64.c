#include "../inc/W25Q64.h"

#if __USE_PC__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define W25Q64_TOTAL_SIZE   (8 * 1024 * 1024)  // 8MB
#define W25Q64_SECTOR_SIZE  (4 * 1024)         // 4KB 扇区
#define W25Q64_PAGE_SIZE    (256)              // 256 字节页

static FILE* g_flash_file = NULL;
static const char* FLASH_FILE_PATH = "E:\\CProject\\KSCdraw\\KSCdraw\\.data\\w25q64_sim.bin";

void W25Q64_Init(void)
{
    g_flash_file = fopen(FLASH_FILE_PATH, "rb+");
    if (g_flash_file == NULL) {
        g_flash_file = fopen(FLASH_FILE_PATH, "wb");
        if (g_flash_file != NULL) {
            uint8_t* buffer = (uint8_t*)malloc(W25Q64_TOTAL_SIZE);
            memset(buffer, 0xFF, W25Q64_TOTAL_SIZE);
            fwrite(buffer, 1, W25Q64_TOTAL_SIZE, g_flash_file);
            free(buffer);
            fclose(g_flash_file);
            g_flash_file = fopen(FLASH_FILE_PATH, "rb+");
        }
    }
}

void W25Q64_DeInit(void)
{
    if (g_flash_file) {
        fclose(g_flash_file);
        g_flash_file = NULL;
    }
}

void W25Q64_PageProgram(uint32_t Address, uint8_t *DataArray, uint16_t Count)
{
    // 读取原有数据
    uint8_t* old = (uint8_t*)malloc(Count);
    fseek(g_flash_file, Address, SEEK_SET);
    fread(old, 1, Count, g_flash_file);
    
    // 计算实际要写入的数据：原有数据 & 新数据（只能将1变0，不能将0变1）
    uint8_t* real_data = (uint8_t*)malloc(Count);
    for (uint16_t i = 0; i < Count; i++) {
        real_data[i] = old[i] & DataArray[i];
    }
    free(old);
    
    // 写入计算结果
    fseek(g_flash_file, Address, SEEK_SET);
    fwrite(real_data, 1, Count, g_flash_file);
    fflush(g_flash_file);
    free(real_data);
}

void W25Q64_SectorErase(uint32_t Address)
{
    // 对齐到扇区起始地址（虽然用户没要求判断，但擦除必须是4KB对齐，按硬件行为自动对齐）
    uint32_t sector_start = Address & ~(W25Q64_SECTOR_SIZE - 1);
    uint8_t* erase_buf = (uint8_t*)malloc(W25Q64_SECTOR_SIZE);
    memset(erase_buf, 0xFF, W25Q64_SECTOR_SIZE);
    fseek(g_flash_file, sector_start, SEEK_SET);
    fwrite(erase_buf, 1, W25Q64_SECTOR_SIZE, g_flash_file);
    fflush(g_flash_file);
    free(erase_buf);
}

void W25Q64_ReadData(uint32_t Address, uint8_t *DataArray, uint32_t Count)
{
    fseek(g_flash_file, Address, SEEK_SET);
    fread(DataArray, 1, Count, g_flash_file);
}
#endif

#ifdef __cplusplus
}
#endif

#if __USE_ARMCC__

#include "main.h"                  // Device header
#include "W25Q64_Ins.h"
#include "main.h"

/* ----------------------- 引脚配置 ----------------------- */
// 请根据实际硬件连接修改以下宏定义
#define MY_SPI_SS_LOW()     HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET)
#define MY_SPI_SS_HIGH()    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET)

/* ----------------------- 函数声明 ----------------------- */
void MySPI_Init(void);
void MySPI_Start(void);
void MySPI_Stop(void);
uint8_t MySPI_SwapByte(uint8_t ByteSend);

/* ----------------------- SPI 句柄定义 ----------------------- */
// 必须在 main.c 或其他初始化代码中调用 HAL_SPI_Init 后才能使用
extern SPI_HandleTypeDef hspi1;

/* ----------------------- GPIO 和 SPI 初始化 ----------------------- */

/**
  * @brief  SPI初始化（使用HAL库）
  * @param  无
  * @retval 无
  */
void MySPI_Init(void)
{
    __HAL_RCC_GPIOA_CLK_ENABLE(); // 使能GPIOA时钟
    __HAL_RCC_SPI1_CLK_ENABLE();  // 使能SPI1时钟

    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* 配置 SS (PA4) - 推挽输出 */
    GPIO_InitStruct.Pin = GPIO_PIN_0;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    if (HAL_SPI_Init(&hspi1) != HAL_OK)
    {
        // 初始化错误处理（可选：进入错误状态）
        //Error_Handler();
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_RESET);
    }

    /* 设置SS默认电平 */
    MY_SPI_SS_HIGH();
}

/* ----------------------- SPI 时序控制 ----------------------- */

/**
  * @brief  SPI起始（拉低SS）
  * @param  无
  * @retval 无
  */
void MySPI_Start(void)
{
    MY_SPI_SS_LOW();  // 拉低SS，开始通信
}

/**
  * @brief  SPI终止（拉高SS）
  * @param  无
  * @retval 无
  */
void MySPI_Stop(void)
{
    MY_SPI_SS_HIGH(); // 拉高SS，结束通信
}

/* ----------------------- 数据传输函数 ----------------------- */

/**
  * @brief  SPI交换一个字节（全双工）
  * @param  ByteSend 要发送的字节
  * @retval 接收到的字节
  */
uint8_t MySPI_SwapByte(uint8_t ByteSend)
{
    uint8_t ByteReceive = 0;

    // 使用阻塞方式发送/接收一个字节
    if (HAL_SPI_TransmitReceive(&hspi1, &ByteSend, &ByteReceive, 1, HAL_MAX_DELAY) != HAL_OK)
    {
        // 传输错误处理
        return 0xFF; // 或者调用 Error_Handler()
    }

    return ByteReceive;
}
/**
  * 函    数：W25Q64初始化
  * 参    数：无
  * 返 回 值：无
  */
void W25Q64_Init(void)
{
	MySPI_Init();					//先初始化底层的SPI
}
void W25Q64_DeInit(void)
{
	MySPI_Stop();				//先反初始化底层的SPI
}
/**
  * 函    数：W25Q64读取ID号
  * 参    数：MID 工厂ID，使用输出参数的形式返回
  * 参    数：DID 设备ID，使用输出参数的形式返回
  * 返 回 值：无
  */
void W25Q64_ReadID(uint8_t *MID, uint16_t *DID)
{
	MySPI_Start();								//SPI起始
	MySPI_SwapByte(W25Q64_JEDEC_ID);			//交换发送读取ID的指令
	*MID = MySPI_SwapByte(W25Q64_DUMMY_BYTE);	//交换接收MID，通过输出参数返回
	*DID = MySPI_SwapByte(W25Q64_DUMMY_BYTE);	//交换接收DID高8位
	*DID <<= 8;									//高8位移到高位
	*DID |= MySPI_SwapByte(W25Q64_DUMMY_BYTE);	//或上交换接收DID的低8位，通过输出参数返回
	MySPI_Stop();								//SPI终止
}

/**
  * 函    数：W25Q64写使能
  * 参    数：无
  * 返 回 值：无
  */
void W25Q64_WriteEnable(void)
{
	MySPI_Start();								//SPI起始
	MySPI_SwapByte(W25Q64_WRITE_ENABLE);		//交换发送写使能的指令
	MySPI_Stop();								//SPI终止
}

/**
  * 函    数：W25Q64等待忙
  * 参    数：无
  * 返 回 值：无
  */
void W25Q64_WaitBusy(void)
{
	uint32_t Timeout;
	MySPI_Start();								//SPI起始
	MySPI_SwapByte(W25Q64_READ_STATUS_REGISTER_1);				//交换发送读状态寄存器1的指令
	Timeout = 100000;							//给定超时计数时间
	while ((MySPI_SwapByte(W25Q64_DUMMY_BYTE) & 0x01) == 0x01)	//循环等待忙标志位
	{
		Timeout --;								//等待时，计数值自减
		if (Timeout == 0)						//自减到0后，等待超时
		{
			/*超时的错误处理代码，可以添加到此处*/
			break;								//跳出等待，不等了
		}
	}
	MySPI_Stop();								//SPI终止
}

/**
  * 函    数：W25Q64页编程
  * 参    数：Address 页编程的起始地址，范围：0x000000~0x7FFFFF
  * 参    数：DataArray	用于写入数据的数组
  * 参    数：Count 要写入数据的数量，范围：0~256
  * 返 回 值：无
  * 注意事项：写入的地址范围不能跨页
  */
void W25Q64_PageProgram(uint32_t Address, uint8_t *DataArray, uint16_t Count)
{
	uint16_t i;
	
	W25Q64_WriteEnable();						//写使能
	
	MySPI_Start();								//SPI起始
	MySPI_SwapByte(W25Q64_PAGE_PROGRAM);		//交换发送页编程的指令
	MySPI_SwapByte(Address >> 16);				//交换发送地址23~16位
	MySPI_SwapByte(Address >> 8);				//交换发送地址15~8位
	MySPI_SwapByte(Address);					//交换发送地址7~0位
	for (i = 0; i < Count; i ++)				//循环Count次
	{
		MySPI_SwapByte(*(DataArray+i));			//依次在起始地址后写入数据
	}
	MySPI_Stop();								//SPI终止
	
	W25Q64_WaitBusy();							//等待忙
}

void W25Q64_PageProgram_NeedNum(uint32_t Address, uint8_t *DataArray, uint16_t Count,uint16_t Start_Count)
{
	uint16_t i;
	
	W25Q64_WriteEnable();						//写使能
	
	MySPI_Start();								//SPI起始
	MySPI_SwapByte(W25Q64_PAGE_PROGRAM);		//交换发送页编程的指令
	MySPI_SwapByte(Address >> 16);				//交换发送地址23~16位
	MySPI_SwapByte(Address >> 8);				//交换发送地址15~8位
	MySPI_SwapByte(Address);					//交换发送地址7~0位
	for (i = 0; i < Count; i ++)				//循环Count次
	{
		MySPI_SwapByte(DataArray[i+Start_Count]);			//依次在起始地址后写入数据
	}
	MySPI_Stop();								//SPI终止
	
	W25Q64_WaitBusy();							//等待忙
}

/**
  * 函    数：W25Q64扇区擦除（4KB）
  * 参    数：Address 指定扇区的地址，范围：0x000000~0x7FFFFF
  * 返 回 值：无
  */
void W25Q64_SectorErase(uint32_t Address)
{
	W25Q64_WriteEnable();						//写使能
	
	MySPI_Start();								//SPI起始
	MySPI_SwapByte(W25Q64_SECTOR_ERASE_4KB);	//交换发送扇区擦除的指令
	MySPI_SwapByte(Address >> 16);				//交换发送地址23~16位
	MySPI_SwapByte(Address >> 8);				//交换发送地址15~8位
	MySPI_SwapByte(Address);					//交换发送地址7~0位
	MySPI_Stop();								//SPI终止
	
	W25Q64_WaitBusy();							//等待忙
}

/**
  * 函    数：W25Q64读取数据
  * 参    数：Address 读取数据的起始地址，范围：0x000000~0x7FFFFF
  * 参    数：DataArray 用于接收读取数据的数组，通过输出参数返回
  * 参    数：Count 要读取数据的数量，范围：0~0x800000
  * 返 回 值：无
  */
void W25Q64_ReadData(uint32_t Address, uint8_t *DataArray, uint32_t Count)
{
	uint32_t i;
	MySPI_Start();								//SPI起始
	MySPI_SwapByte(W25Q64_READ_DATA);			//交换发送读取数据的指令
	MySPI_SwapByte(Address >> 16);				//交换发送地址23~16位
	MySPI_SwapByte(Address >> 8);				//交换发送地址15~8位
	MySPI_SwapByte(Address);					//交换发送地址7~0位
	for (i = 0; i < Count; i ++)				//循环Count次
	{
		*(DataArray+i) = MySPI_SwapByte(W25Q64_DUMMY_BYTE);	//依次在起始地址后读取数据
	}
	MySPI_Stop();								//SPI终止
}
#endif

