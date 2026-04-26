#ifndef __KSCBASEICDRAWN_FAST_H__
#define __KSCBASEICDRAWN_FAST_H__

#include "KSCconfig.h"

#include "CLI.h"
#include "string.h"
#include "W25Q64.h"

typedef void (*cmd_handler_t)(void);
typedef struct {
    const char *name;
    cmd_handler_t handler;
} cmd_t;

void list_cmds(void);
#if __USE_ESP32__  
#include <stdio.h>
// 注册宏，指定的段名必须是一个合法的 C 标识符（不能有点号或减号）
// 习惯用 .section_name 或直接字母数字，但为兼容自动生成的符号，最好别用点开头的名字
#define REGISTER_CMD(name, func) \
    static const cmd_t _cmd_##func \
    __attribute__((section("cmd_table"), used)) = { name, func }

// 自动生成的符号是 __start_cmd_table 和 __stop_cmd_table


#elif __USE_PC__

#define REGISTER_CMD(name, func) \
    static const cmd_t _cmd_##func \
    __attribute__((section("cmd_table"), used)) = { name, func }

#elif __USE_ARMCC__
// 声明来自链接脚本的符号

// 注册宏
#define REGISTER_CMD(name, func) \
    static const cmd_t _cmd_##func \
    __attribute__((section(".cmd_table"), used)) = { name, func }

#if 0
 .rodata :
  {
    ...
  } >FLASH

  /* 添加命令行注册段，放在 FLASH 里 */
  .cmd_table :
  {
    __start_cmd_table = .;         /* 段起始地址 */
    KEEP(*(.cmd_table))            /* 把汇编/编译出来的 .cmd_table 段内容都保留在这里 */
    __stop_cmd_table = .;           /* 段结束地址 */
  } >FLASH
#endif
#endif

extern const cmd_t __start_cmd_table ;
extern const cmd_t __stop_cmd_table ;

#if  __USE_LCD__ ==1
#include "KSCdraw.h"

//int kinitscreen_fast(char* arg);
// int kfreescreen_fast(char* arg);
// int ksetcolor_fast(char* arg);
// int kpixel_fast(char* arg);
// int kline_fast(char* arg);
// int krect_fast(char* arg);
// int kcircle_fast(char* arg);
// int kfillrect_fast(char* arg);
// int kfillcircle_fast(char* arg);
// int kstring_fast(char* arg);
// int kstringchinese_fast(char* arg);
int drawhelp(char* arg);
// int kimage_fast(char* arg);

extern cli_node cmd_draw_table[];

#endif

#if __USE_FLASH__ ==1
int kflashinit_fast(char* arg);
int kflashdeinit_fast(char* arg);
int kflashwrite_fast(char* arg);
int kflashread_fast(char* arg);
int kflasherase_fast(char* arg);
int kflashhelp(char* arg);
extern cli_node cmd_flash_table[];
#endif

#if __USE_UART__ ==2
int kconnecthelp(char* arg);
int kconnectinitdeinit_fast(char* arg);
int kconnectsendstr_fast(char* arg);
int kconnectsendnum_fast(char* arg);
int kconnectsenddata_fast(char* arg);
int kconnectrecvdata_fast(char* arg);
extern cli_node cmd_connect_table[];
#endif

#if __USE_LITTLEFS__ ==1
#include "../littlefs/lfs.h"
#include "../littlefs/lfs_config.h"
#include "../inc/file.h"

#define LFS_MAX_PATH 256
#define LFS_READ_BUF_SIZE 512

typedef struct {
    lfs_t lfs;
    const struct lfs_config *cfg;
    char cwd[LFS_MAX_PATH];
    uint8_t mounted;
} k_lfs_t;

extern k_lfs_t k_lfs;

int klfs_init_fast(char* arg);
int klfs_deinit_fast(char* arg);
int klfs_format_fast(char* arg);
int klfsls_fast(char* arg);
int klfscd_fast(char* arg);
int klfspwd_fast(char* arg);
int klfsmkdir_fast(char* arg);
int klfstouch_fast(char* arg);
int klfsrm_fast(char* arg);
int klfscat_fast(char* arg);
int klfscp_fast(char* arg);
int klfshelp_fast(char* arg);

extern cli_node cmd_lfs_table[];

#endif

#endif