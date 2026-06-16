#include "../inc/dd.h"
#include "../inc/KSCOSsystem.h"

static int systime_read(dd_t* dd, void* time, uint32_t ms, uint32_t kreighter) {
    (void)dd; (void)ms; (void)kreighter;
    *((uint32_t*)time) = sysgettime();
    return 0;
}

static const driver_ops_t sys_drv_ops0 = {
    .ops_name = "systime",
    .open = OPEN_NULL_FUNC,
    .close = CLOSE_NULL_FUNC,
    .write = WRITE_NULL_FUNC,
    .read = systime_read,
    .ioctl = IOCTL_NULL_FUNC,
};
static const driver_ops_t* sys_drv_dd_ops[] = {&sys_drv_ops0};

static const pdrv_base_t sys_drv = {"sys_systime"};
REGISTER_DRIVER(sys_drv, sys_drv_dd_ops, 1);
