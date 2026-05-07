#ifndef __FILETXT_H
#define __FILETXT_H

#include "KSCdraw.h"
#include "../littlefs/lfs.h"

// 打开文本文件并进入查看模式
// screen: 屏幕缓冲区
// lfs: 已 mount 的 littlefs 实例
// path: 文件完整路径
// box_x, box_y: 文本框左上角坐标
// 返回 0 成功, -1 失败
int filetxt_open(KSC_window* screen, lfs_t* lfs, const char* path, uintxy box_x, uintxy box_y);

// 处理按键，返回 0 继续, -1 已退出
int filetxt_update(uint8_t key);

// 关闭文件并清理状态
void filetxt_close(void);

// 查询是否处于文本查看状态
int filetxt_is_active(void);

#endif
