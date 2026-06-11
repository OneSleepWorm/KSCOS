#ifndef __INPUT_H
#define __INPUT_H
#include "KSCconfig.h"
/**
 * @brief 输入结构体
 * @note input_name:输入名称指针
 * @note key:按键值
 */
typedef struct input_t{
    uint32_t data;
    uint32_t input_id;
}input_t;

//输入设备驱动结构体
typedef ki8 (*INPUT_INIT_FUNC)(void);
typedef input_t (*INPUT_CREATE_FUNC)(void);
typedef ki8 (*INPUT_DEINIT_FUNC)(void);
typedef ki8 INPUT_INIT;
typedef input_t INPUT_CREATE;
typedef ki8 INPUT_DEINIT;

typedef struct input_device{
    uint32_t device_id;
    INPUT_INIT_FUNC input_init;
    INPUT_CREATE_FUNC input_create;
    INPUT_DEINIT_FUNC input_deinit;
}input_device;
typedef input_device input_device_t;
#ifdef __cplusplus
extern "C" {
#endif

//输入获取函数
input_t input_get(uint32_t input_id);
//输入添加函数
input_t input_add(input_t input);

#ifdef __cplusplus
}
#endif
#endif//__INPUT_H
