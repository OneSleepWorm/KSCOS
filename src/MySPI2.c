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
