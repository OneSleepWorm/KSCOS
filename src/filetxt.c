#include "../inc/filetxt.h"
#if KSC_TEXT_EDITOR
#include "../inc/KSCfont.h"
#include "../inc/key.h"

#include <stdio.h>
#include <string.h>

// ---- 布局常量 ----
#define BOX_WIDTH   100
#define BOX_HEIGHT  100
#define MARGIN      2

#define MAX_LINES   500
#define BUF_SIZE    256

// ---- 静态状态 ----
static KSC_window*   txt_screen = NULL;
static lfs_t*     txt_lfs = NULL;
static lfs_file_t txt_file;
static uintxy     txt_box_x, txt_box_y;
static int        txt_cur_line = 0;
static int        txt_total_lines = 0;
static int        txt_is_open = 0;

static lfs_soff_t txt_line_off[MAX_LINES];

static KSCCOLOR txt_fg = bblack;
static KSCCOLOR txt_bg = ggreen;

// ---- 预留文件过滤器 ----
static int (*txt_filter)(const char* path) = NULL;

void filetxt_set_filter(int (*filter)(const char* path)) {
    txt_filter = filter;
}

static int filetxt_can_open(const char* path) {
    if (txt_filter) return txt_filter(path);
    return 1;
}

// ---- 构建行偏移索引 ----
static int build_index(void) {
    char buf[BUF_SIZE];
    lfs_soff_t off = 0;
    int n = 1;
    txt_line_off[0] = 0;

    lfs_file_rewind(txt_lfs, &txt_file);

    lfs_ssize_t len;
    while ((len = lfs_file_read(txt_lfs, &txt_file, buf, sizeof(buf))) > 0) {
        for (lfs_ssize_t i = 0; i < len; i++) {
            if (buf[i] == '\n') {
                if (n >= MAX_LINES) goto done;
                txt_line_off[n++] = off + i + 1;
            }
        }
        off += len;
    }
done:
    return n;
}

// ---- 逐字符渲染一页 ----
static void render_page(void) {
    if (!txt_is_open || !txt_screen) return;

    KSC_Font1* f = &Systemfont0;
    int cw = f->width;
#if SYSTEMFONT == 7
    cw -= 1;
#endif
    int lh = f->height;

    uintxy x1 = txt_box_x + MARGIN;
    uintxy y1 = txt_box_y + MARGIN;
    uintxy x2 = txt_box_x + BOX_WIDTH - MARGIN;
    uintxy y2 = txt_box_y + BOX_HEIGHT - MARGIN;

    // 填充背景
    kfillbox(txt_screen, txt_bg, txt_box_x, txt_box_y, BOX_WIDTH, BOX_HEIGHT);

    if (txt_cur_line >= txt_total_lines) return;
    lfs_file_seek(txt_lfs, &txt_file, txt_line_off[txt_cur_line], LFS_SEEK_SET);

    uintxy cx = x1, cy = y1;
    char buf[BUF_SIZE];
    lfs_ssize_t len;
    int done = 0;

    while (!done) {
        len = lfs_file_read(txt_lfs, &txt_file, buf, sizeof(buf));
        if (len <= 0) break;

        for (lfs_ssize_t i = 0; i < len && !done; i++) {
            unsigned char c = (unsigned char)buf[i];

            if (c == '\n') {
                cx = x1;
                cy += lh;
                if (cy + lh > y2) done = 1;
            } else if (c == '\r') {
                // 跳过
            } else if (c < 0x80) {
                // ASCII — 自动换行检查
                if (cx + cw > x2) {
                    cx = x1;
                    cy += lh;
                    if (cy + lh > y2) { done = 1; break; }
                }
                kchar(txt_screen, (char)c, cx, cy, txt_fg, txt_bg);
                cx += cw;
            } else {
                // UTF-8 多字节字符: 判断长度
                int ulen;
                if      (c >= 0xF0) ulen = 4;
                else if (c >= 0xE0) ulen = 3;
                else if (c >= 0xC0) ulen = 2;
                else continue;  // 10xxxxxx 非法独立字节，跳过

                if (i + ulen > len) break;  // 跨缓冲区边界，安全退出

#if __USE_CHINESE__ > 0
                // TODO: 从 buf[i] 起 ulen 字节构成完整 UTF-8 序列
                // 调用中文渲染函数绘制一个汉字 (宽16高16)
                // cx += 16;
                // i += ulen - 1;
                // continue;
#endif
                // 中文未启用时: 跳过整个 UTF-8 序列，留空占位
                i += ulen - 1;

                if (cx + cw > x2) {
                    cx = x1;
                    cy += lh;
                    if (cy + lh > y2) { done = 1; break; }
                }
                cx += cw;
            }
        }
    }
}

// ---- 公开接口 ----
int filetxt_open(KSC_window* screen, lfs_t* lfs, const char* path, uintxy box_x, uintxy box_y) {
    if (!screen || !lfs || !path) return -1;
    if (!filetxt_can_open(path)) return -1;

    txt_screen   = screen;
    txt_lfs      = lfs;
    txt_box_x    = box_x;
    txt_box_y    = box_y;
    txt_cur_line = 0;

    int err = lfs_file_open(lfs, &txt_file, path, LFS_O_RDONLY);
    if (err < 0) {
        printf("filetxt: open '%s' failed: %d\n", path, err);
        return -1;
    }

    txt_total_lines = build_index();
    txt_is_open = 1;

    render_page();
    return 0;
}

int filetxt_update(uint8_t key) {
    if (!txt_is_open) return -1;

    int page_lines = (BOX_HEIGHT - MARGIN * 2) / Systemfont0.height;
    int changed = 0;

    switch (key) {
    case KEY_A8:  // UP — 上滚一行
        if (txt_cur_line > 0) { txt_cur_line--; changed = 1; }
        break;
    case KEY_A2:  // DOWN — 下滚一行
        if (txt_cur_line < txt_total_lines - 1) { txt_cur_line++; changed = 1; }
        break;
    case KEY_A6:  // RIGHT — 下翻一页
        if (txt_cur_line + page_lines < txt_total_lines) {
            txt_cur_line += page_lines;
            changed = 1;
        } else if (txt_cur_line < txt_total_lines - 1) {
            txt_cur_line = txt_total_lines - 1;
            changed = 1;
        }
        break;
    case KEY_A4:  // LEFT — 上翻一页
        if (txt_cur_line > 0) {
            txt_cur_line = (txt_cur_line >= page_lines) ? txt_cur_line - page_lines : 0;
            changed = 1;
        }
        break;
    case KEY_A7:  // 退出
        filetxt_close();
        return -1;
    }

    if (changed) render_page();
    return 0;
}

void filetxt_close(void) {
    if (txt_is_open) {
        lfs_file_close(txt_lfs, &txt_file);
        // 清除文本框区域
        if (txt_screen) {
            kfillbox(txt_screen, wwhite, txt_box_x, txt_box_y, BOX_WIDTH, BOX_HEIGHT);
        }
        txt_is_open = 0;
    }
}

int filetxt_is_active(void) {
    return txt_is_open;
}
#endif
