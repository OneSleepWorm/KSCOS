# 项目协议说明
1. 本项目 **src/ 目录下所有自研代码：MIT License（见根目录LICENSE）**
2. third_party/stm32/ 内 STM32 HAL库源码：版权归STMicroelectronics，遵循 **BSD 3-Clause License**，协议文件位于 third_party/stm32/LICENSE
3. 分发本项目时，第三方HAL库需保留原有BSD协议与版权信息。

# KSCdraw — 跨平台 TFT 显示屏绘图库

通用 TFT 显示屏驱动库，提供基本图形绘制、图像显示、文本渲染、对象系统、文件菜单和命令行等功能。  
通过预处理器宏切换平台（PC / ARM / ESP32），硬件依赖与绘图 API 分离。

## 功能特点

- 基本图形：点、线、矩形、圆形、圆角矩形、圆弧、填充图形
- 图像显示：直接绘制、二值化绘制、缩放绘制
- 文本渲染：ASCII 字符（7×7 / 8×16），可选中文支持
- **对象系统** (`KSCobj`)：统一 `ksc_obj_t` 描述所有可绘制元素，支持批量绘制
- **文件菜单** (`filemenu`)：基于对象系统的文件浏览器，支持 littlefs
- **命令行系统** (`cmd`)：`REGISTER_CMD` 宏 + GCC linker section 注册命令
- **设备驱动抽象** (`k_draw_device`)：绘图 API 通过函数指针接口操作硬件，与具体平台解耦
- **输入抽象** (`INPUT_FUNC`)：可插拔的按键输入源
- **多平台**：PC (EasyX) 开发 / ARM (Keil) 部署 / ESP32 部署

## 快速开始

### 0. 核心类型

```c
KSCCOLOR  // RGB565 颜色 (uint16_t)
uintxy    // 坐标 (uint16_t)
KSC_window // 屏幕描述结构体
k_draw_device // 设备驱动接口
```

### 1. 定义设备驱动并挂载

```c
#include "KSCdraw/inc/KSCdraw.h"

k_draw_device dev = {
    .init = screen_init,
    .setcanvas = screen_setcanvas,
    .setcolorpixels = screen_setcolorpixels,
};

kscreenmount(&dev);
```

### 2. 初始化屏幕

```c
KSC_window screen = {
    .ssx = 0,
    .ssy = 0,
    .width = 240,
    .height = 320,
    .bk = wwhite,
};
// 或动态分配
KSC_window* screen = kscreeninit(&dev, 0, 0, 240, 320, wwhite);
```

### 3. 绘制图形

```c
// 填充矩形 — 颜色作为第一个参数传入
kfull(&dev, &screen, bblue, 0, 0, 100, 100);
// 圆角矩形
kroundrect(&dev, &screen, rred, 20, 20, 50, 50, 10);
// 填充圆形
kfillcircle(&dev, &screen, ggreen, 70, 40, 15);
// 圆形边框
kcircle(&dev, &screen, rred, 100, 40, 10);
// 缩放图像
kdrawimagebig(&dev, &screen, Wechat, 120, 40, 16, 16, 4);
```

### 4. 显示文本

```c
kstring(&dev, &screen, "Hello World", 10, 0, rred, wwhite);
```

### 5. 文件菜单

```c
key_init();
ksc_menu_t* menu = filemenu_init(&dev, &screen);
kmenu_draw(&dev, &screen, menu);
while (1) {
    kmenu_update(&dev, &screen, menu);
}
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
| `KEY_QUEUE_SIZE` | 输入队列大小 |
| `__BUTTON_SIMU__` | PC 模拟按键模式 |
| `__LITTLE_END_COLOR__` | RGB565 字节序 |
| `SYSTEMFONT` | ASCII 字体宽度（7 或 8） |

## 目录结构

```
KSCdraw/
├── inc/                 # 头文件
│   ├── KSCconfig.h      # 全局配置宏
│   ├── KSCdraw.h        # 核心绘图 API 声明
│   ├── KSCdisplay.h     # 显示驱动接口
│   ├── KSCobj.h         # 对象系统定义
│   ├── KSCimg.h         # 图像数据定义
│   ├── KSCfont.h        # 字体接口
│   ├── key.h            # 按键输入抽象
│   ├── cmd.h            # 命令行系统
│   ├── filemenu.h       # 文件菜单
│   ├── filetxt.h        # 文本查看器
│   ├── W25Q64.h         # SPI Flash 驱动
│   ├── Serial.h         # 串口 I/O
│   └── main.h           # 主程序头文件聚合
├── src/                 # 源文件
│   ├── KSCdraw.c        # 核心绘图实现
│   ├── KSCdisplay.cpp   # 显示 HAL（PC 用 EasyX）
│   ├── KSCobj.c         # 对象系统实现
│   ├── KSCfont.c        # 字体渲染
│   ├── KSCimg.c         # 图像处理（含图标数据）
│   ├── key.cpp          # 按键输入实现
│   ├── cmd.c            # 命令行实现
│   ├── filemenu.c       # 文件菜单实现
│   ├── filetxt.c        # 文本查看器
│   ├── config.c         # 运行时可配参数
│   ├── Serial.c         # 串口输出
│   ├── TFTDriver.c      # 底层 TFT 驱动
│   ├── W25Q64.c         # SPI Flash 驱动
│   └── UTF8_FlashN.c    # UTF-8 工具
├── littlefs/            # littlefs 嵌入式文件系统（移植版）
├── examples/            # 使用示例
│   ├── flash/
│   ├── key/
│   ├── littlefs/
│   └── obj/
├── .data/               # 测试资源数据
├── LICENSE              # MIT 许可证
└── README.md
```

## API 参考

### 设备 & 屏幕管理

| 函数 | 说明 |
|---|---|
| `kscreenmount(dev)` | 挂载设备驱动（调用 `dev->init()`） |
| `kscreeninit(dev, ssx, ssy, w, h, bk)` | 创建并初始化屏幕，返回 `KSC_window*` |
| `kscreenfree(dev, screen)` | 释放屏幕资源 |

### 基本图形（全部需传 `dev` 和 `screen`）

| 函数 | 说明 |
|---|---|
| `ksetpixel(dev, screen, color, x, y)` | 设置单像素 |
| `kfull(dev, screen, color, x1, y1, w, h)` | 填充矩形区域 |
| `kline(dev, screen, color, x1, y1, x2, y2)` | 任意方向线段（自动检测水平/垂直走快速路径） |
| `kbox(dev, screen, color, x1, y1, w, h)` | 矩形边框 |
| `kfillbox(dev, screen, color, x1, y1, w, h)` | 填充矩形（等价于 `kfull`） |
| `kcircle(dev, screen, color, x0, y0, r)` | 圆形边框 |
| `kfillcircle(dev, screen, color, x0, y0, r)` | 填充圆形 |
| `karc(dev, screen, color, x0, y0, r, dir)` | 圆弧（按象限选择） |
| `kroundrect(dev, screen, color, x1, y1, w, h, r)` | 圆角矩形边框 |
| `kfillroundrect(dev, screen, color, x1, y1, w, h, r)` | 填充圆角矩形 |

### 图像 & 文本

| 函数 | 说明 |
|---|---|
| `kdrawimage(dev, screen, img, x, y, w, h)` | 绘制 RGB565 图像 |
| `kdrawimagebig(dev, screen, img, x, y, w, h, scale)` | 缩放绘制图像 |
| `kimagebin(dev, screen, img, x, y, w, h, ck, cbk)` | 绘制二值化图像 |
| `kchar(dev, screen, ch, x, y, ck, cbk)` | 绘制单个 ASCII 字符 |
| `kstring(dev, screen, str, x, y, ck, cbk)` | 绘制 ASCII 字符串 |
| `kstringchinese(dev, screen, str, x, y, c1, c2)` | 绘制中文字符串（需 `__USE_CHINESE__`） |

### 对象系统

| 函数 | 说明 |
|---|---|
| `kobjdraw(dev, screen, obj)` | 绘制单个 `ksc_obj_t` |
| `kobjsdraw(dev, screen, obj_array, num)` | 批量绘制对象数组 |
| `kwindowdraw(dev, screen)` | 绘制窗口内所有对象 |

对象类型：`_circle`, `_box`, `_string`, `_image`, `_roundrect`, `_fillroundrect`, `_fillbox`, `_fillcircle`, `_line`, `_imagebig`

### 文件菜单

| 函数 | 说明 |
|---|---|
| `filemenu_init(dev, screen)` | 初始化文件菜单（基于 littlefs） |
| `kmenu_draw(dev, screen, menu)` | 绘制菜单 |
| `kmenu_update(dev, screen, menu)` | 更新菜单（处理输入） |

### 命令行

使用 `REGISTER_CMD(name, handler)` 注册命令，通过 linker section 自动收集：

```c
REGISTER_CMD("ls", cmd_ls);
```

运行时调用 `run_command(cmd_str)` 执行。

### 按键输入

```c
key_init();          // 初始化
uint8_t key_scan();  // 扫描单次按键
uint8_t key_read();  // 读取带保持状态的按键
input_t* input_get(); // 获取输入队列（供 INPUT_FUNC 使用）
```

`input_t` 和 `INPUT_FUNC` 允许菜单等模块通过函数指针解耦输入源。

## 构建

根目录提供 `makefile`（TDM-GCC-64 + EasyX）：

```bash
make        # 编译
make run    # 运行
make clean  # 清理
```

## 移植指南

1. 在 `KSCconfig.h` 中将对应平台宏设为 `1`，其余设为 `0`
2. 实现 `KSCdisplay.h` 中的三个 HAL 函数：`screen_init`, `screen_setcanvas`, `screen_setcolorpixels`
3. 创建 `k_draw_device` 实例并将上述函数指针填入
4. ARM/ESP32 平台需额外实现 `TFTDriver.c` 中的底层 SPI/并行接口

## 许可证

MIT — 详见 `LICENSE` 文件
