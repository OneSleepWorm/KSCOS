#ifndef __KSCBASEICDRAWN_FAST_H__
#define __KSCBASEICDRAWN_FAST_H__   

#include "KSCbasicdrawN.h"
#include "KSCconfig.h"
#include "KSCdisplay.h"
#include <string.h>
#include "../inc/CLI.h"

//KSCbasicdrawN_fast命令
int kinitscreen_fast(char* arg);
int kfreescreen_fast(char* arg);
int ksetcolor_fast(char* arg);
int kpixel_fast(char* arg);
int kline_fast(char* arg);
int krect_fast(char* arg);
int kcircle_fast(char* arg);
int kfillrect_fast(char* arg);
int kfillcircle_fast(char* arg);
int kstring_fast(char* arg);
int kstring_fast(char* arg);
int kstringchinese_fast(char* arg);
int drawhelp(char* arg);
int kimage_fast(char* arg);

// 声明cmd_draw_table为外部变量
extern cli_node cmd_draw_table[];

#if __USE_FLASH__ ==1
//flash烧写数据命令
int kflashinit_fast(char* arg);
int kflashdeinit_fast(char* arg);
int kflashwrite_fast(char* arg);
int kflashread_fast(char* arg);
int kflasherase_fast(char* arg);
int kflashhelp(char* arg);
extern cli_node cmd_flash_table[];
#endif

#if __USE_UART__ ==2
//connect命令行处理
int kconnecthelp(char* arg);
int kconnectinitdeinit_fast(char* arg);
int kconnectsendstr_fast(char* arg);
int kconnectsendnum_fast(char* arg);
int kconnectsenddata_fast(char* arg);
int kconnectrecvdata_fast(char* arg);
int kconnecthelp(char* arg);
extern cli_node cmd_connect_table[];
#endif

#endif
