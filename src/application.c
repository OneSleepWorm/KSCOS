// 应用程序生成
#include "../inc/application.h"

const KSC_window  app_screen_default = {
    .bk=wwhite,
    .objbuf=NULL,
    .objnum=0,
    .ssx = 10,
    .ssy = 10,
    .width = SCREEN_WIDTH,
    .height = SCREEN_HEIGHT,
};

uint32_t app_name_hash(const char* app_name){
    uint32_t hash=0;
    while(*app_name){
        hash += (uint8_t)*app_name++;
        hash <<= 5;
    }
    return hash;
}

uint8_t ksc_app_list(void){
    uint8_t num=0;
    for(const ksc_app_ops* app=__start_app_table; app!=__stop_app_table; app++){
        printf("app_name=%s\n",app->app_name);
        num++;
    }
    return num;
}

/* system placeholder app */
REGISTER_APP(sys, NULL, NULL,NULL);

#include <string.h>

ksc_app* ksc_app_init(const char* app_name,void* argv){
    if(!app_name) return NULL;
    ksc_app* app = (ksc_app*)malloc(sizeof(ksc_app));
    KSC_window* window = (KSC_window*)malloc(sizeof(KSC_window));
    if(!app || !window) return NULL;
    app->window = window;
    for(const ksc_app_ops* existapp=__start_app_table; existapp!=__stop_app_table; existapp++){
        if(strcmp(existapp->app_name, app_name)==0){
            app->init_func = existapp->init_func;
            app->kupdate = existapp->screen_update_func;
            app->dev = k_draw_device_find(app_name);
            memcpy(app->window,&app_screen_default,sizeof(KSC_window));
            app->deinit_func = existapp->deinit_func;
            app->init_func(app,argv);
            return app;
        }
    }
    free(app);
    printf("app_name not found\n");
    return NULL;
}


