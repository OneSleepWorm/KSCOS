# KSCdraw — 跨平台 TFT 显示屏绘图库

通用 TFT 显示屏驱动库，提供基本图形绘制、图像显示、文本渲染、对象系统、脏矩形增量刷新和应用程序框架等功能。
通过预处理器宏切换平台（PC / ARM / ESP32），硬件依赖与绘图 API 分离。

## 功能特点

- **基本图形**：点、线、矩形、圆形、圆角矩形、圆弧、填充图形
- **图像显示**：直接绘制、二值化绘制、缩放绘制、多色深解码（1/2/4-bit → RGB565）
- **文本渲染**：ASCII 字符（7×7 / 8×16），可选中文支持（通过 Flash 字库）
- **对象系统**：统一 `ksc_obj_t`（12 字节）描述所有可绘制元素，支持批量绘制和脏矩形增量刷新
- **脏矩形系统**：增量屏幕更新，自动合并脏矩形，仅重绘受影响的区域
- **应用程序框架**：`REGISTER_APP` 宏 + GCC linker section 注册应用，支持 init / update / deinit 生命周期
- **命令行系统**（头文件已就绪）：`REGISTER_CMD` 宏 + GCC linker section 自动收集命令
- **文件菜单**（头文件已就绪）：基于对象系统的 littlefs 文件浏览器
- **输入抽象**：可插拔的按键设备接口（`k_key_device`）
- **设备驱动抽象**：绘图 API 通过 `k_draw_device` 函数指针接口操作硬件，`kscreenmount()` 自动创建并挂载
- **SPI Flash 模拟**：W25Q64 支持 PC 端文件模拟（`.data/w25q64_sim.bin`），无需硬件即可开发测试
- **多平台**：PC (EasyX) 开发 / ARM (Keil + STM32) 部署 / ESP32 部署

## 快速开始

### 0. 核心类型

```c
KSCCOLOR  // RGB565 颜色 (uint16_t)
uintxy    // 坐标 (uint16_t)
KSC_window   // 屏幕描述结构体
k_draw_device // 设备驱动接口（自动挂载）
```

### 1. 挂载设备并初始化屏幕

```c
#include "inc/KSCdraw.h"

// kscreenmount() 自动创建 k_draw_device 并调用 init()
k_draw_device* dev = kscreenmount();

// 栈分配屏幕
KSC_window screen = {
    .ssx = 0, .ssy = 0,
    .width = 240, .height = 160,
    .bk = wwhite,
};
// 或动态分配
KSC_window* screen = kscreeninit(dev, 0, 0, 240, 160, wwhite);
```

### 2. 绘制图形

```c
kfull(dev, &screen, bblue, 0, 0, 100, 100);          // 填充矩形
kroundrect(dev, &screen, rred, 20, 20, 50, 50, 10);  // 圆角矩形
kfillcircle(dev, &screen, ggreen, 70, 40, 15);        // 填充圆形
kcircle(dev, &screen, rred, 100, 40, 10);             // 圆形边框
kdrawimagebig(dev, &screen, Wechat, 120, 40, 16, 16, 4); // 缩放图像
```

### 3. 显示文本

```c
kstring(dev, &screen, "Hello World", 10, 0, rred, wwhite);
```

### 4. 使用对象系统 + 脏矩形刷新

```c
ksc_obj_t objs[] = {
    { ._type = _fillbox, .colorck = bblue,
      .sdx = 10, .sdy = 10, .width = 50, .height = 50 },
    { ._type = _string, .colorck = rred, .data = "OK",
      .sdx = 20, .sdy = 25, .width = 16, .height = 8 },
};
screen.objbuf = objs;
screen.objnum = 2;

kscreendraw(dev, &screen);     // 全屏绘制
// ... 修改对象后 ...
kdirtyrect_add_obj(dev, &screen, &objs[0]); // 标记脏区
kscreenupdate(dev, &screen);                // 增量刷新
```

### 5. 注册应用程序

```c
uint8_t myapp_init(ksc_app* app, void* argv) { return 0; }
uint8_t myapp_update(ksc_app* app, void* argv) {
    // 绘制逻辑
    return 0;
}
uint8_t myapp_deinit(ksc_app* app, void* argv) { return 0; }

REGISTER_APP(my_app, myapp_init, myapp_update, myapp_deinit);

// 运行时
ksc_app_list();                          // 列出所有注册的 App
ksc_app* app = ksc_app_init("my_app", NULL); // 初始化并运行
```

## 配置选项

在 `inc/KSCconfig.h` 中通过宏配置：

| 宏 | 作用 |
|---|---|
| `__USE_PC__` / `__USE_ARMCC__` / `__USE_ESP32__` | 平台选择（同一时间只应启用一个） |
| `__USE_LCD__` | 启用绘图库 |
| `__USE_CHINESE__` | 启用中文字符渲染 |
| `__USE_FLASH__` | 启用 SPI Flash (W25Q64) |
| `__USE_LITTLEFS__` | 启用 littlefs 文件系统 |
| `__USE_KEY__` | 启用按键输入 |
| `__USE_UART__` | 启用串口通信 |
| `KEY_QUEUE_SIZE` | 输入队列大小 |
| `__BUTTON_SIMU__` | PC 模拟按键模式 |
| `__LITTLE_END_COLOR__` | RGB565 字节序（1=小端，0=大端） |
| `SYSTEMFONT` | ASCII 字体宽度（7 或 8） |
| `COLORBIT` / `COLORBYTE` | 颜色位深（默认 2 字节 RGB565） |
| `TFTx` / `TFTy` | 屏幕分辨率（默认 240×160） |
| `_STATICBUF_SIZE` | 静态绘制缓冲区大小（默认 512 字节） |

## 目录结构

```
KSCdraw/
├── makefile              # PC 构建（TDM-GCC-64 + EasyX）
├── master.c              # 入口 / 绘图演示
├── inc/                  # 头文件
│   ├── KSCconfig.h       # 全局配置宏（平台、功能开关）
│   ├── KSCdraw.h         # 核心绘图 API + k_draw_device + KSC_window + ksc_obj_t
│   ├── KSCdisplay.h      # 显示驱动 HAL 接口
│   ├── KSCfont.h         # 字体系统接口（KSC_Font1 / KSC_FontChinese）
│   ├── KSCimg.h          # 图像数据定义 + kloadimage
│   ├── TFTDriver.h       # TFT SPI 底层驱动（ARM 平台）
│   ├── W25Q64.h          # SPI Flash 驱动（支持 PC 模拟）
│   ├── W25Q64_Ins.h      # Flash 指令集定义
│   ├── Serial.h          # 串口 I/O 抽象
│   ├── UTF8_FlashN.h     # UTF-8 解析 + Flash 字库地址映射
│   ├── cmd.h             # 命令行系统（REGISTER_CMD + linker section）
│   ├── key.h             # 按键输入抽象（k_key_device）
│   ├── application.h     # 应用程序框架（REGISTER_APP + linker section）
│   └── master.h          # 主程序头文件聚合
├── src/                  # 源文件
│   ├── KSCdraw.c         # 核心绘图 + 对象系统 + 脏矩形管理
│   ├── KSCdisplay.c      # 显示 HAL（PC EasyX / ARM TFT）
│   ├── KSCfont.c         # 字模数据 + 字体函数指针实现
│   ├── KSCimg.c          # 图像数据（11 个应用图标 + 加载函数）
│   └── TFTDriver.c       # STM32F1 TFT SPI/DMA 驱动
├── littlefs/             # littlefs 嵌入式文件系统（移植版）
├── three_party/stm32/    # STM32 HAL 外设驱动
├── examples/             # 使用示例
│   ├── obj/              # 基本绘图 + 对象菜单
│   ├── key/              # 按键输入 + 菜单
│   ├── littlefs/         # littlefs 启动示例
│   ├── flash/            # Flash 读写 + 中文显示
│   └── Dependence/       # 示例依赖配置
├── .data/                # 测试资源（w25q64_sim.bin）
├── LICENSE               # MIT 许可证
└── README.md
```

## API 参考

### 设备 & 屏幕管理

| 函数 | 说明 |
|---|---|
| `kscreenmount()` | 自动创建并挂载设备驱动，返回 `k_draw_device*` |
| `k_draw_device_init()` | 手动初始化全局设备 |
| `k_draw_device_find(name)` | 按名称查找设备 |
| `kscreeninit(dev, ssx, ssy, w, h, bk)` | 动态分配屏幕，返回 `KSC_window*` |
| `kscreenfree(dev, screen)` | 释放动态分配的屏幕 |
| `kscreenclear(dev, screen)` | 用背景色清屏 |

### 基本图形

| 函数 | 说明 |
|---|---|
| `ksetpixel(dev, screen, color, x, y)` | 设置单像素 |
| `kfull(dev, screen, color, x1, y1, w, h)` | 填充矩形区域（直线段/矩形边框的底层实现） |
| `kline(dev, screen, color, x1, y1, x2, y2)` | 任意方向线段（水平/垂直走快速路径） |
| `kbox(dev, screen, color, x1, y1, w, h)` | 矩形边框 |
| `kfillbox(dev, screen, color, x1, y1, w, h)` | 填充矩形 |
| `kcircle(dev, screen, color, x0, y0, r)` | 圆形边框 |
| `kfillcircle(dev, screen, color, x0, y0, r)` | 填充圆形（扫描线算法） |
| `karc(dev, screen, color, x0, y0, r, dir)` | 圆弧（按象限位掩码选择） |
| `kroundrect(dev, screen, color, x1, y1, w, h, r)` | 圆角矩形边框 |
| `kfillroundrect(dev, screen, color, x1, y1, w, h, r)` | 填充圆角矩形 |

### 图像 & 文本

| 函数 | 说明 |
|---|---|
| `kdrawimage(dev, screen, img, x, y, w, h)` | 绘制 RGB565 图像 |
| `kdrawimagebig(dev, screen, img, x, y, w, h, scale)` | 缩放绘制图像 |
| `kimagebin(dev, screen, img, x, y, w, h, ck, cbk)` | 绘制二值化图像（1-bit → RGB565） |
| `kchar(dev, screen, ch, x, y, ck, cbk)` | 绘制单个 ASCII 字符 |
| `kstring(dev, screen, str, x, y, ck, cbk)` | 绘制 ASCII 字符串 |
| `kstringchinese(dev, screen, str, x, y, c1, c2)` | 绘制中文字符串（需 `__USE_CHINESE__`） |
| `kloadimage(name)` | 按名称加载 Systemimg 中的图像，返回 `Img_File` |

内置图标（`KSCimg.c`）：`Wechat`, `QQ`, `Setting`, `Video`, `Photo`, `Tiktok`, `Qwen`, `Note`, `Game`, `Clock`, `Book`, `Alipay`（16×16 RGB565）

### 对象系统

对象类型常量：`_box`, `_circle`, `_string`, `_image`, `_imagebig`, `_roundrect`, `_fillroundrect`, `_fillbox`, `_fillcircle`, `_line`, `_char`

| 函数 | 说明 |
|---|---|
| `kobjdraw(dev, screen, obj)` | 绘制单个 `ksc_obj_t`（按 `_type` 分发） |
| `kobjsdraw(dev, screen, obj_array, num)` | 批量绘制（跳过已绘制对象） |
| `kobjsdraw_f(dev, screen, obj_array, num)` | 强制绘制（忽略绘制状态） |

### 脏矩形增量刷新

| 函数 | 说明 |
|---|---|
| `kdirtyrect_add(dev, screen, x, y, w, h)` | 手动标记脏矩形 |
| `kdirtyrect_add_obj(dev, screen, obj)` | 按对象位置标记脏矩形 |
| `kdirtyrect_del(dev, screen)` | 清除所有脏矩形 |
| `kdirtyrectmerge(dev, screen, mode)` | 合并脏矩形（mode: 1=面积阈值, 2/3=重叠检测） |
| `kscreendraw(dev, screen)` | 全屏重绘（背景 + 所有对象） |
| `kscreenupdate(dev, screen)` | 增量刷新：合并脏区 → 重绘背景 → 标记受影响对象 → 重绘 → 清除脏区 |

### 内部工具函数

| 函数 | 说明 |
|---|---|
| `imgchange(src, dst, len, bit, colortable)` | 多色深解码（1/2/4-bit → RGB565），32 位宽写优化 |
| `memset_16(buf, size, pattern)` | RGB565 批量填充（指数复制） |

## 构建

根目录提供 `makefile`（需要 TDM-GCC-64 + EasyX）：

```bash
make             # 编译 → build/master.exe
make run         # 运行
make clean       # 清理
make DEBUG=1     # 调试构建（-O0 -g）
make reboot      # clean → build → run
make reboot-debug # clean → debug build → run
```

VSCode 用户：`Ctrl+Shift+B` 提供预配置的构建任务（make / make clean / make run / make reboot）。

## 移植指南

1. 在 `KSCconfig.h` 中将目标平台宏设为 `1`，其余设为 `0`
2. 实现 `KSCdisplay.h` 中的三个 HAL 函数：`screen_init`, `screen_setcanvas`, `screen_setcolorpixels`
3. `kscreenmount()` 会自动将 HAL 函数组装为 `k_draw_device` 并调用 `init()`
4. ARM 平台需额外实现 `TFTDriver.c` 中的底层 SPI/并行接口（参考现有 STM32F1 实现）
5. 如需 Flash 支持，为 PC 平台提供文件模拟（参考 `.data/w25q64_sim.bin`）

## 许可证

MIT — 详见 `LICENSE` 文件
