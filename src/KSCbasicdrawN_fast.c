#include "../inc/KSCbasicdrawN_fast.h"
#include "../inc/KSCimg.h"
#include "string.h"
#include "../inc/KSCconfig.h"
#include "../inc/Serial.h"
#include "main.h"
#include "../inc/W25Q64.h"
#include <stdint.h>

//--locale=english --no-multibyte-chars

#define MAX_ARGV 8
#define success ksendbyte(0x00)
//#define success __nop()

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
    kinitscreen(width,height,NULL,0,0);
    ksetpencolor(kgetscreen(),GREEN,1);
    return 0;
}

int kfreescreen_fast(char* arg){
    kfreescreen(kgetscreen());
    return 0;
}

int ksetcolor_fast(char* arg){
    char** argv = cutargv(arg);
    if(argv == NULL){
        kprintf("setcolor: 语法错误\r\n");
        return 1;
    }
    KSCCOLOR color = (KSCCOLOR)strtol(argv[0],NULL,16);
    kprintf("setcolor: %d\r\n",color);
    int value = atoi(argv[1]);
    ksetpencolor(kgetscreen(),color,value);
	success;
    return 0;
}

int kpixel_fast(char* arg){
    char** argv = cutargv(arg);
    if(argv == NULL){
        kprintf("pixel: 语法错误\r\n");
        return 1;
    }
    int x = atoi(argv[0]);
    int y = atoi(argv[1]);
    ksetpixel(kgetscreen(),x,y);
	success;
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
    kline(kgetscreen(),x1,y1,x2,y2);
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
    krect(kgetscreen(),x1,y1,w,h);
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
    kcircle(kgetscreen(),x0,y0,r);
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
	kfillrect(kgetscreen(),x1,y1,w,h);
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
    kfillcircle(kgetscreen(),x0,y0,r);
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
    kstring(kgetscreen(),str,x,y);
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
    kstringchinese(kgetscreen(),str,x,y);
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
    kdrawimage(kgetscreen(),x,y,img.data,img.width,img.height);
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
    kdrawimagebig(kgetscreen(),x,y,img.data,img.width,img.height,scale);
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
    kprintf("setcolor <color> <value>  设置前景/背景颜色\r\n");
    kprintf("pixel <x> <y>  绘制像素点\r\n");
    kprintf("line <x1> <y1> <x2> <y2>  绘制线\r\n");
    kprintf("rect <x1> <y1> <w> <h>  绘制矩形\r\n");
    kprintf("circle <x> <y> <r>  绘制圆\r\n");
    kprintf("fillrect <x1> <y1> <w> <h>  填充矩形\r\n");
    kprintf("fillcircle <x> <y> <r>  填充圆\r\n");
    kprintf("string <str> <x> <y>  绘制字符串\r\n");
    kprintf("stringchinese <str> <x> <y>  绘制中文字符串(测试版)\r\n");
    kprintf("image <filename> <x> <y>  绘制图片\r\n");
    kprintf("help  显示帮助信息\r\n");
    kprintf("-------------------------------------------\r\n");
    success;
    return 0;
}

// 定义cmd_draw_table数组
cli_node cmd_draw_table[] = {
    {"initscreen",kinitscreen_fast,NULL},
    {"freescreen",kfreescreen_fast,NULL},
    {"setcolor",ksetcolor_fast,NULL},
    {"pixel",kpixel_fast,NULL},
    {"line",kline_fast,NULL},
    {"rect",krect_fast,NULL},
    {"circle",kcircle_fast,NULL},
    {"fillrect",kfillrect_fast,NULL},
    {"fillcircle",kfillcircle_fast,NULL},
    {"string",kstring_fast,NULL},
    {"stringchinese",kstringchinese_fast,NULL},
	{"image",kimage_fast,NULL},
    {"imagebig",kimagebig_fast,NULL},
    {"help",drawhelp,NULL},
    {NULL,NULL,NULL}
};

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
