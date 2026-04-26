#include "../inc/KSCbasicdrawN_fast.h"
#include "../inc/Serial.h"
//--locale=english --no-multibyte-chars

#define MAX_ARGV 8
#define success kprintf("\n")
//#define success __nop()


static void __placeholder_handler(void) {}
REGISTER_CMD("", __placeholder_handler);  // 空字符串，理论上用户不会输入空行
void list_cmds(void) {
    const cmd_t *p = &__start_cmd_table;
    const cmd_t *end = &__stop_cmd_table;
    if(p == NULL || end == NULL){
        printf("No commands registered.\n");
        return;
    }
    while (p < end) {
        printf("Cmd: %s\n", p->name);
        p++;
    }
}

#include "stdio.h"

char** cutargv(char* arg){
    //kprintf("argv: %s\r\n", arg);
    if(arg == NULL){
        return NULL;
    }
    static char* argv[MAX_ARGV]={NULL};
    memset(argv, 0, sizeof(argv));
    char* p = strtok(arg," ");
    //kprintf("cutargv: %s\r\n", p);
    int argc = 0;
    while(p != NULL&&argc < MAX_ARGV){
        argv[argc++] = p;
        //kprintf("argv[%d]: %s\r\n", argc-1, argv[argc-1]);
        p = strtok(NULL," ");
    }
    // 添加边界检查，确保不会越界
    if(argc < MAX_ARGV){
        argv[argc] = NULL;
    } else {
        argv[MAX_ARGV-1] = NULL;
    }
    return argv;
} 
#if __USE_LCD__ > 0
#include "../inc/KSCimg.h"
#include "../inc/KSCdraw.h"

//匹配字符串是否为有效字符串，返回0或KSCCOLOR
KSCCOLOR kturncolor(char* str){
    KSCCOLOR color = 0;
    if(str == NULL){
        return color;
    }
    if(strcmp(str,"green") == 0){
        color = ggreen;
    }else if(strcmp(str,"red") == 0){
        color = rred;
    }else if(strcmp(str,"blue") == 0){
        color = bblue;
    }else if(strcmp(str,"black") == 0){
        color = bblack;
    }else if(strcmp(str,"white") == 0){
        color = wwhite;
    }else {
        color = 0;
    }
    return color;
}



int kinitscreen_fast(char* arg){
    char** argv = cutargv(arg);
    if(argv == NULL){
        kprintf("initscreen: 语法错误\r\n");
        return 1;
    }
    int width = atoi(argv[0]);
    int height = atoi(argv[1]);
    kprintf("initscreen: %d %d\r\n",width,height);
    if(width <= 0 || height <= 0){
        kprintf("initscreen: 宽度和高度必须大于0\r\n");
        return 1;
    }
    screen_init();
    kinitscreen(width,height,0,0);
    return 0;
}

int kfreescreen_fast(char* arg){
    kfreescreen(kgetscreen());
    return 0;
}


int kline_fast(char* arg){
    char** argv = cutargv(arg);
    if(argv == NULL){
        kprintf("line: 语法错误\r\n");
        return 1;
    }
    int x1 = atoi(argv[0]);
    int y1 = atoi(argv[1]);
    int x2 = atoi(argv[2]);
    int y2 = atoi(argv[3]);
    KSCCOLOR color = kturncolor(argv[4]);
    kline(kgetscreen(),color,x1,y1,x2,y2);
	success;
    return 0;
}
int krect_fast(char* arg){
    char** argv = cutargv(arg);
    if(argv == NULL){
        kprintf("rect: 语法错误\r\n");
        return 1;
    }
    int x1 = atoi(argv[0]);
    int y1 = atoi(argv[1]);
    int w = atoi(argv[2]);
    int h = atoi(argv[3]);
    KSCCOLOR color = kturncolor(argv[4]);
    kbox(kgetscreen(),color,x1,y1,w,h);
	success;
    return 0;
}

int kcircle_fast(char* arg){
    char** argv = cutargv(arg);
    if(argv == NULL){
        kprintf("circle: 语法错误\r\n");
        return 1;
    }
    int x0 = atoi(argv[0]);
    int y0 = atoi(argv[1]);
    int r = atoi(argv[2]);
    KSCCOLOR color = kturncolor(argv[3]);
    kcircle(kgetscreen(),color,x0,y0,r);
	success;
    return 0;
}

int kfillrect_fast(char* arg){
    char** argv = cutargv(arg);
    if(argv == NULL){
        kprintf("fillrect: 缺少参数\r\n");
        return 1;
    }
    int x1 = atoi(argv[0]);
    int y1 = atoi(argv[1]);
    int w = atoi(argv[2]);
    int h = atoi(argv[3]);
    KSCCOLOR color = kturncolor(argv[4]);
    //kfillrect(kgetscreen(),color,x1,y1,w,h);
	success;
	
    return 0;
}

int kfillcircle_fast(char* arg){
    char** argv = cutargv(arg);
    if(argv == NULL){
        kprintf("fillcircle: 语法错误\r\n");
        return 1;
    }
    int x0 = atoi(argv[0]);
    int y0 = atoi(argv[1]);
    int r = atoi(argv[2]);
    KSCCOLOR color = kturncolor(argv[3]);
    kfillcircle(kgetscreen(),color,x0,y0,r);
	success;
    return 0;
}

int kstring_fast(char* arg){
    char** argv = cutargv(arg);
    if(argv == NULL){
        kprintf("string: 语法错误\r\n");
        return 1;
    }
    char* str = argv[0];
    int x = atoi(argv[1]);
    int y = atoi(argv[2]);
    KSCCOLOR colorck = kturncolor(argv[3]);
    KSCCOLOR colorbk = kturncolor(argv[4]);
    kstring(kgetscreen(),str,x,y,colorck,colorbk);
	success;
    return 0;
}
#if __USE_CHINESE__ == 1
int kstringchinese_fast(char* arg){
    char** argv = cutargv(arg);
    if(argv == NULL){
        kprintf("stringchinese: 语法错误\r\n");
        return 1;
    }
    char* str = argv[0];
    int x = atoi(argv[1]);
    int y = atoi(argv[2]);
    KSCCOLOR color = kturncolor(argv[3]);
    kstringchinese(kgetscreen(),color,str,x,y);
	success;
    return 0;
}
#endif
int kimage_fast(char* arg){
    char** argv = cutargv(arg);
    if(argv == NULL){
        kprintf("image: 语法错误\r\n");
        return 1;
    }
    char* filename = argv[0];
    Img_File img = kloadimage(filename);
    if(img.name == NULL){
        kprintf("image: 图片不存在\r\n");
        return 1;
    }
    int x = atoi(argv[1]);
    int y = atoi(argv[2]);
    //kdrawimage(kgetscreen(),img.data,x,y,img.width,img.height);
    return 0;
}
int kimagebig_fast(char* arg){
    char** argv = cutargv(arg);
    if(argv == NULL){
        kprintf("image: 语法错误\r\n");
        return 1;
    }
    char* filename = argv[0];
    Img_File img = kloadimage(filename);
    if(img.name == NULL){
        kprintf("image: 图片不存在\r\n");
        return 1;
    }
    int x = atoi(argv[1]);
    int y = atoi(argv[2]);
    int scale =atoi(argv[3]);
    //kdrawimagebig(kgetscreen(),x,y,img.data,img.width,img.height,scale);
    success;
    return 0;
}


int drawhelp(char* arg){
    kprintf("draw cli runner\r\n");
    kprintf("Usage:\r\n");
    kprintf("draw <command> [args]\r\n");
    kprintf("Available commands:\r\n");
    kprintf("initscreen <width> <height>  初始化屏幕\r\n");
    kprintf("freescreen  释放屏幕缓冲区\r\n");
    kprintf("line <x1> <y1> <x2> <y2> <color>  绘制线\r\n");
    kprintf("rect <x1> <y1> <w> <h> <color>  绘制矩形\r\n");
    kprintf("circle <x> <y> <r> <color>  绘制圆\r\n");
    kprintf("fillcircle <x> <y> <r> <color>  填充圆\r\n");
    kprintf("fillrect <x1> <y1> <w> <h> <color>  填充矩形\r\n");
    kprintf("fillcircle <x> <y> <r> <color>  填充圆\r\n");
    kprintf("string <str> <x> <y> <color>  绘制字符串\r\n");
    kprintf("stringchinese <str> <x> <y> <color>  绘制中文字符串(测试版)\r\n");
    kprintf("image <filename> <x> <y> <scale>  绘制图片\r\n");
    kprintf("imagebig <filename> <x> <y> <scale>  绘制图片(缩放)\r\n");
    kprintf("help  显示帮助信息\r\n");
    kprintf("-------------------------------------------\r\n");
    success;
    return 0;
}

// 定义cmd_draw_table数组
cli_node cmd_draw_table[] = {
    {"initscreen",kinitscreen_fast,NULL},
    {"freescreen",kfreescreen_fast,NULL},
    {"line",kline_fast,NULL},
    {"rect",krect_fast,NULL},
    {"circle",kcircle_fast,NULL},
    {"fillrect",kfillrect_fast,NULL},
    {"fillcircle",kfillcircle_fast,NULL},
    {"string",kstring_fast,NULL},
#if __USE_CHINESE__ == 1
    {"stringchinese",kstringchinese_fast,NULL},
#endif
	{"image",kimage_fast,NULL},
    {"imagebig",kimagebig_fast,NULL},
    {"help",drawhelp,NULL},
    {NULL,NULL,NULL}
};
#endif


#if __USE_FLASH__ > 0
//flash命令行处理
#include "../inc/W25Q64.h"

int kflashinit_fast(char* arg){
    flash_init();
    success;
    return 0;
}
int kflashdeinit_fast(char* arg){
    flash_deinit();
    success;
    return 0;
}
int kflashwrite_fast(char* arg){
    static uint32_t addr = 0;
    static uint32_t count = 0;
        char** argv = cutargv(arg);
        if(argv == NULL){
            kprintf("write: 语法错误\r\n");
            return 1;
        }
        addr = strtol(argv[0],NULL,16);
        count = atoi(argv[1]);
        uint8_t* pdata =NULL;
        uint16_t readycount =0;
        //切成每块小于等于FLASH_BUFFER_SIZE(256)的数据块
        for(int i=0;count>0;i++){
            if(count>MAX_FLASH_WRITE_SIZE){
                readycount = MAX_FLASH_WRITE_SIZE;
            }else{
                readycount = count;
            }
            kprintf("All data:%d,readycount:%d\r\n",count,readycount);
            success;
            for(int timer=0;timer<2000;timer++){
                HAL_Delay(1);
                if((pdata =kfgetc_s(readycount))!=NULL){
                    break;
                }
                if(timer==1999){
                kprintf("kfgetc_s timeout\r\n");
                return 1;
                }
            }
            flash_writedata(addr, pdata,
                 readycount);
            count -=readycount;
            addr +=readycount;
            if(i%100==0){
                ledturn();
            }
            success;
        }
        success;
        return 0;
}

int kflashread_fast(char* arg){
    //ledturn();
    char** argv = cutargv(arg);
    if(argv == NULL){
        kprintf("read: 语法错误\r\n");
        return 1;
    }
    
    
    uint32_t addr = strtol(argv[0],NULL,16);
    int count = atoi(argv[1]);
    flash_readdata(addr, publicdatabuf, count);
    for(int i=0;i<count;i++){
        kprintf("%02X ", publicdatabuf[i]);
    }
    success;
    return 0;
}
int kflashclear_fast(char* arg){
    char** argv = cutargv(arg);
    if(argv == NULL){
        kprintf("clear: 语法错误\r\n");
        return 1;
    }
    int addr = strtol(argv[0],NULL,16);
    kprintf("addr=%06x\r\n",addr);
    flash_clear(addr);
    success;
    return 0;
}

int kflashhelp(char* arg){
    kprintf("flash cli runner\r\n");
    kprintf("Usage:\r\n");
    kprintf("flash <command> [args]\r\n");
    kprintf("Available commands:\r\n");
    kprintf("init  初始化flash\r\n");
    kprintf("deinit  释放flash\r\n");
    kprintf("write <addr> <count>  向flash写入数据\r\n");
    kprintf("read <addr> <count>  从flash读取数据\r\n");
    kprintf("clear <addr>  扇区擦除\r\n");
    kprintf("-------------------------------------------\r\n");
    success;
    return 0;
}

cli_node cmd_flash_table[] = {
    {"init",kflashinit_fast,NULL},
    {"deinit",kflashdeinit_fast,NULL},
    {"write",kflashwrite_fast,NULL},
    {"read",kflashread_fast,NULL},
    {"clear",kflashclear_fast,NULL},
    {"help",kflashhelp,NULL},
    {NULL,NULL,NULL}
};
#endif

#if __USE_LITTLEFS__ > 0
#include "../littlefs/lfs.h"
#include "../littlefs/lfs_config.h"

k_lfs_t k_lfs = {
    .mounted = 0,
    .cwd = "/"
};

static int lfs_resolve_path(char *dest, const char *path) {
    if (path == NULL) {
        strcpy(dest, k_lfs.cwd);
        return 0;
    }
    if (path[0] == '/') {
        strncpy(dest, path, LFS_MAX_PATH - 1);
        dest[LFS_MAX_PATH - 1] = 0;
    } else {
        if (strcmp(k_lfs.cwd, "/") == 0) {
            snprintf(dest, LFS_MAX_PATH, "/%s", path);
        } else {
            snprintf(dest, LFS_MAX_PATH, "%s/%s", k_lfs.cwd, path);
        }
    }
    char *p = dest;
    char *q = dest;
    while (*p) {
        if (*p == '/' && *(p+1) == '/') {
            p++;
        } else if (*p == '/' && *(p+1) == '.' && *(p+2) == '/') {
            p += 3;
        } else if (*p == '/' && *(p+1) == '.' && *(p+2) == 0) {
            p += 2;
        } else {
            *q++ = *p++;
        }
    }
    *q = 0;
    if (strlen(dest) > 1 && dest[strlen(dest)-1] == '/') {
        dest[strlen(dest)-1] = 0;
    }
    if (strlen(dest) == 0) {
        strcpy(dest, "/");
    }
    return 0;
}

int klfs_init_fast(char* arg) {
    (void)arg;
    if (k_lfs.mounted) {
        kprintf("lfs: already mounted\r\n");
        return 0;
    }
    int err = lfs_mount(&k_lfs.lfs, k_lfs.cfg);
    if (err < 0) {
        kprintf("lfs: mount failed %d\r\n", err);
        return 1;
    }
    k_lfs.mounted = 1;
    strcpy(k_lfs.cwd, "/");
    kprintf("lfs: mounted\r\n");
    success;
    return 0;
}

int klfs_deinit_fast(char* arg) {
    (void)arg;
    if (!k_lfs.mounted) {
        kprintf("lfs: not mounted\r\n");
        return 0;
    }
    lfs_unmount(&k_lfs.lfs);
    k_lfs.mounted = 0;
    kprintf("lfs: unmounted\r\n");
    success;
    return 0;
}

int klfs_format_fast(char* arg) {
    (void)arg;
    if (k_lfs.mounted) {
        kprintf("lfs: unmount first\r\n");
        return 1;
    }
    int err = lfs_format(&k_lfs.lfs, k_lfs.cfg);
    if (err < 0) {
        kprintf("lfs: format failed %d\r\n", err);
        return 1;
    }
    kprintf("lfs: formatted\r\n");
    success;
    return 0;
}

int klfsls_fast(char* arg) {
    char path[LFS_MAX_PATH];
    lfs_resolve_path(path, arg);

    if (!k_lfs.mounted) {
        kprintf("lfs: not mounted\r\n");
        return 1;
    }

    lfs_dir_t dir;
    int err = lfs_dir_open(&k_lfs.lfs, &dir, path);
    if (err < 0) {
        kprintf("lfs ls: cannot open '%s' %d\r\n", path, err);
        return 1;
    }

    struct lfs_info info;
    kprintf("  %-24s %8s\r\n", "name", "size");
    kprintf("--------------------------\r\n");
    while (lfs_dir_read(&k_lfs.lfs, &dir, &info) > 0) {
        if (info.name[0] == 0) continue;
        if (info.type == LFS_TYPE_DIR) {
            kprintf("  %-24s %8s\r\n", info.name, "<DIR>");
        } else {
            kprintf("  %-24s %8d\r\n", info.name, (int)info.size);
        }
    }
    lfs_dir_close(&k_lfs.lfs, &dir);
    success;
    return 0;
}

int klfscd_fast(char* arg) {
    char path[LFS_MAX_PATH];
    char **argv = cutargv(arg);

    if (!k_lfs.mounted) {
        kprintf("lfs: not mounted\r\n");
        return 1;
    }

    if (argv == NULL || argv[0] == NULL) {
        strcpy(k_lfs.cwd, "/");
        kprintf("lfs: cwd -> /\r\n");
        return 0;
    }

    lfs_resolve_path(path, argv[0]);

    lfs_dir_t dir;
    int err = lfs_dir_open(&k_lfs.lfs, &dir, path);
    if (err < 0) {
        kprintf("lfs cd: '%s' not found\r\n", path);
        return 1;
    }
    lfs_dir_close(&k_lfs.lfs, &dir);

    strncpy(k_lfs.cwd, path, LFS_MAX_PATH - 1);
    k_lfs.cwd[LFS_MAX_PATH - 1] = 0;
    kprintf("lfs: cwd -> %s\r\n", k_lfs.cwd);
    success;
    return 0;
}

int klfspwd_fast(char* arg) {
    (void)arg;
    kprintf("%s\r\n", k_lfs.cwd);
    success;
    return 0;
}

int klfsmkdir_fast(char* arg) {
    char path[LFS_MAX_PATH];
    char **argv = cutargv(arg);

    if (!k_lfs.mounted) {
        kprintf("lfs: not mounted\r\n");
        return 1;
    }

    if (argv == NULL || argv[0] == NULL) {
        kprintf("lfs mkdir: missing path\r\n");
        return 1;
    }

    lfs_resolve_path(path, argv[0]);

    int err = lfs_mkdir(&k_lfs.lfs, path);
    if (err < 0) {
        kprintf("lfs mkdir: failed %d\r\n", err);
        return 1;
    }
    kprintf("lfs: created dir '%s'\r\n", path);
    success;
    return 0;
}

int klfstouch_fast(char* arg) {
    char path[LFS_MAX_PATH];
    char **argv = cutargv(arg);

    if (!k_lfs.mounted) {
        kprintf("lfs: not mounted\r\n");
        return 1;
    }

    if (argv == NULL || argv[0] == NULL) {
        kprintf("lfs touch: missing path\r\n");
        return 1;
    }

    lfs_resolve_path(path, argv[0]);

    lfs_file_t file;
    int err = lfs_file_open(&k_lfs.lfs, &file, path, LFS_O_WRONLY | LFS_O_CREAT);
    if (err < 0) {
        kprintf("lfs touch: failed %d\r\n", err);
        return 1;
    }
    lfs_file_close(&k_lfs.lfs, &file);
    kprintf("lfs: created file '%s'\r\n", path);
    success;
    return 0;
}

int klfsrm_fast(char* arg) {
    char path[LFS_MAX_PATH];
    char **argv = cutargv(arg);

    if (!k_lfs.mounted) {
        kprintf("lfs: not mounted\r\n");
        return 1;
    }

    if (argv == NULL || argv[0] == NULL) {
        kprintf("lfs rm: missing path\r\n");
        return 1;
    }

    lfs_resolve_path(path, argv[0]);

    int err = lfs_remove(&k_lfs.lfs, path);
    if (err < 0) {
        kprintf("lfs rm: failed %d\r\n", err);
        return 1;
    }
    kprintf("lfs: removed '%s'\r\n", path);
    success;
    return 0;
}

int klfscat_fast(char* arg) {
    char path[LFS_MAX_PATH];
    char **argv = cutargv(arg);

    if (!k_lfs.mounted) {
        kprintf("lfs: not mounted\r\n");
        return 1;
    }

    if (argv == NULL || argv[0] == NULL) {
        kprintf("lfs cat: missing path\r\n");
        return 1;
    }

    lfs_resolve_path(path, argv[0]);

    lfs_file_t file;
    int err = lfs_file_open(&k_lfs.lfs, &file, path, LFS_O_RDONLY);
    if (err < 0) {
        kprintf("lfs cat: cannot open '%s' %d\r\n", path, err);
        return 1;
    }

    char buf[LFS_READ_BUF_SIZE];
    lfs_ssize_t read;
    while ((read = lfs_file_read(&k_lfs.lfs, &file, buf, sizeof(buf))) > 0) {
        for (lfs_ssize_t i = 0; i < read; i++) {
            kprintf("%c", buf[i]);
        }
    }
    kprintf("\r\n");
    lfs_file_close(&k_lfs.lfs, &file);
    success;
    return 0;
}

int klfscp_fast(char* arg) {
    char src[LFS_MAX_PATH];
    char dst[LFS_MAX_PATH];
    char **argv = cutargv(arg);

    if (!k_lfs.mounted) {
        kprintf("lfs: not mounted\r\n");
        return 1;
    }

    if (argv == NULL || argv[0] == NULL || argv[1] == NULL) {
        kprintf("lfs cp: missing src or dst\r\n");
        return 1;
    }

    lfs_resolve_path(src, argv[0]);
    lfs_resolve_path(dst, argv[1]);

    lfs_file_t src_file, dst_file;
    int err = lfs_file_open(&k_lfs.lfs, &src_file, src, LFS_O_RDONLY);
    if (err < 0) {
        kprintf("lfs cp: cannot open src '%s' %d\r\n", src, err);
        return 1;
    }

    err = lfs_file_open(&k_lfs.lfs, &dst_file, dst, LFS_O_WRONLY | LFS_O_CREAT | LFS_O_TRUNC);
    if (err < 0) {
        kprintf("lfs cp: cannot create dst '%s' %d\r\n", dst, err);
        lfs_file_close(&k_lfs.lfs, &src_file);
        return 1;
    }

    char buf[LFS_READ_BUF_SIZE];
    lfs_ssize_t read;
    while ((read = lfs_file_read(&k_lfs.lfs, &src_file, buf, sizeof(buf))) > 0) {
        lfs_ssize_t wrote = lfs_file_write(&k_lfs.lfs, &dst_file, buf, read);
        if (wrote < 0) {
            kprintf("lfs cp: write failed %d\r\n", (int)wrote);
            lfs_file_close(&k_lfs.lfs, &src_file);
            lfs_file_close(&k_lfs.lfs, &dst_file);
            return 1;
        }
    }

    lfs_file_close(&k_lfs.lfs, &src_file);
    lfs_file_close(&k_lfs.lfs, &dst_file);
    kprintf("lfs: copied '%s' -> '%s'\r\n", src, dst);
    success;
    return 0;
}

int klfshelp_fast(char* arg) {
    (void)arg;
    kprintf("lfs cli runner\r\n");
    kprintf("Usage:\r\n");
    kprintf("lfs <command> [args]\r\n");
    kprintf("Available commands:\r\n");
    kprintf("init            mount filesystem\r\n");
    kprintf("deinit          unmount filesystem\r\n");
    kprintf("format          format flash\r\n");
    kprintf("ls [path]       list directory\r\n");
    kprintf("cd [path]       change directory\r\n");
    kprintf("pwd             print working directory\r\n");
    kprintf("mkdir <path>    create directory\r\n");
    kprintf("touch <path>    create file\r\n");
    kprintf("rm <path>       remove file/dir\r\n");
    kprintf("cat <path>      display file content\r\n");
    kprintf("cp <src> <dst>  copy file\r\n");
    kprintf("help            show this help\r\n");
    kprintf("-------------------------------------------\r\n");
    success;
    return 0;
}

cli_node cmd_lfs_table[] = {
    {"init", klfs_init_fast, NULL},
    {"deinit", klfs_deinit_fast, NULL},
    {"format", klfs_format_fast, NULL},
    {"ls", klfsls_fast, NULL},
    {"cd", klfscd_fast, NULL},
    {"pwd", klfspwd_fast, NULL},
    {"mkdir", klfsmkdir_fast, NULL},
    {"touch", klfstouch_fast, NULL},
    {"rm", klfsrm_fast, NULL},
    {"cat", klfscat_fast, NULL},
    {"cp", klfscp_fast, NULL},
    {"help", klfshelp_fast, NULL},
    {NULL, NULL, NULL}
};

#endif

//connect命令行处理（暂时弃用）
/*
#if __USE_UART__ == 2
int kconnectinit_fast(char* arg){
    kconnect_init();
    success;
    return 0;
}
int kconnectdeinit_fast(char* arg){
    kconnect_deinit();
    return 0;
}
int kconnectsendstr_fast(char* arg){
    char** argv = cutargv(arg);
    if(argv == NULL){
        kprintf("send str: 语法错误\r\n");
        return 1;
    }
    char* str = argv[0];
    kconnect_sendstring(str);
    success;
    return 0;
}
int kconnectsendnum_fast(char* arg){
    char** argv = cutargv(arg);
    if(argv == NULL){
        kprintf("send num: 语法错误\r\n");
        return 1;
    }
    int num = atoi(argv[0]);
    kconnect_sendbyte((uint8_t)num);
    success;
    return 0;
}
int kconnectsenddata_fast(char* arg){
    char** argv = cutargv(arg);
    if(argv == NULL){
        kprintf("send data: 语法错误\r\n");
        return 1;
    }
    int count = atoi(argv[0]);
    kconnect_sendarray(publicdatabuf, count);
    success;
    return 0;
}
int kconnectrecvdata_fast(char* arg){
    char** argv = cutargv(arg);
    if(argv == NULL||strcmp(argv[0], "recv") != 0){
        kprintf("recv data: 语法错误\r\n");
        return 1;
    }
    int count = atoi(argv[1]);
    memcpy(publicdatabuf, kconnect_fget(count), count);
    success;
    return 0;
}

int kconnecthelp(char* arg){
    kprintf("connect cli runner\r\n");
    kprintf("Usage:\r\n");
    kprintf("connect <command> [args]\r\n");
    kprintf("Available commands:\r\n");
    kprintf("init  初始化连接\r\n");
    kprintf("deinit  释放连接\r\n");
    kprintf("send str <str>  发送字符串\r\n");
    kprintf("send num <num>  发送数字(10进制格式)\r\n");
    kprintf("send data <count>  发送数据数组(低于%d字节)\r\n",MAX_INPUT_SIZE);
    kprintf("recv data <count>  接收数据数组(低于%d字节)\r\n",MAX_INPUT_SIZE);
    kprintf("help  显示帮助信息\r\n");
    kprintf("-------------------------------------------\r\n");
    success;
    return 0;
}
cli_node cmd_connect_send_table[] = {
    {"str",kconnectsendstr_fast,NULL},
    {"num",kconnectsendnum_fast,NULL},
    {"data",kconnectsenddata_fast,NULL},
    {NULL,NULL,NULL}
};

cli_node cmd_connect_table[] = {
    {"init",kconnectinit_fast,NULL},
    {"deinit",kconnectdeinit_fast,NULL},
    {"send",NULL,cmd_connect_send_table},
    {"recv",kconnectrecvdata_fast,NULL},
    {"help",kconnecthelp,NULL},
    {NULL,NULL,NULL}
};
#endif
*/
