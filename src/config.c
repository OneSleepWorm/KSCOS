#include "../inc/KSCconfig.h"

#ifndef  __USE_PC__
#include "main.h"
uint8_t  connect_publicdata[CONNECT_BUFFER_SIZE]={0};
uint8_t  publicdatabuf[FLASH_BUFFER_SIZE]={0};
uint8_t  testnum =0;

void ledturn(void){
	if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) == GPIO_PIN_RESET){
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
	}
	else{
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
	}
}
#endif

