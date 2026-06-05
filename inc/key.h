#ifndef __KEY_H
#define __KEY_H
#include "KSCconfig.h"

#if __USE_KEY__

#define KEY_NONE  0xFF

#define KEY_A0    0
#define KEY_A1    1
#define KEY_A2    2
#define KEY_A3    3
#define KEY_A4    4
#define KEY_A5    5
#define KEY_A6    6
#define KEY_A7    7
#define KEY_A8    8
#define KEY_A9    9
#define KEY_A10   10
#define KEY_A11   11
#define KEY_A12   12
#define KEY_A13   13
#define KEY_A14   14
#define KEY_A15   15

//定义按键持续状态宏
#define KEY_PRESS (1<<4)//按键按下
#define KEY_RELEASE (1<<5)//按键松开
//#define KEY_NOPRESS (0)//无按键
#define KEY_KEEP_FULL (0x03<<4)//按键保持按下并持续了_USE_KEY_KEEP_次
#define KEY_KEEP_NOFULL (0)//按键保持按下但时间不足

//按键分区
#define KEY_PSY(key) (key&0x0F)
#define KEY_LOG(key) (key>>4)

#define _USE_KEY_KEEP_ 0

#ifdef __cplusplus
extern "C" {
#endif
typedef int (* BOTTON_CALLBACK)(void);
typedef struct {
    uint32_t input_name_hash;
    uint8_t key;
}input_t;
extern input_t input_queue[KEY_QUEUE_SIZE];
void key_init(void);
uint8_t key_scan(void);
uint8_t key_read(void);
input_t* input_get(void);
typedef input_t* (*INPUT_FUNC)(void);

typedef input_t* (*INPUT_GET_FUNC)(char* input_name);
typedef uint8_t (*INPUT_ADD_FUNC)(char* input_name,void* data);
typedef struct k_key_device{
    INPUT_GET_FUNC input_get;
    INPUT_ADD_FUNC input_add;
}k_key_device;

k_key_device* k_key_device_init(void);
k_key_device* k_key_device_find(const char* app_name);

#ifdef __cplusplus
}//extern "C"
#endif


#endif//__USE_KEY__ > 0

#endif//__KEY_H
