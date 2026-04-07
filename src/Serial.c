#include "../inc/Serial.h"
#include "string.h"
#include "stdarg.h"
#include "stdio.h"
#include "stm32f1xx_hal.h"
#include "stdint.h"
#include "stdlib.h"

extern UART_HandleTypeDef huart1;

uint32_t connect_write_num=0;
uint32_t connect_read_num=0;
extern uint8_t  connect_publicdata[CONNECT_BUFFER_SIZE];
//uint8_t  connect_buf_num = 0;

// 接收标志位,用于判断是否有数据可读,0:无数据,1:有数据
uint8_t connect_flag=0;


void serial_init(void)
{
  /* USER CODE BEGIN USART1_Init 0 */
  
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_USART1_CLK_ENABLE();
  
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  GPIO_InitStruct.Pin = GPIO_PIN_9 ;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  GPIO_InitStruct.Pin = GPIO_PIN_10;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  /* USER CODE END USART1_Init 0 */

  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */
  HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(USART1_IRQn);
  /* USER CODE END USART1_Init 2 */
}

#if __USE_CONNECT_WRITE_BUFFER__ <= 0
/**
 * @brief 发送字节
 * 
 * @param Byte 要发送的字节
 */
void serial_sendbyte(uint8_t Byte)
{
    HAL_UART_Transmit(&huart1, &Byte, 1, HAL_MAX_DELAY); // 阻塞发送
}
/**
 * @brief 发送字符串
 * 
 * @param str 要发送的字符串
 */
void serial_sendstring(char *str)
{
    HAL_UART_Transmit(&huart1, (const uint8_t*)str,strlen(str) , HAL_MAX_DELAY);
}

void serial_sendarray(uint8_t *array, uint16_t size)
{
    HAL_UART_Transmit(&huart1, array, size, HAL_MAX_DELAY);
}

#endif

#if __USE_CONNECT_WRITE_BUFFER__ > 0
static uint8_t connect_private_writebuffer[__USE_CONNECT_WRITE_BUFFER__];
/**
 * @brief 发送字节
 * 
 * @param Byte 要发送的字节
 */
void serial_sendbyte(uint8_t Byte)
{
    HAL_UART_Transmit_IT(&huart1, &Byte, 1);
}
/**
 * @brief 发送字符串
 * 
 * @param str 要发送的字符串
 */
void serial_sendstring(char *str)
{
    if(strlen(str)>=CONNECT_BUFFER_SIZE){
        log("str too long");
        return;
    }
    memcpy(connect_private_writebuffer,str,strlen(str));
    HAL_UART_Transmit_IT(&huart1, connect_private_writebuffer, strlen(str));
}
#endif

#if __USE_PRINTF__ > 0
int fputc(int ch, FILE *f)
{
    serial_sendbyte((uint8_t)ch);
    return ch;
}
#endif

/**
 * @brief 发送格式化字符串
 * 
 * @param format 格式化字符串
 * @param ... 格式化字符串的参数
 */
void kprintf(char *format, ...)
{
    if(!format)return;
    char String[257];
    va_list arg;
    va_start(arg, format);
    vsprintf(String, format, arg);
    va_end(arg);
    serial_sendstring(String);
}

/**
 * @brief 初始化串口接收
 * 
 */
void serial_receive_init(void) {
    // 重置计数
    connect_write_num = 0;
    connect_read_num = 0;
    connect_flag=0;
    
    // HAL_UART_Receive_IT(&huart1, connect_publicdata, 1);
    HAL_UART_Receive_DMA(&huart1, connect_publicdata, CONNECT_BUFFER_SIZE);
    __HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);
}

/**
 * @brief 反初始化串口接收
 * 
 */
void serial_receive_deinit(void){
    //HAL_UART_Receive_IT(&huart1, NULL, 0);  // 传入NULL和0会禁用中断
    HAL_UART_Receive_DMA(&huart1, NULL,0);
    //__HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);
}
/**
 * @brief 获取字节
 * 
 * @return uint8_t 字节
 */
uint8_t serial_getbyte(void){
		static uint8_t te =0;
        if(connect_read_num<connect_write_num){
            te = connect_publicdata[connect_read_num++];
            connect_flag=1;
        }else{
            connect_flag=0;
        }
        return te;
}
/**
 * @brief 获取接收标志位
 * 
 * @return uint8_t 接收标志位
 */
uint8_t serial_getflag(void){
    return connect_flag;
}
/**
 * @brief 获取字符串
 * @warning 该函数返回的字符串指针指向的是接收缓冲区，调用者需要尽快处理数据并清空缓冲区，否则可能会被新的接收数据覆盖。
 * @return char* 字符串
 */
char* serial_fgetc(void){
    //serial_receive_init();
    //kprintf("flag:%d", serial_getflag());
	if(!serial_getflag())return NULL;
    connect_flag=0;
    *(connect_publicdata+(connect_write_num))='\0';
    //kprintf("%d",connect_write_num);
    kreceive_init();
    return (char*)connect_publicdata;
}

uint8_t* kfgetc_s(uint16_t size){
    if(!serial_getflag())return NULL;
    connect_flag=0;
    //if(size==connect_write_num){
    connect_write_num = 0;
    connect_read_num = 0;
    return connect_publicdata;
    //}
    //return NULL;
}

/**
 * @brief 接收完成回调函数
 * 
 * @param huart 串口句柄
 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
        //__HAL_UART_CLEAR_IDLEFLAG(&huart1);
        // 2. 停止DMA传输，防止在计算长度时还有新数据进来
        //HAL_UART_DMAStop(&huart1);
        //ledturn();
}

void USART1_IRQHandler(void)
{
  /* USER CODE BEGIN USART1_IRQn 0 */
  //ledturn();
  testnum++;
  connect_flag=1;
    __HAL_UART_CLEAR_IDLEFLAG(&huart1);
    HAL_UART_DMAStop(&huart1);
  connect_write_num = CONNECT_BUFFER_SIZE - __HAL_DMA_GET_COUNTER(huart1.hdmarx);
  HAL_UART_Receive_DMA(&huart1, connect_publicdata, CONNECT_BUFFER_SIZE);
    __HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);
  /* USER CODE END USART1_IRQn 0 */
  HAL_UART_IRQHandler(&huart1);
  /* USER CODE BEGIN USART1_IRQn 1 */

  /* USER CODE END USART1_IRQn 1 */
}
