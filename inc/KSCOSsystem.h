#ifndef __KSCOSSYSTEM_H__
#define __KSCOSSYSTEM_H__

#include <stdint.h>
#include "KSCconfig.h"

#define KSCOS_LOW_CLOCK 0
#define KSCOS_NORMAL_CLOCK 1
#define KSCOS_HIGH_CLOCK 2

extern __volatile uint32_t KSCOSsystem_Clock;
void KSCOSsystem_Init(void);
void KSCOSSystemClock_Init(uint8_t clock_type);
void KSCOS_Error_Handler(void);
ki8 KSCOS_default_Error_Handler(void* data);
void sysdelay(uint32_t ms);

#endif
