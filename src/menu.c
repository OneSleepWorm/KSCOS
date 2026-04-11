#include "menu.h"

#define MENU_ROWS   3
#define MENU_COLS   4
#define ITEM_COUNT  (MENU_ROWS * MENU_COLS)

#define LIGHTMENU 0xFFFF
#define DARKMENU 0x0000


/* 图标位图（顺序与 Systemimg 一致） */
static const uint8_t* icon_list[ITEM_COUNT] = {
    Wechat, QQ, Setting, Video,
    Photo, Tiktok, Qwen, Note,
    Game, Clock, Book, Alipay
};

/* 文字标签 */
static const char* label_list[ITEM_COUNT] = {
    "微信", "扣扣", "设置", "视频",
    "相册", "抖音", "通义", "笔记",
    "游戏", "时钟", "阅读", "支付宝"
};

static KSC_buf*     g_screen = NULL;
static MenuItem     g_items[ITEM_COUNT];
static int          g_sel_index = 0;

/* 获取指定索引菜单项的基准坐标（左上角） */
static void get_item_base(int index, uintxy* x, uintxy* y)
{
    uintxy total_w = MENU_COLS * COL_SPACING;
    uintxy total_h = MENU_ROWS * ROW_SPACING;
    uintxy start_x = (g_screen->width  - total_w) / 2;
    uintxy start_y = (g_screen->height - total_h) / 3;

    int row = index / MENU_COLS;
    int col = index % MENU_COLS;
    *x = start_x + col * COL_SPACING;
    *y = start_y + row * ROW_SPACING;
}

/* 绘制单个菜单项内容（图标+文字） */
static void draw_item_content(int index)
{
    uintxy base_x, base_y;
    get_item_base(index, &base_x, &base_y);

    const MenuItem* item = &g_items[index];

    /* 图标 */
    kdrawimage(g_screen,
               base_x + ICON_DX,
               base_y + ICON_DY,
               item->icon,
               16, 16);

    /* 文字 */
#if __USE_CHINESE__ > 0
    ksetpencolor(g_screen, DARKMENU, 1);
    ksetpencolor(g_screen, LIGHTMENU, 0);
    kstringchinese(g_screen,
                   item->label,
                   base_x + LABEL_DX,
                   base_y + LABEL_DY);
#else
    kstring(g_screen,
            item->label,
            base_x + LABEL_DX,
            base_y + LABEL_DY);
#endif
}

/* 绘制选中框（矩形边框） */
static void draw_border(int index, KSCCOLOR color)
{
    uintxy base_x, base_y;
    get_item_base(index, &base_x, &base_y);

    //KSCCOLOR old_pen1 = g_screen->pen0;
    ksetpencolor(g_screen, color, 1);

    krect(g_screen,
          base_x + BORDER_DX,
          base_y + BORDER_DY,
          BORDER_W,
          BORDER_H);

    // ksetpencolor(g_screen, old_pen1, 1);
}

/* 绘制所有菜单项内容 */
static void draw_all_contents(void)
{
    for (int i = 0; i < ITEM_COUNT; i++) {
        draw_item_content(i);
    }
}

/*==============================================================================
 * 公共接口
 *============================================================================*/

void menu_init(KSC_buf* screen)
{
    if (!screen) return;
    g_screen = screen;

    /* 初始化结构体数组 */
    for (int i = 0; i < ITEM_COUNT; i++) {
        g_items[i].label = label_list[i];
        g_items[i].icon  = icon_list[i];
    }

    /* 设置画笔：背景黑色，前景白色 */
    ksetpencolor(g_screen, LIGHTMENU, 1);
    ksetpencolor(g_screen, DARKMENU, 0);

    /* 清屏 */
    kfillrect(g_screen, 0, 0, g_screen->width, g_screen->height);

    /* 绘制所有菜单内容 */
    draw_all_contents();

    /* 绘制初始选中框（蓝色） */
    g_sel_index = 0;
    draw_border(g_sel_index, BLUE);
}

void menu_move(MenuDirection dir)
{
    if (!g_screen) return;

    int cur_row = g_sel_index / MENU_COLS;
    int cur_col = g_sel_index % MENU_COLS;
    int new_row = cur_row;
    int new_col = cur_col;

    switch (dir) {
        case MENU_DIR_UP:    new_row--; break;
        case MENU_DIR_DOWN:  new_row++; break;
        case MENU_DIR_LEFT:  new_col--; break;
        case MENU_DIR_RIGHT: new_col++; break;
        default: return;
    }

    /* 边界检查 */
    if (new_row < 0 || new_row >= MENU_ROWS) return;
    if (new_col < 0 || new_col >= MENU_COLS) return;

    int new_index = new_row * MENU_COLS + new_col;

    /* 擦除旧边框 */
    draw_border(g_sel_index, LIGHTMENU);

    /* 更新选中索引并绘制新边框 */
    g_sel_index = new_index;
    draw_border(g_sel_index, BLUE);
}