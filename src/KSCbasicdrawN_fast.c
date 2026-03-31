#include "../inc/KSCbasicdrawN_fast.h"
#include "../inc/KSCimg.h"
#include "string.h"
#include "../inc/KSCconfig.h"

#include "../inc/osconnect.h"
#define MAX_ARGV 8
#define success kconnect_sendstring("OK")
//#define success __nop()

#include "stdio.h"

char** cutargv(char* arg){
    if(arg == NULL){
        return NULL;
    }
    static char* argv[MAX_ARGV]={NULL};
    char* p = strtok(arg," ");
    int argc = 0;
    while(p != NULL&&argc < MAX_ARGV){
        argv[argc++] = p;
        p = strtok(NULL," ");
    }
    argv[argc] = NULL;
    return argv;
} 

int kinitscreen_fast(char* arg){
    char** argv = cutargv(arg);
    if(argv == NULL){
        printf("initscreen: 语法错误\r\n");
        return 1;
    }
    int width = atoi(argv[0]);
    int height = atoi(argv[1]);
    printf("initscreen: %d %d\r\n",width,height);
    if(width <= 0 || height <= 0){
        printf("initscreen: 宽度和高度必须大于0\r\n");
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
        printf("setcolor: 语法错误\r\n");
        return 1;
    }
    KSCCOLOR color = (KSCCOLOR)strtol(argv[0],NULL,16);
    printf("setcolor: %d\r\n",color);
    int value = atoi(argv[1]);
    ksetpencolor(kgetscreen(),color,value);
	success;
    return 0;
}

int kpixel_fast(char* arg){
    char** argv = cutargv(arg);
    if(argv == NULL){
        printf("pixel: 语法错误\r\n");
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
        printf("line: 语法错误\r\n");
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
        printf("rect: 语法错误\r\n");
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
        printf("circle: 语法错误\r\n");
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
        printf("fillrect: 语法错误\r\n");
        return 1;
    }
    int x1 = atoi(argv[0]);
    int y1 = atoi(argv[1]);
    int w = atoi(argv[2]);
    int h = atoi(argv[3]);
    kfillrect(kgetscreen(),x1,y1,w,h);
	printf("hihihi");
	success;
    return 0;
}

int kfillcircle_fast(char* arg){
    char** argv = cutargv(arg);
    if(argv == NULL){
        printf("fillcircle: 语法错误\r\n");
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
        printf("string: 语法错误\r\n");
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
        printf("stringchinese: 语法错误\r\n");
        return 1;
    }
    char* str = argv[1];
    int x = atoi(argv[2]);
    int y = atoi(argv[3]);
    kstringchinese(kgetscreen(),str,x,y);
 return 0;
}
#endif
int kimage_fast(char* arg){
    char** argv = cutargv(arg);
    if(argv == NULL){
        printf("image: 语法错误\r\n");
        return 1;
    }
    char* filename = argv[0];
    Img_File img = kloadimage(filename);
    if(img.name == NULL){
        printf("image: 图片不存在\r\n");
        return 1;
    }
    int x = atoi(argv[1]);
    int y = atoi(argv[2]);
    kdrawimage(kgetscreen(),x,y,img.data,img.width,img.height);
    return 0;
}



int drawhelp(char* arg){
    printf("draw cli runner\r\n");
    printf("Usage:\r\n");
    printf("draw <command> [args]\r\n");
    printf("Available commands:\r\n");
    printf("initscreen <width> <height>  初始化屏幕\r\n");
    printf("freescreen  释放屏幕缓冲区\r\n");
    printf("setcolor <color> <value>  设置前景/背景颜色\r\n");
    printf("pixel <x> <y>  绘制像素点\r\n");
    printf("line <x1> <y1> <x2> <y2>  绘制线\r\n");
    printf("rect <x1> <y1> <w> <h>  绘制矩形\r\n");
    printf("circle <x> <y> <r>  绘制圆\r\n");
    printf("fillrect <x1> <y1> <w> <h>  填充矩形\r\n");
    printf("fillcircle <x> <y> <r>  填充圆\r\n");
    printf("string <str> <x> <y>  绘制字符串\r\n");
    printf("stringchinese <str> <x> <y>  绘制中文字符串(测试版)\r\n");
    printf("image <filename> <x> <y>  绘制图片\r\n");
    printf("help  显示帮助信息\r\n");
    printf("-------------------------------------------\r\n");
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
    {"help",drawhelp,NULL},
    {NULL,NULL,NULL}
};

#if __USE_FLASH__ > 0
//flash命令行处理
#include "../inc/Flash_OS.h"

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
    static uint16_t count = 0;
    
    if(get_run_cli_callback() == 0){
        char** argv = cutargv(arg);
        if(argv == NULL){
            printf("write: 语法错误\r\n");
            return 1;
        }
        addr = strtol(argv[0],NULL,16);
        count = atoi(argv[1]);
        set_run_cli_callback(1);
        //flash_write_data(addr, publicdatabuf, count);
        success;
        return 0;
    }else{
        if(count <=MAX_WRITE_COUNT){
            flash_write_data(addr, (uint8_t*)arg, count);
            count =0;
            set_run_cli_callback(0);
            success;
            // kconnect_sendbyte(count>>8);
            // kconnect_sendbyte(count);
            return 0;
        }else{
            flash_write_data(addr, (uint8_t*)arg, MAX_WRITE_COUNT);
            count -=MAX_WRITE_COUNT;
            addr +=MAX_WRITE_COUNT;
            success;
            // kconnect_sendbyte(count>>8);
            // kconnect_sendbyte(count);
            return 0;
        }
    }
}

int kflashread_fast(char* arg){
    char** argv = cutargv(arg);
    if(argv == NULL){
        printf("read: 语法错误\r\n");
        return 1;
    }
    uint32_t addr = strtol(argv[0],NULL,16);
    int count = atoi(argv[1]);
    printf("log:read addr=%d,count=%d\r\n",addr,count);
    flash_read_data(addr, publicdatabuf, count);
    for(int i=0;i<count;i++){
        printf("%02x ",publicdatabuf[i]);
    }
    printf("\r\n");
    success;
    return 0;
}
int kflasherase_fast(char* arg){
    char** argv = cutargv(arg);
    if(argv == NULL){
        printf("erase: 语法错误\r\n");
        return 1;
    }
    int addr = atoi(argv[0]);
    flash_sector_erase(addr);
    success;
    return 0;
}


int kflashhelp(char* arg){
    printf("flash cli runner\r\n");
    printf("Usage:\r\n");
    printf("flash <command> [args]\r\n");
    printf("Available commands:\r\n");
    printf("init  初始化flash\r\n");
    printf("deinit  释放flash\r\n");
    printf("write <addr> <count>  向flash写入数据\r\n");
    printf("read <addr> <count>  从flash读取数据\r\n");
    printf("erase <addr>  扇区擦除\r\n");
    printf("-------------------------------------------\r\n");
    success;
    return 0;
}

cli_node cmd_flash_table[] = {
    {"init",kflashinit_fast,NULL},
    {"deinit",kflashdeinit_fast,NULL},
    {"write",kflashwrite_fast,NULL},
    {"read",kflashread_fast,NULL},
    {"erase",kflasherase_fast,NULL},
    {"help",kflashhelp,NULL},
    {NULL,NULL,NULL}
};
#endif

//connect命令行处理
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
        printf("send str: 语法错误\r\n");
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
        printf("send num: 语法错误\r\n");
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
        printf("send data: 语法错误\r\n");
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
        printf("recv data: 语法错误\r\n");
        return 1;
    }
    int count = atoi(argv[1]);
    memcpy(publicdatabuf, kconnect_fget(count), count);
    success;
    return 0;
}

int kconnecthelp(char* arg){
    printf("connect cli runner\r\n");
    printf("Usage:\r\n");
    printf("connect <command> [args]\r\n");
    printf("Available commands:\r\n");
    printf("init  初始化连接\r\n");
    printf("deinit  释放连接\r\n");
    printf("send str <str>  发送字符串\r\n");
    printf("send num <num>  发送数字(10进制格式)\r\n");
    printf("send data <count>  发送数据数组(低于%d字节)\r\n",MAX_INPUT_SIZE);
    printf("recv data <count>  接收数据数组(低于%d字节)\r\n",MAX_INPUT_SIZE);
    printf("help  显示帮助信息\r\n");
    printf("-------------------------------------------\r\n");
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
