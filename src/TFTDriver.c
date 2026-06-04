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
#include "../three_party/stm32/stm32f1xx_hal_spi.h"
#include "stm32f1xx_hal_dma.h"
#include "stm32f1xx_hal_gpio.h"

// --- 引脚定义 (PB13=SCK, PB15=MOSI; CS=PB12, DC=PB8, RST=PB9) ---
#define TFT_DC(x)   HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8,  x)
#define TFT_CS(x)   HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, x)
#define TFT_RST(x)  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9,  x)

#define TFT_SCK(x)  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, x)
#define TFT_MOSI(x) HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, x)

#define USE_HARDWARE_SPI 1

#if USE_HARDWARE_SPI

static SPI_HandleTypeDef hspi2;
DMA_HandleTypeDef hdma_spi2_tx;

volatile uint8_t dma_tc_flag = 0;

void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi)
{
    if (hspi->Instance == SPI2)
        dma_tc_flag = 1;
}

void my_Error_Handler(void);

void FastSystemClock(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL8;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    my_Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    my_Error_Handler();
  }
}

void spi_init(void)
{
    FastSystemClock();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_SPI2_CLK_ENABLE();
    __HAL_RCC_DMA1_CLK_ENABLE();

    GPIO_InitTypeDef gpio = {0};
    gpio.Mode  = GPIO_MODE_AF_PP;
    gpio.Pull  = GPIO_NOPULL;
    gpio.Speed = GPIO_SPEED_FREQ_HIGH;
    gpio.Pin   = GPIO_PIN_13 | GPIO_PIN_15;
    HAL_GPIO_Init(GPIOB, &gpio);

    gpio.Mode  = GPIO_MODE_OUTPUT_PP;
    gpio.Pin   = GPIO_PIN_12 | GPIO_PIN_8 | GPIO_PIN_9;
    HAL_GPIO_Init(GPIOB, &gpio);

    hdma_spi2_tx.Instance                 = DMA1_Channel5;
    hdma_spi2_tx.Init.Direction           = DMA_MEMORY_TO_PERIPH;
    hdma_spi2_tx.Init.PeriphInc           = DMA_PINC_DISABLE;
    hdma_spi2_tx.Init.MemInc              = DMA_MINC_ENABLE;
    hdma_spi2_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_spi2_tx.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
    hdma_spi2_tx.Init.Mode                = DMA_NORMAL;
    hdma_spi2_tx.Init.Priority            = DMA_PRIORITY_HIGH;
    HAL_DMA_Init(&hdma_spi2_tx);

    __HAL_LINKDMA(&hspi2, hdmatx, hdma_spi2_tx);

    HAL_NVIC_SetPriority(DMA1_Channel5_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(DMA1_Channel5_IRQn);

    hspi2.Instance               = SPI2;
    hspi2.Init.Mode              = SPI_MODE_MASTER;
    hspi2.Init.Direction         = SPI_DIRECTION_2LINES;
    hspi2.Init.DataSize          = SPI_DATASIZE_8BIT;
    hspi2.Init.CLKPolarity       = SPI_POLARITY_LOW;
    hspi2.Init.CLKPhase          = SPI_PHASE_1EDGE;
    hspi2.Init.NSS               = SPI_NSS_SOFT;
    hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
    hspi2.Init.FirstBit          = SPI_FIRSTBIT_MSB;
    hspi2.Init.TIMode            = SPI_TIMODE_DISABLE;
    hspi2.Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLE;
    hspi2.Init.CRCPolynomial     = 10;
    HAL_SPI_Init(&hspi2);

    TFT_CS(1);
    TFT_RST(1);
    TFT_DC(1);
}

void spi_transmit_byte(uint8_t byte)
{
    TFT_CS(0);
    HAL_SPI_Transmit(&hspi2, &byte, 1, HAL_MAX_DELAY);
    TFT_CS(1);
}

void spi_transmit(const uint8_t *data, uint16_t len)
{
    TFT_CS(0);
    HAL_SPI_Transmit(&hspi2, (uint8_t*)data, len, HAL_MAX_DELAY);
    TFT_CS(1);
}

void TFT_clear(uint16_t color)
{
    TFT_Setcanvas(0, 0, 239, 319);
    static uint16_t colorbuf[512];
    for (uint16_t i = 0; i < 512; i++)
        colorbuf[i] = color;

    TFT_CS(0);
    // for (uint32_t i = 0; i < 160; i++) {
    //     dma_tc_flag = 0;
    //     HAL_SPI_Transmit_DMA(&hspi2, (uint8_t*)colorbuf, 960);
    //     while (!dma_tc_flag);
    // }
    for (uint32_t i = 0; i < 160; i++) {
        spi_transmit((const uint8_t *)colorbuf, 960);
    }
    // TFT_Setcolors(colorbuf, 960);
    // for (uint32_t i = 0; i < 160; i++) {
    //     spi_transmit((const uint8_t *)colorbuf, 960);
    // }
    TFT_CS(1);
}

#else

void spi_init(void)
{
    __HAL_RCC_GPIOB_CLK_ENABLE();

    GPIO_InitTypeDef gpio = {0};
    gpio.Mode  = GPIO_MODE_OUTPUT_PP;
    gpio.Pull  = GPIO_NOPULL;
    gpio.Speed = GPIO_SPEED_FREQ_HIGH;
    gpio.Pin   = GPIO_PIN_13 | GPIO_PIN_15 | GPIO_PIN_12 | GPIO_PIN_8 | GPIO_PIN_9;
    HAL_GPIO_Init(GPIOB, &gpio);

    TFT_SCK(1);
    TFT_CS(1);
    TFT_RST(1);
    TFT_DC(1);
}

void spi_transmit_byte(uint8_t byte)
{
    TFT_CS(0);
    for (uint8_t i = 0; i < 8; i++)
    {
        TFT_SCK(0);
        TFT_MOSI(byte & 0x80);
        byte <<= 1;
        TFT_SCK(1);
    }
    TFT_CS(1);
    TFT_SCK(0);
}

void spi_transmit(const uint8_t *data, uint16_t len)
{
    for (uint16_t i = 0; i < len; i++)
        spi_transmit_byte(data[i]);
}

void TFT_clear(uint16_t color)
{
    TFT_Setcanvas(0, 0, 239, 319);
    static uint16_t colorbuf[512];
    for (uint16_t i = 0; i < 512; i++)
        colorbuf[i] = color;

    TFT_CS(0);
    for (uint32_t i = 0; i < 160; i++) {
        spi_transmit((const uint8_t *)colorbuf, 960);
    }
    TFT_CS(1);
}

#endif

// --- TFT 驱动 ---
typedef struct {
    uint8_t command;
    uint8_t data[16];
    uint8_t data_length;
} lcdcommand;

void TFT_SendCommand(lcdcommand cmd)
{
    TFT_DC(0);
    spi_transmit_byte(cmd.command);
    TFT_DC(1);
    for (uint8_t i = 0; i < cmd.data_length; i++)
        spi_transmit_byte(cmd.data[i]);
}

void delay_ms(uint32_t ms)
{
    HAL_Delay(ms);
}
#ifdef __USE_ST7789__
const lcdcommand init_cmds[] = {
    {0x00, {0}, 0},
    {0x01, {0}, 0},
    {0x00, {0}, 0},
    {0x11, {0}, 0},
    {0x00, {0}, 0},
    {0x3A, {0x05}, 1},
    {0xC5, {0x1A}, 1},
    {0x36, {0x00}, 1},
    {0xB2, {0x05, 0x05, 0x00, 0x33, 0x33}, 5},
    {0xB7, {0x05}, 1},
    {0xBB, {0x3F}, 1},
    {0xC0, {0x2C}, 1},
    {0xC2, {0x01}, 1},
    {0xC3, {0x0F}, 1},
    {0xC4, {0x20}, 1},
    {0xC6, {0x01}, 1},
    {0xD0, {0xA4, 0xA1}, 2},
    {0xE8, {0x03}, 1},
    {0xE9, {0x09, 0x09, 0x08}, 3},
    {0xE0, {0xD0, 0x05, 0x09, 0x09, 0x08, 0x14, 0x28, 0x33, 0x3F, 0x07, 0x13, 0x14, 0x28, 0x30}, 14},
    {0xE1, {0xD0, 0x05, 0x09, 0x09, 0x08, 0x03, 0x24, 0x32, 0x32, 0x3B, 0x14, 0x13, 0x28, 0x2F}, 14},
    {0x20, {0}, 0},
    {0x00, {0}, 0},
    {0x29, {0}, 0}
};
#elif __USE_ST7735__
const lcdcommand init_cmds[] = {
    {0x00, {0}, 0},
    {0x11, {0}, 0},
    {0x00, {0}, 0},
    {0x21, {0}, 0},
    {0x21, {0}, 0},
    {0xB1, {0x05,0x3A,0x3A}, 3},
    {0xB2, {0x05,0x3A,0x3A}, 3},
    {0xB3, {0x05,0x3A,0x3A,0x05,0x3A,0x3A}, 6},
    {0xB4, {0x03}, 1},
    {0xC0, {0x62,0x02,0x04}, 3},
    {0xC1, {0xC0}, 1},
    {0xC2, {0x0D,0x00}, 2},
    {0xC3, {0x8D,0x6A}, 2},
    {0xC4, {0x8D,0xEE}, 2},
    {0xC5, {0x0E}, 1},
    {0xE0, {0x10, 0x0E, 0x02, 0x03, 0x0E, 0x07, 0x02, 0x07, 0x0A, 0x12, 0x27, 0x37, 0x00, 0x0D, 0x0E, 0x10}, 16},
    {0xE1, {0x10, 0x0E, 0x03, 0x03, 0x0F, 0x06, 0x02, 0x08, 0x0A, 0x13, 0x26, 0x36, 0x00, 0x0D, 0x0E, 0x10}, 16},
    {0x3A, {0x05}, 1},
    {0x36, {0x68}, 1},
    {0x29, {0}, 0}
};
#endif


void TFT_Setcanvas(uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey)
{
    lcdcommand cmd_col[3] = {
        {0x2A, {sx >> 8, sx & 0xFF, ex >> 8, ex & 0xFF}, 4},
        {0x2B, {sy >> 8, sy & 0xFF, ey >> 8, ey & 0xFF}, 4},
        {0x2C, {0}, 0}
    };
    for (size_t i = 0; i < 3; i++) {
        if (cmd_col[i].command != 0)
            TFT_SendCommand(cmd_col[i]);
        else
            delay_ms(120);
    }
}
void TFT_Setcolors(const uint16_t *colorbuf, uint16_t num)
{
    spi_transmit((const uint8_t *)colorbuf, num * 2);
}





void TFT_Setcolor(uint16_t color)
{
    spi_transmit_byte(color >> 8);
    spi_transmit_byte(color & 0xFF);
}



void TFT_Init(void)
{
    spi_init();
    TFT_RST(0);
    delay_ms(100);
    TFT_RST(1);
    delay_ms(300);
    for (size_t i = 0; i < sizeof(init_cmds) / sizeof(init_cmds[0]); i++) {
        if (init_cmds[i].command != 0)
            TFT_SendCommand(init_cmds[i]);
        else
            delay_ms(120);
    }
    TFT_clear(0x0000);
    
    // HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
}

void my_Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#endif
