#ifndef __KEY_H
#define __KEY_H
#include "KSCconfig.h"

#if __USE_KEY__
#include "input.h"
#define KEY_NONE  0xFF

#define KEY_A0    (1<<0)
#define KEY_A1    (1<<1)
#define KEY_A2    (1<<2)
#define KEY_A3    (1<<3)
#define KEY_A4    (1<<4)
#define KEY_A5    (1<<5)
#define KEY_A6    (1<<6)
#define KEY_A7    (1<<7)
#define KEY_A8    (1<<8)
#define KEY_A9    (1<<9)
#define KEY_A10   (1<<10)
#define KEY_A11   (1<<11)
#define KEY_A12   (1<<12)
#define KEY_A13   (1<<13)
#define KEY_A14   (1<<14)
#define KEY_A15   (1<<15)


#ifdef __USE_PC__
#define PC_KEY_DEVICE_ID (1+1<<8)
#define KEY_DEVICE_ID PC_KEY_DEVICE_ID
#endif

#ifdef __USE_STM32__

#endif

#ifdef __cplusplus
extern "C" {
#endif

extern input_device key_default_device;

#ifdef __cplusplus
}
#endif //__cplusplus

#endif//__USE_KEY__ > 0

#endif//__KEY_H
