#include "../inc/KSCdraw.h"
#include "../inc/KSCconfig.h"
#if __USE_TEXT__
#include "../inc/KSCfont.h"
#include "../inc/application.h"

#include <stdio.h>
#include <string.h>

typedef struct k_txt_config{
    char* data;
    uint16_t lenmax;
    uint16_t index;
}k_txt_config;
// ---- 布局常量 ----

#define OBJ_WIDTH      8
#define OBJ_HEIGHT     16
#define OBJNUM_X       (SCREEN_WIDTH/OBJ_WIDTH)
#define OBJNUM_Y       (SCREEN_HEIGHT/OBJ_HEIGHT)
#define TXTSCREEN_BACK_COLOR yyellow
// ---- 静态状态 ----
static k_txt_config config;

k_txt_config* txtconfig_set(char* data){
    
    config.data=data;
    config.lenmax=strlen(data);
    config.index=0;
    
    // printf("config.lenmax=%d\n",config.lenmax);
    return &config;
}
// 初始化屏幕

// 屏幕初始化
uint8_t txtscreeninit(ksc_app* app,void* argv){

    KSC_window* ifscreen=malloc(sizeof(KSC_window));
    if(!ifscreen){
        printf("malloc ifscreen failed\n");
        return 0;
    }
    printf("malloc success\n");
    memcpy(ifscreen,&app_screen_default,sizeof(KSC_window));
    printf("ifscreen->ssx=%d\n",ifscreen->ssx);
    ifscreen->bk=TXTSCREEN_BACK_COLOR;
    static KSC_obj_t txt_obj[OBJNUM_X*OBJNUM_Y];
    ifscreen->objbuf=txt_obj;
    ifscreen->objnum=OBJNUM_X*OBJNUM_Y;
    //printf("screen.objnum=%d\n",ifscreen->objnum);
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
    app->window=ifscreen;
    app->app_data = txtconfig_set((char*) argv);
    return 0;
}


uint8_t txtdataupdate(ksc_app* app,void* app_data){
    if(!app)return 0;
    k_txt_config* config=(k_txt_config*) app->app_data;
    if(config->index>=config->lenmax)return 0;
    uint8_t realnum = config->lenmax-config->index <=app->window->objnum?
    config->lenmax-config->index:app->window->objnum;
    // printf("realnum=%d\n",realnum);
    for(uint8_t i=0;i<realnum;i++){
        //printf("i=%d\n",i);
        app->window->objbuf[i].data=&config->data[i]+config->index;
        app->window->objbuf[i]._type=_waitingdraw|_char;
    }
    kscreenclear(app->dev,app->window);
    config->index+=realnum;
    kobjsdraw(app->dev,app->window,app->window->objbuf,realnum);
    return 0;
}

REGISTER_APP(txt,txtscreeninit,txtdataupdate,NULL);

#endif
