// lfs_port.c
#include "lfs.h"
#include "lfs_config.h"
#include "stdio.h"  // 确保路径正确，这个头文件里有你的flash_read等函数声明
#include "../inc/W25Q64.h"

// 定义W25Q64的总容量 (64Mbit = 8MB)
#define FLASH_TOTAL_SIZE (64 * 1024 * 1024) // 单位：字节
#define FLASH_BLOCK_SIZE 4096

// 实现读操作：从W25Q64的block块，偏移off处，读取size字节到buffer
int lfs_port_read(const struct lfs_config *c, lfs_block_t block,
                  lfs_off_t off, void *buffer, lfs_size_t size) {
    uint32_t phys_addr = block * FLASH_BLOCK_SIZE + off;
    flash_read(phys_addr, (uint8_t*)buffer, size);
    return 0; // 成功返回0
}

// 实现写操作：将buffer中的size字节写入W25Q64
int lfs_port_prog(const struct lfs_config *c, lfs_block_t block,
                  lfs_off_t off, const void *buffer, lfs_size_t size) {
    uint32_t phys_addr = block * FLASH_BLOCK_SIZE + off;
    flash_write(phys_addr, (uint8_t*)buffer, size);
    return 0;
}

// 实现擦除操作：擦除W25Q64的指定block
int lfs_port_erase(const struct lfs_config *c, lfs_block_t block) {
    uint32_t phys_addr = block * FLASH_BLOCK_SIZE;
    flash_clear(phys_addr);
    return 0;
}

// 同步操作：如果flash_write已保证数据落盘，此函数可为空
int lfs_port_sync(const struct lfs_config *c) {
    return 0;
}

// LittleFS的配置结构体，运行时填充
const struct lfs_config cfg = {
    // 硬件接口函数
    .read  = lfs_port_read,
    .prog  = lfs_port_prog,
    .erase = lfs_port_erase,
    .sync  = lfs_port_sync,

    // 硬件参数（来自lfs_config.h的宏定义）
    .read_size   = LFS_READ_SIZE,
    .prog_size   = LFS_PROG_SIZE,
    .block_size  = LFS_BLOCK_SIZE,
    .block_count = FLASH_TOTAL_SIZE / FLASH_BLOCK_SIZE,
    .cache_size  = LFS_CACHE_SIZE,
    .lookahead_size = LFS_LOOKAHEAD_SIZE,
    .block_cycles = LFS_BLOCK_CYCLES,
};