#include "../inc/filemenu.h"
#include "../inc/filetxt.h"
#include "../inc/key.h"
#include "../littlefs/lfs.h"
#include "../littlefs/lfs_config.h"
#include "../inc/KSCdraw.h"

#include <stdio.h>
#include <string.h>

#define MAX_FILE_LIST 8
#define MAX_PATH_LEN  128

//文件名样式
ksc_style_t menu_style0 = {
    .colorck = rred,
    .colorbk = wwhite,
    .width   = 80,
    .height  = 10,
    .state   = 0,
    .custom  = 0,
    .sdx     = 1,
    .sdy     = 1,
    .d_and_r = 0,
    ._type   = _string_extra,
};
//文件类型样式
ksc_style_t menu_style1 = {
    .colorck = rred,
    .colorbk = wwhite,
    .width   = 80,
    .height  = 10,
    .state   = 0,
    .custom  = 0,
    .sdx     = 100,
    .sdy     = 1,
    .d_and_r = 0,
    ._type   = _string_extra,
};
//文件框样式
ksc_style_t menu_style2 = {
    .colorck = bblue,
    .colorbk = wwhite,
    .width   = 80,
    .height  = 10,
    .state   = 0,
    .custom  = 0,
    .sdx     = 0,
    .sdy     = 0,
    .d_and_r = 0,
    ._type   = _box,
};

ksc_menu_config_t filemenu_config = {
    .mdw          = 150,
    .mdh          = 10,
    .menu_hnum    = 8,
    .menu_wnum    = 1,
    .menu_obj_num = 1,
    .menu_index   = 0,
};

Img_File mfile[MAX_FILE_LIST] = {0};

lfs_t     lfs;
lfs_dir_t dir;
static KSC_window* menu_screen = NULL;

static int filedir(ksc_menu_t* menu);

ksc_menu_t* filemenu_init(KSC_window* screen) {
    if(menu_screen != NULL){
        return NULL;
    }
    static ksc_style_t* style[3] = {
        &menu_style0,
        &menu_style1,
        &menu_style2,
    };
    static ksc_menu_t filemenu = {
        .config = &filemenu_config,
        .list   = mfile,
        .style  = style,
    };

    if (screen == NULL) {
        screen = kgetscreen();
    }
    menu_screen = screen;

    filemenu.config->menu_obj_num = sizeof(style) / sizeof(style[0]);

    int err = lfs_mount(&lfs, &cfg);
    if (err < 0) {
        printf("filemenu: lfs_mount failed: %d\n", err);
        filemenu.config->menu_num = 0;
        return &filemenu;
    }

    err = lfs_dir_open(&lfs, &dir, "/");
    if (err < 0) {
        printf("filemenu: lfs_dir_open / failed: %d\n", err);
        filemenu.config->menu_num = 0;
        return &filemenu;
    }

    filedir(&filemenu);
    return &filemenu;
}

static int filedir(ksc_menu_t* menu) {
    uint8_t num = 0;
    struct lfs_info info;
    int ret;

    while ((ret = lfs_dir_read(&lfs, &dir, &info)) > 0) {
        if (num >= MAX_FILE_LIST) {
            printf("filemenu: too many entries (max %d), truncating\n",
                   MAX_FILE_LIST);
            break;
        }

        strncpy(menu->list[num].name, info.name,
                sizeof(menu->list[num].name) - 1);
        menu->list[num].name[sizeof(menu->list[num].name) - 1] = '\0';
        menu->list[num].size = info.size;
        menu->list[num].type = info.type;
        menu->list[num].data = NULL;
        num++;
    }

    if (ret < 0) {
        printf("filemenu: lfs_dir_read error: %d\n", ret);
    }

    menu->config->menu_num   = num;
    menu->config->menu_index = 0;
    return num;
}

#define KEY_UP    KEY_A8
#define KEY_DOWN  KEY_A2
#define KEY_LEFT  KEY_A4
#define KEY_RIGHT KEY_A6
#define KEY_ENTER KEY_A5

static int path_append(char *path, size_t path_size, const char *name) {
    size_t path_len = strlen(path);
    size_t name_len = strlen(name);

    if (path_len + 1 + name_len >= path_size) {
        printf("filemenu: path too long\n");
        return -1;
    }

    if (path[path_len - 1] != '/') {
        strcat(path, "/");
    }
    strcat(path, name);
    return 0;
}

static void path_parent(char *path) {
    char *p = strrchr(path, '/');
    if (p) {
        *p = '\0';
    }
    if (path[0] == '\0') {
        path[0] = '/';
        path[1] = '\0';
    }
}

static int reopen_dir(const char *path) {
    int ret = lfs_dir_open(&lfs, &dir, path);
    if (ret < 0) {
        printf("filemenu: cannot open dir '%s': %d\n", path, ret);
    }
    return ret;
}

int filemenu_update(ksc_menu_t* menu, uint8_t key) {
    static char path[MAX_PATH_LEN] = "/";
    int ret = 0;

    if (menu == NULL || menu->config == NULL) return -1;

    if (key == KEY_RIGHT || key == KEY_ENTER) {
        uint8_t idx = menu->config->menu_index;

        if (menu->config->menu_num == 0) return 0;
        if (idx >= menu->config->menu_num) {
            printf("filemenu: menu_index %d out of range (max %d)\n",
                   idx, menu->config->menu_num - 1);
            return 0;
        }

        const char* name = menu->list[idx].name;

        if (name[0] == '.' && name[1] == '\0') {
            // "." — 当前目录，仅刷新
            printf("filemenu: refresh current dir '%s'\n", path);
            lfs_dir_close(&lfs, &dir);
            reopen_dir(path);
            filedir(menu);
            return 0;
        }

        if (name[0] == '.' && name[1] == '.' && name[2] == '\0') {
            // ".." — 上级目录，模拟 KEY_LEFT
            if (strcmp(path, "/") == 0) return 0;
            lfs_dir_close(&lfs, &dir);
            path_parent(path);
            ret = reopen_dir(path);
            if (ret < 0) {
                strcpy(path, "/");
                reopen_dir(path);
            }
            filedir(menu);
            printf("path:%s\n", path);
            return ret;
        }

        if (menu->list[idx].type != LFS_TYPE_DIR) {
            // 进入文本查看器
            char fullpath[MAX_PATH_LEN * 2];
            if (strcmp(path, "/") == 0) {
                snprintf(fullpath, sizeof(fullpath), "/%s", menu->list[idx].name);
            } else {
                snprintf(fullpath, sizeof(fullpath), "%s/%s", path, menu->list[idx].name);
            }

            if (filetxt_open(menu_screen, &lfs, fullpath, 30, 30) == 0) {
                while (filetxt_is_active()) {
                    uint8_t k = key_scan();
                    if (k != KEY_NONE) filetxt_update(k);
                }
                // 退出文本查看器后恢复菜单显示
                kmenu_draw(menu_screen, menu);
            }
            return 0;
        }

        char saved_path[MAX_PATH_LEN];
        strncpy(saved_path, path, MAX_PATH_LEN - 1);
        saved_path[MAX_PATH_LEN - 1] = '\0';

        if (path_append(path, MAX_PATH_LEN, menu->list[idx].name) != 0) {
            return 0;
        }

        lfs_dir_close(&lfs, &dir);
        ret = reopen_dir(path);
        if (ret < 0) {
            printf("filemenu: reverting to '%s'\n", saved_path);
            strncpy(path, saved_path, MAX_PATH_LEN - 1);
            path[MAX_PATH_LEN - 1] = '\0';
            reopen_dir(path);
            return ret;
        }

        filedir(menu);

    } else if (key == KEY_LEFT) {
        if (strcmp(path, "/") == 0) {
            return 0;
        }

        lfs_dir_close(&lfs, &dir);
        path_parent(path);

        ret = reopen_dir(path);
        if (ret < 0) {
            // Fallback to root
            strcpy(path, "/");
            reopen_dir(path);
        }

        filedir(menu);

    } else if (key == KEY_UP) {
        if (menu->config->menu_index == 0) return 0;
        menu->config->menu_index--;

    } else if (key == KEY_DOWN) {
        if (menu->config->menu_num == 0) return 0;
        if (menu->config->menu_index >= menu->config->menu_num - 1) return 0;
        menu->config->menu_index++;
    }

    printf("path:%s\n", path);
    return ret;
}
void kmenu_clear(KSC_window* screen, ksc_menu_t* menu) {
    if(menu == NULL || menu->config == NULL){
        return;
    }
    KSCCOLOR ncolorbk = screen->bk;
    printf("ncolorbk:%04x\n",ncolorbk);
    //覆盖旧的菜单框
    kfull(screen,ncolorbk,
        0,0,screen->width,screen->height
    );
}

void kmenu_draw(KSC_window* screen, ksc_menu_t* menu) {
    if(menu == NULL || menu->config == NULL){
        return;
    }
    KSCCOLOR ncolorck,ncolorbk;
    uint8_t nheight;
    uint8_t nsx,nsy;
    // 绘制菜单文件名
    ncolorck = menu->style[0]->colorck;
    ncolorbk = menu->style[0]->colorbk;
    // nwidth = menu->style[0]->width;
    nheight = menu->style[0]->height;
    nsx = menu->style[0]->sdx;
    nsy = menu->style[0]->sdy;
    for(uint8_t i=0;i<menu->config->menu_num;i++){
        kstring(screen,menu->list[i].name,nsx,nsy+i*nheight,ncolorck,ncolorbk);
    }
    // 绘制菜单类型
    ncolorck = menu->style[1]->colorck;
    ncolorbk = menu->style[1]->colorbk;
    // nwidth = menu->style[1]->width;
    nheight = menu->style[1]->height;
    nsx = menu->style[1]->sdx;
    nsy = menu->style[1]->sdy;
    for(uint8_t i=0;i<menu->config->menu_num;i++){
        kstring(screen,menu->list[i].type==LFS_TYPE_DIR?"DIR":"FILE"
            ,nsx,nsy+i*nheight,ncolorck,ncolorbk);
    }
    
    nsy = menu->style[2]->sdy+menu->config->mdh*menu->config->menu_index;
    //绘制新的菜单框
    kbox(screen,menu->style[2]->colorck
        ,menu->style[2]->sdx
        ,nsy
        ,menu->style[2]->width
        ,menu->style[2]->height
    );
}
void kmenu_update(KSC_window* screen, ksc_menu_t* menu){
    uint8_t key = key_scan();
    if(key == KEY_NONE) return;
    uint8_t nsy =menu->style[2]->sdy+menu->config->mdh*menu->config->menu_index;
    //覆盖旧的菜单框
    kbox(screen,menu->style[2]->colorbk
        ,menu->style[2]->sdx
        ,nsy
        ,menu->style[2]->width
        ,menu->style[2]->height
    );
    //
    filemenu_update(menu, key);
    if(key != KEY_UP && key != KEY_DOWN) {
        kmenu_clear(screen,menu);
        kmenu_draw(screen,menu);
    }
    nsy = menu->style[2]->sdy+menu->config->mdh*menu->config->menu_index;
    //绘制新的菜单框
    kbox(screen,menu->style[2]->colorck
        ,menu->style[2]->sdx
        ,nsy
        ,menu->style[2]->width
        ,menu->style[2]->height
    );
}



// void filemenu_flashscreen(KSC_window* screen, ksc_menu_t* menu,uint8_t key, uint8_t x, uint8_t y) {
//     KSC_menu_clear(screen,menu,10,10);
//     kmenu_update(screen,menu,key);
//     kmenudraw(screen,menu);
// }

