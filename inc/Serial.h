#ifndef __SERIAL_H
#define __SERIAL_H

//#include "stdio.h"
#include "main.h"
#include "stdint.h"
#include "KSCconfig.h"

extern uint8_t connect_flag;
extern uint32_t connect_write_num;
extern uint32_t connect_read_num;

void serial_init(void);
void Error_Handler(void);
void serial_sendbyte(uint8_t Byte);
void serial_sendstring(char *str);
void serial_receive_init(void);
void serial_receive_deinit(void);
uint8_t serial_getbyte(void);
char* serial_fgetc(void);
void kprintf(char *format, ...);
uint8_t* kfgetc_s(uint16_t size);


#define kconnect_init serial_init
#define kreceive_init serial_receive_init
#define kreceive_deinit serial_receive_deinit
#define ksendbyte serial_sendbyte
#define ksendstring serial_sendstring
#define kgetbyte serial_getbyte
#define kfgetc serial_fgetc

// #define kfgetc_s(pdata) do{HAL_Delay(1);(pdata) = kfgetc();}while((pdata) == NULL)

#endif
