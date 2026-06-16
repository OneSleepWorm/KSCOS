#include "../inc/dd.h"
#include "../inc/KSCOSsystem.h"
#if __USE_PC__
#include <pthread.h>
#include <windows.h>

typedef struct {
    volatile uint8_t running;
    pthread_t thread;
} timer_ctx_t;

static void* timer_loop(void* arg) {
    dd_t* dd = (dd_t*)arg;
    timer_ctx_t* ctx = (timer_ctx_t*)dd->driver_data;
    uint16_t period = (uint16_t)(uintptr_t)dd->user_data;
    while (ctx->running) {
        Sleep(period);
        if (dd->callback) dd->callback(dd->user_data);
    }
    return NULL;
}

static int timer_open(dd_t* dd) {
    if (!dd->callback) return -1;
    if (!dd->user_data) dd->user_data = (void*)(uintptr_t)1000;

    timer_ctx_t* ctx = (timer_ctx_t*)dd->driver_data;
    if (!ctx) {
        ctx = (timer_ctx_t*)osmalloc(sizeof(timer_ctx_t));
        if (!ctx) return -1;
        dd->driver_data = ctx;
    }
    ctx->running = 1;
    pthread_create(&ctx->thread, NULL, timer_loop, dd);
    pthread_detach(ctx->thread);
    return 0;
}

static int timer_close(dd_t* dd) {
    timer_ctx_t* ctx = (timer_ctx_t*)dd->driver_data;
    if (!ctx) return -1;
    ctx->running = 0;
    return 0;
}

static int timer_read(dd_t* dd, void* data, uint32_t size, uint32_t kreigster) {
    (void)dd; (void)kreigster;
    if (size >= sizeof(uint32_t)) *(uint32_t*)data = GetTickCount();
    return 0;
}

static const driver_ops_t timer_ops = {
    .ops_name = "clock",
    .open = timer_open,
    .close = timer_close,
    .read = timer_read,
};

REGISTER_DRIVER("tim_clocktask", &timer_ops);

#endif
