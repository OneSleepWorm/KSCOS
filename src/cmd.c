#include "../inc/cmd.h"

// static void __placeholder_handler(void) {}
// REGISTER_CMD("", __placeholder_handler);  // 空字符串，理论上用户不会输入空行
void list_cmds(void) {
    const cmd_t *p = &__start_cmd_table;
    const cmd_t *end = &__stop_cmd_table;
    if(p == NULL || end == NULL){
        printf("No commands registered.\n");
        return;
    }
    printf("PATH:\n");
    while (p < end) {
        printf("%s\n", p->name);
        p++;
    }
}
#include <string.h>

#define MAX_ARGS 8
#define MAX_ARG_LENGTH 16
/**
 * 切割命令字符串函数
 * @param str 输入的命令字符串
 * @param argc 分割后的字符串数量（输出参数）
 * @param argv 分割后的字符串数组（输出参数）
 * @return 成功返回0，失败返回-1
 */
int split_command_string(char* str, int* argc, char*** argv) {
    static char* arg_array[MAX_ARGS + 1];

    int length = strlen(str);
    if (length == 0) {
        *argc = 0;
        *argv = NULL;
        return -2;
    }

    for (int i = 0; i < MAX_ARGS; i++) {
        arg_array[i] = NULL;
    }
    arg_array[MAX_ARGS] = NULL;

    char* s = (char*)str;
    int in_quote = 0;
    int arg_index = 0;

    while (*s) {
        if (*s == '"') {
            *s = '\0';
            in_quote = !in_quote;
            s++;
            continue;
        }

        if (!in_quote && (*s == ' ' || *s == '\t' || *s == '\n')) {
            *s = '\0';
            s++;
            continue;
        }

        if (arg_index >= MAX_ARGS) {
            printf("Error: Command line exceeds maximum arguments.\n");
            return -1;
        }
        arg_array[arg_index++] = s;

        while (*s && (in_quote || (*s != ' ' && *s != '\t' && *s != '\n'))) {
            if (*s == '"') {
                *s = '\0';
                in_quote = !in_quote;
                s++;
            } else {
                s++;
            }
        }
    }

    *argc = arg_index;
    *argv = arg_array;
    return 0;
}



void system_cmd(int* argc, char** argv){
    printf("system reveice: ");
    for(int i = 0; i < *argc; i++){
        printf("%s ", argv[i]);
    }
}
REGISTER_CMD("system", system_cmd);

extern const struct lfs_config cfg;
#include "../littlefs/lfs.h"
#include "../littlefs/lfs_config.h"

#define FILE_LFS_NAME_MAX 255

static lfs_t file_lfs;
static uint8_t file_lfs_mounted = 0;
static char file_lfs_cwd[FILE_LFS_NAME_MAX + 2] = "/";

static int file_ensure_mount(void) {
    if (file_lfs_mounted) {
        return 0;
    }
    int err = lfs_mount(&file_lfs, &cfg);
    if (err < 0) {
        printf("file: mount failed %d\r\n", err);
        return err;
    }
    file_lfs_mounted = 1;
    return 0;
}

static void file_resolve_path(char* dest, const char* path) {
    if (path == NULL || path[0] == 0) {
        strcpy(dest, file_lfs_cwd);
        return;
    }
    if (path[0] == '/') {
        strncpy(dest, path, FILE_LFS_NAME_MAX);
        dest[FILE_LFS_NAME_MAX] = 0;
        return;
    }
    if (strcmp(file_lfs_cwd, "/") == 0) {
        snprintf(dest, FILE_LFS_NAME_MAX + 1, "/%s", path);
    } else {
        snprintf(dest, FILE_LFS_NAME_MAX + 1, "%s/%s", file_lfs_cwd, path);
    }
}

void ls_cmd(int* argc, char** argv) {
    char path[FILE_LFS_NAME_MAX + 2];
    const char* target = (*argc > 1) ? argv[1] : NULL;
    file_resolve_path(path, target);

    if (file_ensure_mount() < 0) {
        return;
    }

    lfs_dir_t dir;
    int err = lfs_dir_open(&file_lfs, &dir, path);
    if (err < 0) {
        printf("ls: cannot open '%s' %d\r\n", path, err);
        return;
    }

    struct lfs_info info;
    printf("  %-24s %8s\r\n", "name", "size");
    printf("--------------------------\r\n");
    while (lfs_dir_read(&file_lfs, &dir, &info) > 0) {
        if (info.name[0] == 0) continue;
        if (strcmp(info.name, ".") == 0) continue;
        if (strcmp(info.name, "..") == 0) continue;
        if (info.type == LFS_TYPE_DIR) {
            printf("  %-24s %8s\r\n", info.name, "<DIR>");
        } else {
            printf("  %-24s %8d\r\n", info.name, (int)info.size);
        }
    }
    lfs_dir_close(&file_lfs, &dir);
}
REGISTER_CMD("ls", ls_cmd);

void cd_cmd(int* argc, char** argv) {
    if (file_ensure_mount() < 0) {
        return;
    }

    char path[FILE_LFS_NAME_MAX + 2];
    if (*argc > 1) {
        file_resolve_path(path, argv[1]);
    } else {
        strcpy(path, "/");
    }

    lfs_dir_t dir;
    int err = lfs_dir_open(&file_lfs, &dir, path);
    if (err < 0) {
        printf("cd: '%s' not found\r\n", path);
        return;
    }
    lfs_dir_close(&file_lfs, &dir);

    strncpy(file_lfs_cwd, path, FILE_LFS_NAME_MAX);
    file_lfs_cwd[FILE_LFS_NAME_MAX] = 0;
}
REGISTER_CMD("cd", cd_cmd);

void pwd_cmd(int* argc, char** argv) {
    (void)argc;
    (void)argv;
    printf("%s\r\n", file_lfs_cwd);
}
REGISTER_CMD("pwd", pwd_cmd);

void mkdir_cmd(int* argc, char** argv) {
    if (file_ensure_mount() < 0) {
        return;
    }

    if (*argc < 2) {
        printf("mkdir: missing path\r\n");
        return;
    }

    char path[FILE_LFS_NAME_MAX + 2];
    file_resolve_path(path, argv[1]);

    int err = lfs_mkdir(&file_lfs, path);
    if (err < 0) {
        printf("mkdir: failed %d\r\n", err);
        return;
    }
    printf("mkdir: created dir '%s'\r\n", path);
}
REGISTER_CMD("mkdir", mkdir_cmd);

void touch_cmd(int* argc, char** argv) {
    if (file_ensure_mount() < 0) {
        return;
    }

    if (*argc < 2) {
        printf("touch: missing path\r\n");
        return;
    }

    char path[FILE_LFS_NAME_MAX + 2];
    file_resolve_path(path, argv[1]);

    lfs_file_t file;
    int err = lfs_file_open(&file_lfs, &file, path, LFS_O_WRONLY | LFS_O_CREAT);
    if (err < 0) {
        printf("touch: failed %d\r\n", err);
        return;
    }
    lfs_file_close(&file_lfs, &file);
    printf("touch: created file '%s'\r\n", path);
}
REGISTER_CMD("touch", touch_cmd);

void rm_cmd(int* argc, char** argv) {
    if (file_ensure_mount() < 0) {
        return;
    }

    if (*argc < 2) {
        printf("rm: missing path\r\n");
        return;
    }

    char path[FILE_LFS_NAME_MAX + 2];
    file_resolve_path(path, argv[1]);

    int err = lfs_remove(&file_lfs, path);
    if (err < 0) {
        printf("rm: failed %d\r\n", err);
        return;
    }
    printf("rm: removed '%s'\r\n", path);
}
REGISTER_CMD("rm", rm_cmd);

void cp_cmd(int* argc, char** argv) {
    if (file_ensure_mount() < 0) {
        return;
    }

    if (*argc < 3) {
        printf("cp: usage: cp <src> <dst>\r\n");
        return;
    }

    char src[FILE_LFS_NAME_MAX + 2];
    char dst[FILE_LFS_NAME_MAX + 2];
    file_resolve_path(src, argv[1]);
    file_resolve_path(dst, argv[2]);

    lfs_file_t src_file;
    int err = lfs_file_open(&file_lfs, &src_file, src, LFS_O_RDONLY);
    if (err < 0) {
        printf("cp: cannot open src '%s' %d\r\n", src, err);
        return;
    }

    lfs_file_t dst_file;
    err = lfs_file_open(&file_lfs, &dst_file, dst, LFS_O_WRONLY | LFS_O_CREAT | LFS_O_TRUNC);
    if (err < 0) {
        printf("cp: cannot open dst '%s' %d\r\n", dst, err);
        lfs_file_close(&file_lfs, &src_file);
        return;
    }

    char buf[64];
    lfs_ssize_t read;
    while ((read = lfs_file_read(&file_lfs, &src_file, buf, sizeof(buf))) > 0) {
        lfs_ssize_t wrote = lfs_file_write(&file_lfs, &dst_file, buf, read);
        if (wrote < 0) {
            printf("cp: write failed %d\r\n", (int)wrote);
            lfs_file_close(&file_lfs, &src_file);
            lfs_file_close(&file_lfs, &dst_file);
            return;
        }
    }

    lfs_file_close(&file_lfs, &src_file);
    lfs_file_close(&file_lfs, &dst_file);
    printf("cp: copied '%s' -> '%s'\r\n", src, dst);
}
REGISTER_CMD("cp", cp_cmd);

void mv_cmd(int* argc, char** argv) {
    if (file_ensure_mount() < 0) {
        return;
    }

    if (*argc < 3) {
        printf("mv: usage: mv <src> <dst>\r\n");
        return;
    }

    char src[FILE_LFS_NAME_MAX + 2];
    char dst[FILE_LFS_NAME_MAX + 2];
    file_resolve_path(src, argv[1]);
    file_resolve_path(dst, argv[2]);

    int err = lfs_rename(&file_lfs, src, dst);
    if (err < 0) {
        printf("mv: failed %d\r\n", err);
        return;
    }
    printf("mv: '%s' -> '%s'\r\n", src, dst);
}
REGISTER_CMD("mv", mv_cmd);

void df_cmd(int* argc, char** argv) {
    (void)argc;
    (void)argv;

    if (file_ensure_mount() < 0) {
        return;
    }

    struct lfs_fsinfo info;
    int err = lfs_fs_stat(&file_lfs, &info);
    if (err < 0) {
        printf("df: cannot get fs info %d\r\n", err);
        return;
    }

    lfs_ssize_t used_blocks = lfs_fs_size(&file_lfs);
    if (used_blocks < 0) {
        printf("df: cannot get used size %d\r\n", (int)used_blocks);
        return;
    }

    lfs_size_t total = info.block_size * info.block_count;
    lfs_size_t used = info.block_size * (lfs_size_t)used_blocks;
    lfs_size_t free = total - used;

    printf("  total: %lu bytes\r\n", (unsigned long)total);
    printf("  used:  %lu bytes\r\n", (unsigned long)used);
    printf("  free:  %lu bytes\r\n", (unsigned long)free);
}
REGISTER_CMD("df", df_cmd);

void cat_cmd(int* argc, char** argv) {
    if (*argc < 2) {
        printf("cat: missing path\r\n");
        return;
    }

    if (file_ensure_mount() < 0) {
        return;
    }

    char path[FILE_LFS_NAME_MAX + 2];
    file_resolve_path(path, argv[1]);

    lfs_file_t file;
    int err = lfs_file_open(&file_lfs, &file, path, LFS_O_RDONLY);
    if (err < 0) {
        printf("cat: cannot open '%s' %d\r\n", path, err);
        return;
    }

    char buf[64];
    lfs_ssize_t read;
    while ((read = lfs_file_read(&file_lfs, &file, buf, sizeof(buf))) > 0) {
        for (lfs_ssize_t i = 0; i < read; i++) {
            printf("%c", buf[i]);
        }
    }
    printf("\r\n");
    lfs_file_close(&file_lfs, &file);
}
REGISTER_CMD("cat", cat_cmd);

void echo_cmd(int* argc, char** argv) {
    if (*argc < 4) {
        printf("echo: usage: echo [-n] \"text\" > file\r\n");
        return;
    }

    if (file_ensure_mount() < 0) {
        return;
    }

    int no_newline = 0;
    int content_idx = 1;
    if (strcmp(argv[1], "-n") == 0) {
        no_newline = 1;
        content_idx = 2;
        if (*argc < 5) {
            printf("echo: usage: echo [-n] \"text\" > file\r\n");
            return;
        }
    }

    int append = 0;
    const char* redirect = argv[*argc - 2];
    if (strcmp(redirect, ">>") == 0) {
        append = 1;
    } else if (strcmp(redirect, ">") == 0) {
        append = 0;
    } else {
        printf("echo: expected > or >> before file path\r\n");
        return;
    }

    char path[FILE_LFS_NAME_MAX + 2];
    file_resolve_path(path, argv[*argc - 1]);

    int flags = LFS_O_WRONLY | LFS_O_CREAT;
    if (append) {
        flags |= LFS_O_APPEND;
    } else {
        flags |= LFS_O_TRUNC;
    }

    lfs_file_t file;
    int err = lfs_file_open(&file_lfs, &file, path, flags);
    if (err < 0) {
        printf("echo: cannot open '%s' %d\r\n", path, err);
        return;
    }

    const char* content = argv[content_idx];
    lfs_ssize_t written = lfs_file_write(&file_lfs, &file, content, strlen(content));
    if (!no_newline) {
        lfs_file_write(&file_lfs, &file, "\n", 1);
    }
    lfs_file_close(&file_lfs, &file);

    if (written < 0) {
        printf("echo: write failed %d\r\n", (int)written);
    }
}
REGISTER_CMD("echo", echo_cmd);

void run_command(char* cmd){
    int argc = 0;
    char** argv = NULL;

    if(split_command_string(cmd, &argc, &argv) != 0){
        printf("split_command_string failed.\n");
        return;
    }

    if (argc == 0) {
        return;
    }

    const cmd_t *p = &__start_cmd_table;
    const cmd_t *end = &__stop_cmd_table;
    while (p < end) {
        if (strcmp(p->name, argv[0]) == 0) {
            p->handler(&argc, argv);
            return;
        }
        p++;
    }
    printf("unknown command: %s\r\n", argv[0]);
}

void show_dir(void){
    printf("c:%s ", file_lfs_cwd);
}
