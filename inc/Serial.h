#ifndef __SERIAL_H
#define __SERIAL_H

//#include "stdio.h"
//#include "stm32f1xx_hal.h"
#include "stdint.h"
#include "KSCconfig.h"


void serial_init(void);
void Error_Handler(void);
void serial_sendbyte(uint8_t Byte);
void serial_sendstring(char *str);
void serial_receive_init(void);
void serial_receive_deinit(void);
uint8_t serial_getbyte(void);
char* serial_fgetc(void);
void kprintf(char *format, ...);


#endif
