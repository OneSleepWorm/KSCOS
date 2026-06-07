#ifndef __CLOCKTASK_H__
#define __CLOCKTASK_H__

#include "KSCconfig.h"

typedef struct clock_task_t clock_task_t;
typedef ki8 (*CTASK_INIT_FUNC)(clock_task_t* task);
typedef ki8 (*CTASK_RUN_FUNC)(clock_task_t* task);
typedef void (*CTASK_CALLBACK_FUNC)(void* user_data);
typedef ki8 (*CTASK_STOP_FUNC)(clock_task_t* task);

typedef ki8 CTASK_INIT;
typedef ki8 CTASK_RUN;
typedef void CTASK_CALLBACK;
typedef ki8 CTASK_STOP;


// 任务结构体
// 任务ID
// 初始化函数
// 运行函数
// 回调函数
// 停止函数
typedef struct clock_task_t {
    const uint32_t task_id;
    CTASK_INIT_FUNC init;
    CTASK_RUN_FUNC run;
    CTASK_CALLBACK_FUNC callback;
    CTASK_STOP_FUNC stop;
    void* user_data;
}clock_task_t;

#if __USE_PC__
extern __volatile clock_task_t pc_clock_task;
#endif
#if __USE_STM32__
extern __volatile clock_task_t stm_clock_task;
#endif

#endif
