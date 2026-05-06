#ifndef __KSCBASEICDRAWN_FAST_H__
#define __KSCBASEICDRAWN_FAST_H__

#include "KSCconfig.h"
#include "cmd.h"
#include "string.h"
#include "W25Q64.h"

#if  __USE_LCD__ ==1
#include "KSCdraw.h"

int drawhelp(char* arg);

#endif

#if __USE_FLASH__ ==1
int kflashinit_fast(char* arg);
int kflashdeinit_fast(char* arg);
int kflashwrite_fast(char* arg);
int kflashread_fast(char* arg);
int kflasherase_fast(char* arg);
int kflashhelp(char* arg);
#endif

#if __USE_UART__ ==2
int kconnecthelp(char* arg);
int kconnectinitdeinit_fast(char* arg);
int kconnectsendstr_fast(char* arg);
int kconnectsendnum_fast(char* arg);
int kconnectsenddata_fast(char* arg);
int kconnectrecvdata_fast(char* arg);
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

#endif

#endif
