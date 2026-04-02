

# TFT TFT显示屏驱动库

这是一个通用TFT显示屏驱动库，提供了丰富的绘图功能，包括基本图形绘制、图像显示和文本渲染等。做到了与硬件配置分离，易于移植到不同的硬件平台。

## 功能特点

- 支持基本图形绘制（点、线、矩形、圆形、圆角矩形等）
- 支持图像显示（包括缩放功能）
- 支持文本显示（ASCII字符）
- 可选支持中文字符显示
- 支持自定义颜色设置
- 与硬件配置分离，易于移植到不同的STM32平台
- 支持命令行功能，便于调试和交互

## 硬件要求

- 任意微控制器（任意型号）
- TFT显示屏
- 适当的硬件连接（根据具体显示屏型号）

## 软件要求

- Keil MDK-ARM开发环境
- STM32CubeMX（用于生成基础代码）
- C语言编译器

## 目录结构

```
hardware/
├── examples/           # 示例代码
│   └── example.c       # 基本使用示例
├── inc/                # 头文件
│   ├── TFTDriver.h    # 硬件驱动头文件（根据具体硬件配置）
│   ├── KSCbasicdrawN.h # 基本绘图功能头文件
│   ├── KSCconfig.h     # 配置文件
│   ├── KSCdisplay.h    # 显示驱动头文件
│   ├── KSCfont.h       # 字体头文件
│   ├── KSCimg.h        # 图像头文件
│   └── CLI.h           # 命令行功能头文件
├── src/                # 源文件
│   ├── TFTDriver.c    # 硬件驱动实现（根据具体硬件配置）
│   ├── KSCbasicdrawN.c # 基本绘图功能实现
│   ├── KSCdisplay.c    # 显示驱动实现
│   ├── KSCfont.c       # 字体实现
│   └── KSCimg.c        # 图像实现
├── README.md           # 项目说明
├── LICENSE             # 许可证
```

## 快速开始

### 0. 核心定义

- `KSCCOLOR` - 颜色定义 （RGB格式565）
- `uintxy` - 坐标定义 （无符号16位整数）

```c
#include "KSCbasicdrawN.h"
```


### 1. 初始化屏幕

```c
KSC_buf* screen = kinitscreen(240, 320, NULL, 0, 0);
```

### 2. 设置画笔颜色

```c
ksetpencolor(screen, GREEN, 1);
```

### 3. 绘制图形

```c
// 绘制圆角矩形
kroundrect(screen, 20, 20, 50, 50, 10);

// 绘制填充圆形
kfillcircle(screen, 70, 40, 15);

// 绘制圆形边框
kcircle(screen, 100, 40, 10);

// 绘制缩放图像
kdrawimagebig(screen, 120, 40, Wechat, 16, 16, 4);
```

### 4. 显示文本

```c
ksetpencolor(screen, RED, 1);
kstring(screen, "Hello World", 10, 0);
```

## API参考

### 屏幕操作

- `KSC_buf* kinitscreen(uintxy width, uintxy height, uint8_t* Buffer, uintxy ssx, uintxy ssy)` - 初始化屏幕
- `void kfreescreen(KSC_buf* screen)` - 释放屏幕资源
- `KSC_buf* kgetscreen(void)` - 获取当前屏幕
- `KSC_mes ksetscreen(KSC_buf* screen)` - 设置当前屏幕

### 颜色操作

- `KSCCOLOR kgetpencolor(KSC_buf* screen, uint8_t pen)` - 获取画笔颜色
- `KSC_mes ksetpencolor(KSC_buf* screen, KSCCOLOR color, uint8_t pen)` - 设置画笔颜色

### 基本图形绘制

- `KSC_mes ksetpixel(KSC_buf* screen, uintxy x, uintxy y)` - 设置像素点
- `KSC_mes kline(KSC_buf* screen, uintxy x1, uintxy y1, uintxy x2, uintxy y2)` - 绘制任意方向线段
- `KSC_mes kfillrect(KSC_buf* screen, uintxy x1, uintxy y1, uintxy w, uintxy h)` - 填充矩形区域
- `KSC_mes karc(KSC_buf* screen, uintxy x0, uintxy y0, uint8_t r, uint8_t Anglediraction)` - 绘制圆弧
- `KSC_mes kcircle(KSC_buf* screen, uintxy x0, uintxy y0, uint8_t r)` - 绘制完整圆形
- `KSC_mes krect(KSC_buf* screen, uintxy x1, uintxy y1, uintxy w, uintxy h)` - 绘制矩形边框
- `KSC_mes kroundrect(KSC_buf* screen, uintxy x1, uintxy y1, uintxy w, uintxy h, uint8_t r)` - 绘制圆角矩形
- `KSC_mes kfillroundrect(KSC_buf* screen, uintxy x1, uintxy y1, uintxy w, uintxy h, uint8_t r)` - 绘制填充圆角矩形
- `KSC_mes kfillcircle(KSC_buf* screen, uintxy x0, uintxy y0, uint8_t r)` - 填充圆形区域

### 图像和文本

- `KSC_mes kdrawimage(KSC_buf* screen, uintxy x, uintxy y, const uint8_t* img, uint8_t width, uint8_t height)` - 绘制图像
- `KSC_mes kdrawimagebig(KSC_buf* screen, uintxy x, uintxy y, const uint8_t* img, uint8_t width, uint8_t height, uint8_t scale)` - 绘制缩放图像
- `KSC_mes kchar(KSC_buf* screen, char ch, uintxy x, uintxy y)` - 绘制单个字符
- `KSC_mes kstring(KSC_buf* screen, const char* str, uintxy x, uintxy y)` - 绘制字符串
- `KSC_mes kstringchinese(KSC_buf* screen, const char* str, uintxy x, uintxy y)` - 绘制中文字符串（需要定义__USE_CHINESE__）

### 命令行功能

- `void run_cli(char* str, cli_node* table)` - 执行命令
- `void set_run_cli_callback(uint8_t callback)` - 设置命令行回调状态
- `int get_run_cli_callback(void)` - 获取命令行回调状态
- `void run_cli_root(char* str)` - 执行根命令

#### 命令行数据结构

- `typedef int (*cmd_func)(char *argv)` - 命令函数类型
- `typedef struct cmd_node{ const char *name; cmd_func func; struct cmd_node *cmd_table; }cli_node` - 命令节点结构体

## 配置选项

在`KSCconfig.h`文件中可以配置以下选项：

- `__USE_CHINESE__` - 启用中文字符支持(暂未公布)

## 示例

查看`examples/example.c`文件了解基本使用方法。

## 移植指南

1. 根据你的硬件配置修改`TFTDriver.c`文件
2. 确保你的TFT显示屏硬件连线正确连接
3. 根据需要调整`KSCconfig.h`中的配置选项

## 贡献

欢迎提交问题和改进建议！

## 许可证

本项目采用MIT许可证，详见`LICENSE`文件。