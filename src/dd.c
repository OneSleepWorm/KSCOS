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
 *  const pdev_base_t sys_dev={"sys"};
 *  REGISTER_DEVICE(sys_dev);
 *  const pdrv_base_t sys_drv={"sys_drv"};
 * 
    int systime_read(dd_t* dd, void* time,uint32_t ms,uint32_t kreighter){
    *((uint32_t*)time) = sysgettime();
    return 0;
    }
 
    driver_ops_t sys_drv_ops0 = {
    .ops_name="systime",
    .open=OPEN_NULL_FUNC,
    .close=CLOSE_NULL_FUNC,
    .write=WRITE_NULL_FUNC,
    .read=systime_read,
    .ioctl=IOCTL_NULL_FUNC,
    };
    static const driver_ops_t* sys_drv_dd_ops[] = {&sys_drv_ops0};
    REGISTER_DRIVER(sys_drv, sys_drv_dd_ops, 1);
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
    size_t drv_stride = drv_section_bytes / KSC_bus.drv_count;

    //拷贝设备数据到总线
    memcpy(KSC_bus.dev_table, __start_pdev_table, KSC_bus.dev_count * sizeof(pdev_t));

    //拷贝驱动数据到总线（需考虑链接器对齐间距）
    KSC_bus.drv_table = oscalloc(KSC_bus.drv_count, sizeof(pdrv_t));
    if(KSC_bus.drv_table == NULL) return -1;
    const char* src = (const char*)__start_pdrv_table;
    pdrv_t* dst = KSC_bus.drv_table;
    for(uint32_t i = 0; i < KSC_bus.drv_count; i++) {
        memcpy(dst++, src, sizeof(pdrv_t));
        src += drv_stride;
    }
    printf("KSC_bus.dev_count = %d\n", KSC_bus.dev_count);
    for(uint32_t i = 0; i < KSC_bus.dev_count; i++)
    {
        printf("KSC_bus.dev_table[%d] = %s\n", i, KSC_bus.dev_table[i].base.device_name);
    }
    printf("KSC_bus.drv_count = %d\n", KSC_bus.drv_count);
    // 打印驱动表(驱动名称)
    for(uint32_t i = 0; i < KSC_bus.drv_count; i++){
        printf("KSC_bus.drv_table[%d] = %s\n", i, KSC_bus.drv_table[i].base.driver_name);
    }
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
        // 比较设备名称
        if(strncmp(device_name, dev->base.device_name, strlen(device_name)) == 0)
        {
            bus_dev = dev;
        }
    }
    // 再查找驱动是否存在(从总线驱动表查找)
    const pdrv_t* bus_drv = NULL;
    for(const pdrv_t* drv = KSC_bus.drv_table; drv < KSC_bus.drv_table + KSC_bus.drv_count; drv++)
    {
        // 比较驱动名称(驱动名称的前缀必须与设备名称相同)
        if(strncmp(device_name, drv->base.driver_name, strlen(device_name)) == 0)
        {
            bus_drv = drv;
        }
    }
    if(bus_drv == NULL) return NULL;
    dd_t* dd = osmalloc(sizeof(dd_t));
    if(dd == NULL) return NULL;
    // 如果driver_ops_name为空，获取第一个驱动描述符操作集,否则匹配driver_ops_name
    if(driver_ops_name == NULL)
    {
        dd->dd_ops = bus_drv->dd_ops[0];
        dd->callback = CALLBACK_NULL_FUNC;
        dd->driver_data = NULL;
        dd->user_data = NULL;
        return dd;
    }
    else{
        for(int i =0;i<bus_drv->dd_count;i++){
            if(strncmp(driver_ops_name, bus_drv->dd_ops[i]->ops_name, strlen(driver_ops_name)) == 0)
            {
                dd->dd_ops = bus_drv->dd_ops[i];
                dd->callback = CALLBACK_NULL_FUNC;
                dd->driver_data = NULL;
                dd->user_data = NULL;
                return dd;
            }
        }
    }
    
    // 如果没有找到匹配的驱动描述符，返回NULL
    osfree(dd);
    return NULL;
}

int null_func(struct dd_t* dev){return 0;}
int null_rw_func(struct dd_t* dev, void* data, uint32_t size, uint32_t kreigster){return 0;}
int null_ioctl_func(struct dd_t* dev, uint8_t argc, const char** argv){return 0;}
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

#include <stdarg.h>
#include <stdlib.h>

int ddioctl(dd_t* dd, uint8_t argc, ...) {
    if (argc == 0 || dd == NULL || dd->dd_ops == NULL) {
        return -1;
    }
    
    va_list args;
    va_start(args, argc);
    
    // 分配字符串指针数组
    const char** argv = malloc(argc * sizeof(const char*));
    if (!argv) {
        va_end(args);
        return -1;
    }
    
    // 逐个取出字符串指针
    for (uint8_t i = 0; i < argc; i++) {
        argv[i] = va_arg(args, const char*);  // 取 char*，注意不是 uint8_t*
    }
    
    va_end(args);
    
    int ret = dd->dd_ops->ioctl(dd, argc, argv);
    
    free(argv);
    return ret;
}


