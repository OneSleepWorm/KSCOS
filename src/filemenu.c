#include "../inc/filemenu.h"
#include "../inc/key.h"
#include "../littlefs/lfs.h"
#include "../littlefs/lfs_config.h"
#include "../inc/KSCdraw.h"

#include <stdio.h>
#include <string.h>

#define MAX_FILE_LIST 8
#define MAX_PATH_LEN  128

ksc_style_t menu_style0 = {
    .colorck = rred,
    .colorbk = wwhite,
    .width   = 80,
    .height  = 20,
    .state   = 0,
    .custom  = 0,
    .sdx     = 1,
    .sdy     = 1,
    .d_and_r = 0,
    ._type   = _string_extra | _custom_label1,
};

ksc_style_t menu_style1 = {
    .colorck = rred,
    .colorbk = wwhite,
    .width   = 80,
    .height  = 20,
    .state   = 0,
    .custom  = 0,
    .sdx     = 100,
    .sdy     = 1,
    .d_and_r = 0,
    ._type   = _string_extra | _custom_label2,
};

ksc_style_t menu_style2 = {
    .colorck = bblue,
    .colorbk = bblue,
    .width   = 80,
    .height  = 10,
    .state   = 0,
    .custom  = 0,
    .sdx     = 0,
    .sdy     = 0,
    .d_and_r = 0,
    ._type   = _box | _custom_label3,
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

static int filedir(ksc_menu_t* menu);

ksc_menu_t* filemenu_init(KSC_buf* screen) {
    static ksc_style_t* style[3] = {
        &menu_style2,
        &menu_style0,
        &menu_style1,
    };
    static ksc_menu_t filemenu = {
        .config = &filemenu_config,
        .list   = mfile,
        .style  = style,
    };

    if (screen == NULL) {
        screen = kgetscreen();
    }

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
            printf("filemenu: '%s' is not a directory\n",
                   menu->list[idx].name);
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
