#ifndef KSCconfig_h
#define KSCconfig_h

//编译器系统配置
#define __USE_PC__ 1
#define __USE_ARMCC__ 0 
#define __USE_ESP32__ 0

//program config
#define __USE_LCD__ 1
#define USE_NOT_BUFFER 1
#define __USE_CHINESE__ 0
#define __USE_FLASH__ 0
#define __USE_UART__ 0
#define __USE_LITTLEFS__ 0
#define __USE_KEY__ 1

#define __LITTLE_END_COLOR__ 1

#define COLORBIT 2
#define COLORBYTE 2

#define TFTx 240
#define TFTy 160
#define MAX_INPUT_SIZE 255
#define _STATICBUF_SIZE 512

#define uintxy uint16_t
#define intxy int16_t
#define KSCCOLOR uint16_t

//data config
#define SYSTEMFONT 7
#define SYSTEMCHINESEFONT 16

#define SYSTEMCOLOR0 0xFFFF
#define SYSTEMCOLOR1 0x0000
#define SYSTEMCOLOR2 0x001F
#define SYSTEMCOLOR3 0xF800



#include <stdint.h>

#if __USE_PC__
#include <stdint.h>
// #include <graphics.h>
#include <stdlib.h>
#include <stdio.h>
// #include <unistd.h>
// #include <time.h>
// #include <windows.h>
#define K_malloc(size) malloc(size)
//#define KSCCOLOR uint32_t
extern uint8_t publicdatabuf[MAX_INPUT_SIZE];
#define log(...) 0
#define __USE_INPUT_KEY_SIMU__ 
#endif

#if __USE_ARMCC__

#include <stdlib.h>
#define K_malloc(size) malloc(size)
#define __USE_INPUT_KEY__ 
#endif

#if __USE_ESP32__
#include "esp_err.h"
#include "esp_log.h"

#define PPTAG "KSCdraw"
// 支持任意参数，和 ESP_LOGW 用法完全一样
#define log(...) ESP_LOGW(PPTAG, __VA_ARGS__)
#define co(color) (((color)&0xFF)<<8)|((color)&0xFF00)
#endif


#endif
