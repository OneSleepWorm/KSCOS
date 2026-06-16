////////////////////////////////////////////////////////////////
/**
 * @antuor   OneSleepWorm(一只瞌睡虫)
 * @brief    定义设备结构体+驱动结构体+dd设备描述符
 * 
 */
////////////////////////////////////////////////////////////////

/** 
 * @brief 设备驱动注册示例
 * 
 * @note 该示例仅用于说明设备驱动注册的流程，实际应用中需要根据具体需求进行修改。
 *
    int systime_read(dd_t* dd, void* time,uint32_t ms,uint32_t kreighter){
    *((uint32_t*)time) = sysgettime();
    return 0;
    }
 
    static const driver_ops_t sys_drv_ops0 = {
    .ops_name="systime",
    .open=OPEN_NULL_FUNC,
    .close=CLOSE_NULL_FUNC,
    .write=WRITE_NULL_FUNC,
    .read=systime_read,
    .ioctl=IOCTL_NULL_FUNC,
    };
    REGISTER_DRIVER("sys_systime", &sys_drv_ops0);
 */

#include "../inc/dd.h"
#include "../inc/KSCOSsystem.h"
#include "string.h"

__volatile static bus_t KSC_bus={0};
/**
 * @brief 初始化总线
 * @note 初始化总线时，需要分配设备表内存，初始化设备表，初始化驱动表，拷贝设备数据而非指针到总线。
 * @note 会分配sizeof(pdev_t) * 设备数量的内存给设备表。
 * @return int 0 成功 -1 失败
 */
int bus_init(void)
{
    // 初始化总线
    //分配设备表内存
    KSC_bus.dev_table = oscalloc(((const char*)__stop_pdev_table - (const char*)__start_pdev_table) / sizeof(pdev_t), sizeof(pdev_t));
    //检查内存是否成功分配
    if(KSC_bus.dev_table == NULL)
    {
        return -1;
    }
    //初始化设备表
    KSC_bus.dev_count = ((const char*)__stop_pdev_table - (const char*)__start_pdev_table) / sizeof(pdev_t);
    //初始化驱动表
    size_t drv_section_bytes = (const char*)__stop_pdrv_table - (const char*)__start_pdrv_table;
    KSC_bus.drv_count = drv_section_bytes / sizeof(pdrv_t);

    //拷贝设备数据到总线
    memcpy(KSC_bus.dev_table, __start_pdev_table, KSC_bus.dev_count * sizeof(pdev_t));

    //拷贝驱动数据到总线（sizeof(pdrv_t)=16 为 2 的幂，linker 按 sizeof 连续排布）
    KSC_bus.drv_table = oscalloc(KSC_bus.drv_count, sizeof(pdrv_t));
    if(KSC_bus.drv_table == NULL) return -1;
    memcpy(KSC_bus.drv_table, __start_pdrv_table, KSC_bus.drv_count * sizeof(pdrv_t));

    //调试：打印数量
    printf("dev_count: %d\n", KSC_bus.dev_count);
    printf("drv_count: %d\n", KSC_bus.drv_count);
    //调试：打印设备名
    for(const pdev_t* dev = KSC_bus.dev_table; dev < KSC_bus.dev_table + KSC_bus.dev_count; dev++)
    {
        printf("dev_name: %s\n", dev->base.device_name);
    }
    //调试：打印驱动名
    for(const pdrv_t* drv = KSC_bus.drv_table; drv < KSC_bus.drv_table + KSC_bus.drv_count; drv++)
    {
        printf("drv_name: %s\n", drv->base.driver_name);
    }
    //调试结束
    return 0;
}
/**
 * @brief 获取设备驱动描述符
 * 
 * @param device_name 设备名称
 * @param driver_ops_name 驱动描述符名称(驱动完整结构体的子集,可为空)
 * @return dd_t* 设备驱动描述符指针
 * @note 我们人为规定设备名称的最后1位为设备号，除非你明确确定使用，否则无需指定设备号
 * @note 驱动匹配规则：只要驱动前缀与设备名称相同，即可匹配到对应的驱动，注意命名规范
 * @note 驱动命名推荐：设备名称_驱动名称,例如：spi1_lcdst7789
 */
dd_t* bus_getdriver(char* device_name, char* driver_ops_name)
{
    if(device_name == NULL)
    {
        return NULL;
    }
    // 先查找总线所挂载的设备中是否存在该设备，获取它
    const pdev_t* bus_dev = NULL;
    for(const pdev_t* dev = KSC_bus.dev_table; dev < KSC_bus.dev_table + KSC_bus.dev_count; dev++)
    {
        if(strncmp(device_name, dev->base.device_name, strlen(device_name)) == 0)
        {
            bus_dev = dev;
        }
    }
    // 查找驱动：遍历所有匹配设备名的驱动，按注册顺序返回第一个匹配的
    for(const pdrv_t* drv = KSC_bus.drv_table; drv < KSC_bus.drv_table + KSC_bus.drv_count; drv++)
    {
        if(strncmp(device_name, drv->base.driver_name, strlen(device_name)) != 0)
            continue;

        if(driver_ops_name != NULL && strncmp(driver_ops_name, drv->ops->ops_name, strlen(driver_ops_name)) != 0)
            continue;

        dd_t* dd = osmalloc(sizeof(dd_t));
        if(dd == NULL) return NULL;
        dd->dd_ops = drv->ops;
        dd->callback = CALLBACK_NULL_FUNC;
        dd->driver_data = NULL;
        dd->user_data = NULL;
        return dd;
    }

    return NULL;
}

int null_func(struct dd_t* dev){return 0;}
int null_rw_func(struct dd_t* dev, void* data, uint32_t size, uint32_t kreigster){return 0;}
int null_ioctl_func(struct dd_t* dev, const char* fmt, va_list ap){(void)dev;(void)fmt;(void)ap;return 0;}
void* null_callback(void* data){return NULL;}

int ddopen(dd_t* dd){
    if(dd->dd_ops->open == NULL) return -1;
    return dd->dd_ops->open(dd);
}
int ddclose(dd_t* dd){
    if(dd->dd_ops->close == NULL) return -1;
    return dd->dd_ops->close(dd);
}
int ddread(dd_t* dd, void* data, uint32_t size, uint32_t kreigster){
    if(dd->dd_ops->read == NULL) return -1;
    return dd->dd_ops->read(dd, data, size, kreigster);
}
int ddwrite(dd_t* dd, void* data, uint32_t size, uint32_t kreigster){
    if(dd->dd_ops->write == NULL) return -1;
    return dd->dd_ops->write(dd, data, size, kreigster);
}

int ddioctl(dd_t* dd, const char* fmt, ...) {
    if (!dd || !dd->dd_ops || !dd->dd_ops->ioctl) return -1;
    va_list ap;
    va_start(ap, fmt);
    va_list ap_copy;
    va_copy(ap_copy, ap);
    int ret = dd->dd_ops->ioctl(dd, fmt, ap_copy);
    va_end(ap_copy);
    va_end(ap);
    return ret;
}


