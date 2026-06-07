#include "../inc/KSCconfig.h"

#if __USE_CLOCK_TASK__
#include "../inc/clocktask.h"

#if __USE_ARMCC__

#endif
#if __USE_PC__
#include <windows.h>
#include <pthread.h>
// 任务周期，单位：毫秒
static const uint32_t pc_clock_task_cycle = 1000;
// 任务名称
const uint32_t pc_clock_task_id =0x00000001;
// 任务结束标志
static ki8 pc_clock_task_running_flag = 0;

// 任务线程函数
static void* task_funtion(void* task){
    clock_task_t* taskp = (clock_task_t*)task;
    printf("task start:%d\n",taskp->task_id);
    while(pc_clock_task_running_flag){
        Sleep(pc_clock_task_cycle);
        taskp->callback(taskp->user_data);
    }
}

CTASK_RUN clock_task_run(clock_task_t* task){
        pthread_t thread_id;
        pthread_create(&thread_id, NULL
        , task_funtion, (void*)task);
        pc_clock_task_running_flag = 1;
        pthread_detach(thread_id);
        return 0;
}

CTASK_STOP clock_task_stop(clock_task_t* task){
    pc_clock_task_running_flag = 0;
    return 0;
}

clock_task_t pc_clock_task={
    .task_id = pc_clock_task_id,
    .init = NULL,
    .run = clock_task_run,
    .callback = NULL,
    .stop = clock_task_stop,
};
#endif

#endif
