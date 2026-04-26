#include "../littlefs/lfs.h"
#include "../littlefs/lfs_config.h"
#include "../inc/W25Q64.h"
extern const struct lfs_config cfg;
void littlefs_test(void) {
    W25Q64_Init();
    lfs_t lfs;
    int err;

    // 1. 挂载文件系统
    err = lfs_mount(&lfs, &cfg);
    if (err) {
        printf("Mount failed (err=%d), formatting...\n", err);
        err = lfs_format(&lfs, &cfg);
        if (err) {
            printf("Format failed (err=%d)\n", err);
            return;
        }
        err = lfs_mount(&lfs, &cfg);
        if (err) {
            printf("Remount failed (err=%d)\n", err);
            return;
        }
    }
    printf("LittleFS mounted successfully.\n");

    // 2. 创建并写入文件
    lfs_file_t file;
    const char *write_data = "Hello from LittleFS (standard C version)!";
    err = lfs_file_open(&lfs, &file, "/test.txt", LFS_O_WRONLY | LFS_O_CREAT);
    if (err) {
        printf("Failed to open file for writing: %d\n", err);
        lfs_unmount(&lfs);
        return;
    }
    lfs_ssize_t written = lfs_file_write(&lfs, &file, write_data, strlen(write_data));
    if (written < 0) {
        printf("Write failed: %d\n", (int)written);
    } else {
        printf("Wrote %d bytes to /test.txt\n", (int)written);
    }
    lfs_file_close(&lfs, &file);

    // 3. 读取文件并校验
    char read_buf[128] = {0};
    err = lfs_file_open(&lfs, &file, "/test.txt", LFS_O_RDONLY);
    if (err) {
        printf("Failed to open file for reading: %d\n", err);
        lfs_unmount(&lfs);
        return;
    }
    lfs_ssize_t read_len = lfs_file_read(&lfs, &file, read_buf, sizeof(read_buf) - 1);
    if (read_len < 0) {
        printf("Read failed: %d\n", (int)read_len);
    } else {
        printf("Read %d bytes: %s\n", (int)read_len, read_buf);
        if (read_len == (lfs_ssize_t)strlen(write_data) &&
            memcmp(write_data, read_buf, read_len) == 0) {
            printf("Data verification PASSED\n");
        } else {
            printf("Data verification FAILED\n");
        }
    }
    lfs_file_close(&lfs, &file);

    // 4. 获取文件信息
    struct lfs_info info;
    if (lfs_stat(&lfs, "/test.txt", &info) == 0) {
        printf("File: %s, size: %d bytes\n", info.name, info.size);
    }

    // 5. 列出根目录内容
    lfs_dir_t dir;
    printf("Root directory listing:\n");
    err = lfs_dir_open(&lfs, &dir, "/");
    if (err == 0) {
        while (lfs_dir_read(&lfs, &dir, &info) > 0) {
            printf("  %s %s\n", 
                   info.type == LFS_TYPE_REG ? "FILE" : "DIR",
                   info.name);
        }
        lfs_dir_close(&lfs, &dir);
    } else {
        printf("Failed to open root dir: %d\n", err);
    }

    // 6. 卸载文件系统
    lfs_unmount(&lfs);
    printf("LittleFS test finished.\n");
}
