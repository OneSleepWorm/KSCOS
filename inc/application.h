#ifndef __APPLICATION_H
#define __APPLICATION_H

#include "KSCdraw.h"


typedef struct ksc_app ksc_app;
typedef uint8_t (*SCR_UPDATE)(ksc_app* app,void* argv);
typedef uint8_t (*APP_INIT)(ksc_app* app,void* argv);
typedef uint8_t (*APP_DEINIT)(ksc_app* app,void* argv);

typedef struct ksc_app{
    char app_name[16];
    KSC_window* window;
    k_draw_device* dev;
    void* app_data;
    APP_INIT init_func;
    APP_DEINIT deinit_func;
    SCR_UPDATE kupdate;
}ksc_app;

#define SCREEN_WIDTH   100
#define SCREEN_HEIGHT  100
extern const KSC_window app_screen_default;
#include <stdint.h>

uint32_t app_name_hash(const char* app_name);

typedef struct ksc_app_ops{
    char* app_name;
    APP_INIT init_func;
    SCR_UPDATE screen_update_func;
    APP_DEINIT deinit_func;
}ksc_app_ops;

#if __USE_PC__
    #define REGISTER_APP(name, init_func, screen_update_func,deinit_func) \
    static const ksc_app_ops _APP_##name \
    __attribute__((section("app_table"), used)) = \
    { #name, init_func, screen_update_func,deinit_func}

#endif
extern const ksc_app_ops __start_app_table[] ;
extern const ksc_app_ops __stop_app_table[] ;

uint8_t ksc_app_list(void);
ksc_app* ksc_app_init(const char* app_name,void* argv);

#endif
