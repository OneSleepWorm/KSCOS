#ifndef __OSCONNECT_H__
#define __OSCONNECT_H__

#include <stdint.h>
#include "KSCconfig.h"
#include "Serial.h"

#ifdef __USE_UART__
void kconnect_init(void);
void kconnect_deinit(void);
void kconnect_sendbyte(uint8_t Byte);
void kconnect_sendarray(uint8_t *Array, uint16_t Length);
void kconnect_sendstring(char *String);
// 防止意外展开问题
void kconnect_receive_init(void);
void kconnect_receive_deinit(void);
void kprintf(char *format, ...);
uint8_t kconnect_getbyte(void);
char* kfgetc(void);

#endif

#endif
