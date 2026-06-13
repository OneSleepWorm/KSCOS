#ifndef __CLOCKTASK_H__
#define __CLOCKTASK_H__

#include "KSCconfig.h"

typedef struct clock_task_t clock_task_t;
typedef ki8 (*CTASK_INIT_FUNC)(clock_task_t* task);
typedef ki8 (*CTASK_RUN_FUNC)(clock_task_t* task);
// 回调函数,这里的返回值实际上没有意义,只是为了保持一致的接口
typedef ki8 (*CTASK_CALLBACK_FUNC)(void* user_data);
typedef ki8 (*CTASK_STOP_FUNC)(clock_task_t* task);

typedef ki8 CTASK_INIT;
typedef ki8 CTASK_RUN;
typedef ki8 CTASK_CALLBACK;
typedef ki8 CTASK_STOP;


// 任务结构体
// 任务ID
// 初始化函数
// 运行函数
// 回调函数
// 停止函数
typedef struct clock_task_t {
    CTASK_INIT_FUNC init;
    CTASK_RUN_FUNC run;
    CTASK_CALLBACK_FUNC callback;
    CTASK_STOP_FUNC stop;
    void* user_data;
    uint16_t task_cycle;
    const uint8_t task_id;
    uint8_t state;
}clock_task_t;

clock_task_t  clock_task_create(CTASK_INIT_FUNC init,CTASK_CALLBACK_FUNC callback
    ,void* user_data,uint16_t task_cycle);

extern const clock_task_t clock_default_task;

#endif
