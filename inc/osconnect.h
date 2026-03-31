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
#define kconnect_receive_init()    do { serial_receive_init(); } while(0)
#define kconnect_receive_deinit()  do { serial_receive_deinit(); } while(0)
void kprintf(char *format, ...);
uint8_t kconnect_getbyte(void);
#define kfgetc() serial_fgetc()

#endif

#endif
