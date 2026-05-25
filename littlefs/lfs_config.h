
// lfs_config.h
#ifndef LFS_CONFIG_H
#define LFS_CONFIG_H

// 使用自定义的配置结构体，而不是默认的
#define LFS_CONFIG_USE_CUSTOM_CONFIG 1

extern const struct lfs_config cfg;
// 硬件配置宏
#define LFS_READ_SIZE      256  // 匹配W25Q64的页大小
#define LFS_PROG_SIZE      256  // 匹配W25Q64的页大小
#define LFS_BLOCK_SIZE     4096 // 匹配W25Q64的扇区大小（擦除单位）
#define LFS_CACHE_SIZE     512  // 性能与内存的平衡选择
#define LFS_BLOCK_CYCLES   500  // 磨损均衡周期
#define LFS_LOOKAHEAD_SIZE 512  // 用于分配查找的缓冲区大小

#endif
