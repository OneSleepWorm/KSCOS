#include "../inc/dd.h"
#include <stdio.h>

static int console_ioctl(dd_t* dd, const char* fmt, va_list ap) {
    (void)dd;
    char buf[256];
    int n = vsnprintf(buf, sizeof(buf), fmt, ap);
    if (n > 0) {
        size_t len = (size_t)n < sizeof(buf) ? (size_t)n : sizeof(buf) - 1;
        fwrite(buf, 1, len, stdout);
        fflush(stdout);
    }
    return n;
}

static const driver_ops_t console_ops = {
    .ops_name = "console",
    .ioctl = console_ioctl,
};

REGISTER_DRIVER("sys_console", &console_ops);
