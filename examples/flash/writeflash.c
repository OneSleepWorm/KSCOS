//--locale=english --no-multibyte-chars
#include "Serial.h"
#include <stdint.h>
#include <string.h>
extern UART_HandleTypeDef huart1;
#include "KSCbasicdrawN.h"
#include "CLI.h"
#include "W25Q64.h"
extern SPI_HandleTypeDef hspi1;

//stm32f1xx_it.c
#include "KSCconfig.h"

int main(void)
{
  KSC_buf* screen=kinitscreen(240,320,NULL,0,0);
  flash_init();
  kconnect_init();
  kreceive_init();
  kstring(NULL,"Hello world!",10,10);
  kstringchinese(screen,"春眠不觉晓",10,30);
  kstringchinese(screen,"处处闻啼鸟",10,50);
  kstringchinese(screen,"夜来风雨声",10,70);
  kstringchinese(screen,"花落知多少",10,90);

  uint32_t time=0;
  while (1)
  {
    HAL_Delay(1);
    run_cli_root(kfgetc());

  }
  /* USER CODE END 3 */
}