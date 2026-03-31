#include "../inc/KSCconfig.h"
#include "main.h"

uint8_t  connect_publicdata[CONNECT_BUFFER_SIZE];
uint8_t  publicdatabuf[CONNECT_BUFFER_SIZE];
uint8_t  testnum =0;

void ledturn(void){
	if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) == GPIO_PIN_RESET){
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
	}
	else{
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
	}
}


