#include "../inc/dd.h"
#include "../inc/KSCOSsystem.h"

static int systime_read(dd_t* dd, void* data, uint32_t count, uint32_t kreighter) {
    *((uint32_t*)data) = sysgettime();
    return 0;
}
static int systime_write(dd_t* dd, void* data, uint32_t count, uint32_t kreighter) {
    sysdelay(count);
    return 0;
}

static const driver_ops_t sys_drv_ops0 = {
    .ops_name = "systime",
    .open = OPEN_NULL_FUNC,
    .close = CLOSE_NULL_FUNC,
    .write = systime_write,
    .read = systime_read,
    .ioctl = IOCTL_NULL_FUNC,
};
REGISTER_DRIVER("sys_systime", &sys_drv_ops0);
