#ifndef __KSCOSSYSTEM_H__
#define __KSCOSSYSTEM_H__

#include <stdint.h>
#include "KSCconfig.h"

#define KSCOS_LOW_CLOCK 0
#define KSCOS_NORMAL_CLOCK 1
#define KSCOS_HIGH_CLOCK 2

extern __volatile uint32_t KSCOSsystem_Clock;
void KSCOSSystem_Init(void);
void KSCOSSystemClock_Init(uint8_t clock_type);
void KSCOS_Error_Handler(void);
ki8 KSCOS_default_Error_Handler(void* data);
void sysdelay(uint32_t ms);

void* osmalloc(size_t size);
void osfree(void* ptr);
void* oscalloc(size_t num, size_t size);
void* osrealloc(void* ptr, size_t size);
void* osmemmove(void* dst, const void* src, size_t len);

uint32_t sysgettime(void);


#endif
