#ifndef _MENU_H_
#define _MENU_H_

#include "KSCbasicdrawN.h"
#include "KSCimg.h"

/* 菜单项窗口尺寸 */
#define ITEM_W      40
#define ITEM_H      60

/* 内部元素相对偏移 */
#define ICON_DX     2
#define ICON_DY     2
#define LABEL_DX    2
#define LABEL_DY    40

/* 矩形框偏移与尺寸（与窗口相同，框住整个菜单项） */
#define BORDER_DX   0
#define BORDER_DY   0
#define BORDER_W    ITEM_W
#define BORDER_H    ITEM_H

/* 行列间距 */
#define COL_SPACING 42
#define ROW_SPACING 62

/* 移动方向枚举 */
typedef enum {
    MENU_DIR_UP = 2,
    MENU_DIR_DOWN =10,
    MENU_DIR_LEFT =5,
    MENU_DIR_RIGHT =7
} MenuDirection;

/* 菜单项数据结构 */
typedef struct {
    const char*     label;      /* 文字标签 */
    const uint8_t*  icon;       /* 图标位图指针 */
} MenuItem;

void menu_init(KSC_buf* screen);
void menu_move(MenuDirection dir);

#endif