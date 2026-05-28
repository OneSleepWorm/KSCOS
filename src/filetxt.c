#include "../inc/filetxt.h"
#if __USE_TEXT__
#include "../inc/KSCfont.h"

#include <stdio.h>
#include <string.h>
// ---- 布局常量 ----
#define SCREEN_WIDTH   100
#define SCREEN_HEIGHT  100
#define OBJ_WIDTH      7
#define OBJ_HEIGHT     8
#define OBJNUM_X       (SCREEN_WIDTH/OBJ_WIDTH)
#define OBJNUM_Y       (SCREEN_HEIGHT/OBJ_HEIGHT)
// ---- 静态状态 ----
static KSC_window  txt_screen = {
    .bk=0xE0FF,
    .objbuf=NULL,
    .objnum=0,
    .ssx = 10,
    .ssy = 10,
    .width = SCREEN_WIDTH,
    .height = SCREEN_HEIGHT,
};
static KSC_obj_t txt_obj[OBJNUM_X*OBJNUM_Y];

// 初始化屏幕

// 屏幕初始化
KSC_window txtscreeninit(k_draw_device* dev){
    KSC_window screen=txt_screen;
    screen.objbuf=txt_obj;
    screen.objnum=OBJNUM_X*OBJNUM_Y;
    printf("screen.objnum=%d\n",screen.objnum);
    for(uint8_t i=0;i<OBJNUM_Y;i++){
        for(uint8_t j=0;j<OBJNUM_X;j++){
            txt_obj[i*OBJNUM_X+j]._type=_drawed&_char;
            txt_obj[i*OBJNUM_X+j].sdx=j*OBJ_WIDTH;
            txt_obj[i*OBJNUM_X+j].sdy=i*OBJ_HEIGHT;
            txt_obj[i*OBJNUM_X+j].colorck=ggreen;

            txt_obj[i*OBJNUM_X+j].width=Systemfont0.width;
            txt_obj[i*OBJNUM_X+j].height=Systemfont0.height;
        }
    }
    return screen;
}

k_txt_config txtconfig_set(char* data){
    k_txt_config config={
        .data=data,
        .lenmax=strlen(data),
        .index=0,
    };
    printf("config.lenmax=%d\n",config.lenmax);
    return config;
}
void txtdataupdate(KSC_window* screen,k_txt_config* config){
    if(!screen || !config)return;
    if(config->index>=config->lenmax)return;
    uint8_t realnum = config->lenmax-config->index <=screen->objnum?config->lenmax-config->index:screen->objnum;
    printf("realnum=%d\n",realnum);
    for(uint16_t i=0;i<realnum;i++){
        screen->objbuf[i].data=&config->data[i]+config->index;
        screen->objbuf[i]._type=_waitingdraw|_char;
    }
    config->index+=realnum;
    screen->objnum=realnum;
}


#endif
