#ifndef KSCconfig_h
#define KSCconfig_h

//编译器系统配置
//#define __USE_GCC__ 
#define __USE_ARMCC__ 

//program config
#define USE_NOT_BUFFER 1
#define __USE_CHINESE__ 0
#define __USE_FLASH__ 0
#define __USE_UART__ 0


#define COLORBIT 2
#define COLORBYTE 2

#define TFTx 128
#define TFTy 64
#define MAX_INPUT_SIZE 255

#define uintxy uint16_t
#define intxy int16_t

//data config
#define SYSTEMFONT 8
#define SYSTEMCHINESEFONT 16

#define SYSTEMCOLOR0 BLACK
#define SYSTEMCOLOR1 BLUE
#define SYSTEMCOLOR2 GREEN
#define SYSTEMCOLOR3 RED



#include <stdint.h>

#ifdef __USE_GCC__
#include <stdint.h>
#include <graphics.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <windows.h>
#define K_malloc(size) malloc(size)
#define KSCCOLOR uint32_t
extern uint8_t publicdatabuf[MAX_INPUT_SIZE];

#endif

#ifdef __USE_ARMCC__


#include <stdlib.h>
#define K_malloc(size) malloc(size)
#define KSCCOLOR uint16_t
#endif

#endif

#if __USE_FLASH__ > 0
extern uint8_t publicdatabuf[MAX_INPUT_SIZE];

#endif
