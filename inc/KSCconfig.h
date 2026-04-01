#ifndef KSCconfig_h
#define KSCconfig_h

//编译器系统配置
//#define __USE_GCC__ 
#define __USE_ARMCC__ 

//program config
#define __USE_CHINESE__ 0
#define __USE_FLASH__ 0
#define __USE_UART__ 1
#define __USE_PRINTF__ 1

//buffer config
#define TFTx 240
#define TFTy 320

//small config 
//#define __USE_CLEAR_SCRREN__ 


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

//onlywrite config
#define COLORBYTE 2
#define CONNECT_BUFFER_SIZE 256
#define uintxy uint16_t
#define intxy int16_t
extern uint8_t connect_publicdata[CONNECT_BUFFER_SIZE];
extern uint8_t testnum;
void ledturn(void);
//#define log(str) kprintf(str)



#endif
