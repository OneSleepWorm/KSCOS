#include "inc/master.h"

static void* timer_tick(void* arg) { (void)arg;
    static int count = 0;
    printf("tick %d\n", count++);
    return NULL;
}

int main(void)
{
    bus_init();

    dd_t* tmr = bus_getdriver("tim", "clock");
    if (!tmr) {
        printf("timer driver not found\n");
        return -1;
    }
    printf("timer driver: %s\n", tmr->dd_ops->ops_name);

    tmr->user_data = (void*)(uintptr_t)500;
    tmr->callback = timer_tick;
    ddopen(tmr);
    Sleep(3000);
    ddclose(tmr);
    Sleep(500);
    printf("timer stopped\n");

    dd_t* sys = bus_getdriver("sys", "systime");
    if (sys) {
        uint32_t t = 0;
        ddread(sys, &t, sizeof(t), 0);
        printf("systime: %u\n", t);
    }

    return 0;
}
