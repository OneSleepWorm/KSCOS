#include "Serial.h"
#include "string.h"
#include "stdarg.h"
#include "stdio.h"
#include "stm32f1xx_hal.h"
#include "stdint.h"
#include "stdlib.h"

extern UART_HandleTypeDef huart1;

uint8_t* connect_write=0;
uint8_t* connect_read=0;
extern uint8_t  connect_publicdata[CONNECT_BUFFER_SIZE];

// 接收标志位,用于判断是否有数据可读,0:无数据,1:有数据
static uint8_t connect_flag=0;


void serial_init(void)
{
  /* USER CODE BEGIN USART1_Init 0 */
  
  // 手动配置引脚（临时测试）
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

#ifndef __USE_CONNECT_WRITE_BUFFER__
void Serial_sendbyte(uint8_t Byte)
{
    HAL_UART_Transmit(&huart1, &Byte, 1, HAL_MAX_DELAY); // 阻塞发送
}
void Serial_sendstring(char *str)
{
    while (*str)
    {serial_sendbyte(*str++);}
}
#endif

#if __USE_CONNECT_WRITE_BUFFER__ > 0
uint8_t connect_private_writebuffer[CONNECT_BUFFER_SIZE];
void serial_sendbyte(uint8_t Byte)
{
    HAL_UART_Transmit_IT(&huart1, &Byte, 1);
}
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

void kprintf(char *format, ...)
{
    char String[100];
    va_list arg;
    va_start(arg, format);
    vsprintf(String, format, arg);
    va_end(arg);
    serial_sendstring(String);
}

void serial_receive_init(void) {
    // 重置指针
    connect_write = connect_publicdata;
    connect_read = connect_publicdata;
    // 清空缓冲区
    memset(connect_publicdata, 0, CONNECT_BUFFER_SIZE);
    connect_flag=0;
    // 启动第一次接收
    HAL_StatusTypeDef status = HAL_UART_Receive_IT(&huart1, connect_write, 1);
    if (status != HAL_OK) {
        Error_Handler();
    }
}

void serial_receive_deinit(void){
    HAL_UART_Receive_IT(&huart1, NULL, 0);  // 传入NULL和0会禁用中断
}

uint8_t serial_getbyte(void){
		static uint8_t te =0;
        if(connect_flag){
            te=*connect_read;
        }else {
			return 0;
        }
        connect_read++;
        if(connect_read>=connect_publicdata+CONNECT_BUFFER_SIZE){
            connect_read=connect_publicdata;//回绕到开头
        }
        if(connect_read==connect_write){
            connect_flag=0;
        }
        return te;
}
uint8_t serial_getflag(void){
    return connect_flag;
}

char* serial_fgetc(void){
    static char str[32]={0};
    char* p=str;
	HAL_Delay(200);
    while(serial_getflag()&&p-str<31){
        *p=serial_getbyte();
        p++;
    }
    *p='\0';
    return str;
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart == &huart1) {
        connect_write++;
        if (connect_write >= connect_publicdata + CONNECT_BUFFER_SIZE) {
            connect_write = connect_publicdata; 
        }
        connect_flag=1;
        HAL_UART_Receive_IT(huart, connect_write, 1);
    }
}
