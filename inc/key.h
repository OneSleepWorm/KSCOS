#ifndef __KEY_H
#define __KEY_H
#include "KSCconfig.h"

#if __USE_KEY__
#include "input.h"
#define KEY_DEVICE_ID (1+(1<<8))

#define KEY_NONE  0U

#define KEY_A0    (1U<<0)
#define KEY_A1    (1U<<1)
#define KEY_A2    (1U<<2)
#define KEY_A3    (1U<<3)
#define KEY_A4    (1U<<4)
#define KEY_A5    (1U<<5)
#define KEY_A6    (1U<<6)
#define KEY_A7    (1U<<7)
#define KEY_A8    (1U<<8)
#define KEY_A9    (1U<<9)
#define KEY_A10   (1U<<10)
#define KEY_A11   (1U<<11)
#define KEY_A12   (1U<<12)
#define KEY_A13   (1U<<13)
#define KEY_A14   (1U<<14)
#define KEY_A15   (1U<<15)


#if __USE_PC__

#endif //__USE_PC__

#ifdef __USE_STM32__

#endif //__USE_STM32__

#ifdef __cplusplus
extern "C" {
#endif //__cplusplus

extern input_device key_default_device;

#ifdef __cplusplus
}
#endif //__cplusplus

#endif//__USE_KEY__ > 0

#endif//__KEY_H
