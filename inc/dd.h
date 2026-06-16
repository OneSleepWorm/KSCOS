#ifndef __dd_h__
#define __dd_h__

#include "KSCconfig.h"
#include <stdarg.h>

/**
 * @brief KSCOS 设备-驱动(Device-Driver)总线框架
 *
 * ============================================================
 * 一、设计思想
 * ============================================================
 * 受 Linux 设备驱动模型启发，结合嵌入式场景简化而来。
 * 核心思想：用 "名字前缀匹配" 将设备(Device)与驱动(Driver)桥接，
 * 设备只需知道自己是什么(名字)，驱动只需声明自己能驱动什么(名字)，
 * 总线(bus)在运行时根据名字前缀自动完成匹配。
 *
 * 此处的"设备"不是寄存器地址或中断号，而是一个逻辑名称——
 * 它代表一个可被驱动服务的外设实例。
 *
 * ============================================================
 * 二、三层架构
 * ============================================================
 *
 * ┌─────────────────────────────────────────────────────────┐
 * │  第2层: pdrv_t (驱动)                                    │
 * │  作用: 描述一类驱动程序的能力                             │
 * │  内容: 驱动名 + 单个 ops 操作集                           │
 * │  注册: REGISTER_DRIVER(...) → 编入 pdrv_table 段        │
 * │  命名: 设备前缀_功能名, 如 tim_clocktask / sys_systime    │
 * │                                                         │
 * │  第2层: pdrv_t (驱动)                                    │
 * │  作用: 描述一类驱动程序的能力                             │
 * │  内容: 驱动名 + dd_ops[] 操作集数组                       │
 * │  注册: REGISTER_DRIVER(...) → 编入 pdrv_table 段        │
 * │  命名: 设备前缀_功能名, 如 tim_clocktask / sys_systime    │
 * │                                                         │
 * │  第3层: dd_t (设备描述符)                                │
 * │  作用: 用户与驱动交互的运行时句柄                          │
 * │  内容: dd_ops(操作集) + callback + driver_data + user_data│
 * │  获取: bus_getdriver(dev_name, ops_name) → dd_t*        │
 * └─────────────────────────────────────────────────────────┘
 *
 *
 * ============================================================
 * 三、匹配规则（三步前缀匹配）
 * ============================================================
 * bus_getdriver(device_name, driver_ops_name) 执行:
 *
 *   第1步: 遍历设备表, 找 device_name 前缀匹配的设备
 *          例: "tim" → 匹配 "tim1", "tim2", "tim3", "tim4"
 *          总是匹配最后一个(若同一个前缀有多个, 取最后注册的)
 *
 *   第2步: 遍历驱动表, 找 device_name 前缀匹配的驱动
 *          例: "tim" → 匹配 "tim_clocktask"
 *          按注册顺序返回第一个匹配的驱动
 *
 *   第3步: 匹配 driver_ops_name 与驱动内 ops_name
 *          例: "clock" → 匹配 ops_name="clock"
 *          若 driver_ops_name 为 NULL, 返回第一个匹配的驱动
 *
 *   返回: 新分配的 dd_t* (需用后释放由总线管理, 暂由用户保证生命周期)
 *
 * ★ 关键: device_name 是连接设备与驱动的"钥匙"——
 *        设备名和驱动名必须有相同的前缀, 才能配对.
 *
 *
 * ============================================================
 * 四、命名规范
 * ============================================================
 * 设备名   →  type + 实例号          如 "tim1", "sys0"
 * 驱动名   →  设备前缀_功能          如 "tim_clocktask"
 * ops_name →  具体功能名             如 "clock", "systime"
 *
 * 驱动名推荐用 "设备前缀_功能" 格式:
 *   - tim_clocktask   (定时器 → 时钟任务)
 *   - tim_pwm         (定时器 → PWM 输出)
 *   - spi1_lcdst7789  (SPI1 → ST7789 屏)
 *   - i2c1_mpu6050    (I2C1 → MPU6050 传感器)
 *
 *
 * ============================================================
 * 五、使用流程
 * ============================================================
 *
 *   // 1. 总线初始化 (一次, 程序入口)
 *   bus_init();
 *
 *   // 2. 获取设备描述符
 *   dd_t* tmr = bus_getdriver("tim", "clock");
 *   //         ↑ 设备前缀     ↑ ops_name
 *
 *   // 3. 配置参数
 *   tmr->user_data = (void*)(uintptr_t)500;  // 500ms
 *   tmr->callback  = my_tick_callback;
 *
 *   // 4. 打开 (驱动开始工作)
 *   ddopen(tmr);
 *
 *   // 5. 使用 ...
 *   Sleep(3000);
 *
 *   // 6. 关闭
 *   ddclose(tmr);
 *
 *   // 7. 读写 (可选)
 *   uint32_t t;
 *   ddread(tmr, &t, sizeof(t), 0);
 *
 *
 * ============================================================
 * 六、注意事项
 * ============================================================
 *
 * 1. pdrv_t 对齐
 *    pdrv_t 的 sizeof(=16) 是 2 的幂，与 aligned(16) 一致，
 *    链接器在 section 中以 sizeof 为单位连续排布，stride = sizeof，
 *    section 字节数可精确用于计数。
 *
 *    pdev_t 同理，sizeof(=16) 也是 2 的幂，无需特殊处理。
 *
 * 2. 静态库链接丢弃问题
 *    若一个 .c 文件只包含 REGISTER_DEVICE / REGISTER_DRIVER 而
 *    没有任何被其他代码引用的符号, 则该文件所在的静态库目标文件
 *    (.o) 会被链接器丢弃, 导致注册不生效.
 *    解决: 此类文件必须直接加入 add_executable(...) 而不是
 *    add_library(...). 参见 timer.c / device.c 在 CMakeLists.txt
 *    中的位置.
 *
 * 3. device_name 不带设备号
 *    用户调用 bus_getdriver 时通常只需指定设备前缀, 如 "tim"
 *    而非 "tim1". 前缀匹配会自动选中该前缀的最后一个设备.
 *    若确实需要特指某个实例, 可传入全名如 "tim2".
 *
 * 4. dd_t 生命周期
 *    bus_getdriver 每次返回新分配的 dd_t, 目前总线不负责回收.
 *    简单场景下用户可直接使用无需 free; 高频创建/销毁场景需
 *    用户自行管理.
 *
 * 5. callback 与 driver_data / user_data 的约定
 *    - callback: 由驱动在特定事件(如定时器到期)时调用, 参数为
 *                user_data
 *    - driver_data: 由驱动的 open/close 管理, 用户不应直接修改
 *    - user_data: 由用户在 ddopen 前配置, 驱动在 open 时读取
 *
 * ============================================================
 * 七、完整注册示例
 * ============================================================
 *
 *   // === 设备注册 (device.c) ===
 *   REGISTER_DEVICE("tim1");
 *
 *   // === 驱动注册 (timer.c) ===
 *   static int timer_open(dd_t* dd) { ... }
 *   static int timer_close(dd_t* dd) { ... }
 *   static int timer_read(dd_t* dd, void* data, uint32_t size, ...) { ... }
 *
 *   static driver_ops_t timer_ops = {
 *       .ops_name = "clock",
 *       .open = timer_open,
 *       .close = timer_close,
 *       .read = timer_read,
 *   };
 *   REGISTER_DRIVER("tim_clocktask", &timer_ops);
 *
 *   // === 用户使用 ===
 *   dd_t* tmr = bus_getdriver("tim", "clock");
 *   tmr->user_data = (void*)(uintptr_t)500;
 *   tmr->callback = my_callback;
 *   ddopen(tmr);
 */

#define container_of(ptr, type, member) ({                      \
    const typeof(((type *)0)->member) * __mptr = (ptr);     \
    (type *)((char *)__mptr - offsetof(type, member)); })

typedef struct pdev_t pdev_t;
typedef struct pdrv_t pdrv_t;

/**
 * @brief 设备基类: 仅包含设备名称
 * @note  命名格式 type+实例号, 如 "tim1", "sys0", "spi2"
 */
typedef struct pdev_base_t{
    const char* device_name;
} pdev_base_t;

/**
 * @brief 设备完整结构
 * @note  包含基类 + 总线匹配到的驱动指针
 */
typedef struct pdev_t {
    pdev_base_t base;
    pdrv_t* driver;      // 由 bus_init 填充
}pdev_t;

/**
 * @brief 总线结构
 * @note  保存设备表与驱动表的运行时副本
 */
typedef struct bus_t bus_t;
typedef struct bus_t{
    pdev_t* dev_table;   // 设备表 (从 pdev_table 段拷贝)
    uint32_t dev_count;
    pdrv_t* drv_table;   // 驱动表 (从 pdrv_table 段拷贝)
    uint32_t drv_count;
} bus_t;

/**
 * @brief 驱动基类: 仅包含驱动名称
 * @note  命名格式 设备前缀_功能, 如 "tim_clocktask", "sys_systime"
 *        驱动名必须与设备名有相同前缀, 才能被总线匹配
 */
typedef struct pdrv_base_t{
    const char* driver_name;
} pdrv_base_t;

typedef struct driver_ops_t driver_ops_t;

/**
 * @brief 驱动完整结构
 * @note  基类 + 指向单个驱动操作集的指针
 *        sizeof=16（2 的幂），与 16 字节对齐一致，
 *        确保链接器在 section 中按 sizeof 为单位连续排布
 */
typedef struct __attribute__((aligned(16))) pdrv_t {
    pdrv_base_t base;
    const driver_ops_t* ops;            // 指向单个 ops
} pdrv_t;

typedef struct dd_t dd_t;

/**
 * @brief 通用回调函数类型
 * @param data  用户数据指针 (对应 dd_t.user_data)
 */
typedef void* (*void_func_t)(void* data);

/**
 * @brief 设备描述符 ( Device Descriptor )
 * @note  用户与驱动交互的核心句柄, 由 bus_getdriver 创建并返回
 * 
 *        dd_ops      — 指向某个 driver_ops_t, 决定 open/close/read/write/ioctl 的实现
 *        callback    — 异步回调 (如定时器到期), 参数为 user_data
 *        driver_data — 驱动内部状态, 由 open/close 管理, 用户不应直接修改
 *        user_data   — 用户自定义参数, 在 ddopen 前设置, 驱动在 open 时读取
 */
typedef struct dd_t{
    const driver_ops_t* dd_ops;
    void_func_t callback;
    void* driver_data;
    void* user_data;
} dd_t;

typedef int (*driver_open_func)(struct dd_t* dd);
typedef int (*driver_close_func)(struct dd_t* dd);
typedef int (*driver_read_func)(struct dd_t* dd, void* data, uint32_t size, uint32_t kreigster);
typedef int (*driver_write_func)(struct dd_t* dd, void* data, uint32_t size, uint32_t kreigster);
typedef int (*driver_ioctl_func)(struct dd_t* dd, const char* fmt, va_list ap);

/**
 * @brief 驱动操作集
 * @note  每个操作集有独立 ops_name, 同一驱动可包含多个操作集
 *        (例如一个驱动同时提供 clock 和 pwm 两种操作)
 */
typedef struct driver_ops_t{
    const char* ops_name;       // 操作集名称, 用于 bus_getdriver 匹配
    driver_open_func open;
    driver_close_func close;
    driver_read_func read;
    driver_write_func write;
    driver_ioctl_func ioctl;
} driver_ops_t;


#if __USE_PC__

// 辅助宏：用于展开 __COUNTER__ 后再进行拼接
#define _CONCAT_IMPL(a, b) a##b
#define _CONCAT(a, b)      _CONCAT_IMPL(a, b)

/**
 * @brief 静态注册设备
 * @param devname  设备名字符串 (如 "tim1", "sys0")
 * @note  生成的结构体被编入 "pdev_table" 链接段, bus_init 时拷贝到总线
 *        必须确保该 .c 文件不被链接器丢弃 (参见静态库注意事项)
 *        所有 REGISTER_DEVICE 调用集中放在 device.c 中
 */
#define REGISTER_DEVICE(devname) \
    static const pdev_t _CONCAT(_DEV_, __COUNTER__) \
    __attribute__((section("pdev_table"), used)) = \
    {{devname}, NULL}

/**
 * @brief 静态注册驱动
 * @param drvname  驱动名字符串 (如 "tim_clocktask", "sys_systime")
 * @param ops_ptr  驱动操作集指针 (driver_ops_t*)
 * @note  生成的结构体被编入 "pdrv_table" 链接段, bus_init 时拷贝到总线
 *        必须确保该 .c 文件不被链接器丢弃 (参见静态库注意事项)
 *        驱动通常注册在其实现文件(如 timer.c)中
 *        用法示例:
 *          static const driver_ops_t my_ops = { .ops_name = "...", ... };
 *          REGISTER_DRIVER("tim_clocktask", &my_ops);
 */
#define REGISTER_DRIVER(drvname, ops_ptr) \
    static const pdrv_t _CONCAT(_DRV_, __COUNTER__) \
    __attribute__((section("pdrv_table"), used)) = \
    {{drvname}, ops_ptr}

// 链接器自动生成的段边界符号 (由 __attribute__((section)) 配合 ld 产生)
extern const pdev_t __start_pdev_table[];
extern const pdev_t __stop_pdev_table[];

extern const pdrv_t __start_pdrv_table[];
extern const pdrv_t __stop_pdrv_table[];
#endif // __USE_PC__

int null_func(struct dd_t* dev);  
int null_rw_func(struct dd_t* dev, void* data, uint32_t size, uint32_t kreigster);
int null_ioctl_func(struct dd_t* dev, const char* fmt, va_list ap);
void* null_callback(void* data);

#define OPEN_NULL_FUNC null_func
#define CLOSE_NULL_FUNC null_func
#define READ_NULL_FUNC null_rw_func
#define WRITE_NULL_FUNC null_rw_func
#define IOCTL_NULL_FUNC null_ioctl_func
#define CALLBACK_NULL_FUNC null_callback
#define DRIVER_DATA_NULL_FUNC NULL
#define USER_DATA_NULL_FUNC NULL

int bus_init(void);
dd_t* bus_getdriver(char* device_name, char* driver_ops_name);

int ddopen(dd_t* dd);
int ddclose(dd_t* dd);
int ddread(dd_t* dd, void* data, uint32_t size, uint32_t kreigster);
int ddwrite(dd_t* dd, void* data, uint32_t size, uint32_t kreigster);

int ddioctl(dd_t* dd, const char* fmt, ...);


#endif // __dd_h__
