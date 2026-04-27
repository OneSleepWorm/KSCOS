#ifndef __CMD_H__
#define __CMD_H__

#include "../inc/KSCconfig.h"


typedef int (*cmd_handler_t)(int* argc, char** argv);
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

void list_cmds(void);
int run_command(const char* cmd);
void show_dir(void);
#endif
