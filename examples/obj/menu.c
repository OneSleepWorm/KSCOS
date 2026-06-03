// #include <conio.h>
// #include "stdio.h"
// #include "KSCdraw/inc/W25Q64.h"
// #include "KSCdraw/littlefs/lfs.h"
// #include "KSCdraw/littlefs/lfs_config.h"

#include "../inc/master.h"
// extern void W25Q64_Init(void);
// void littlefs_test(void);
int main_botton_callback(void){
    printf("main_botton_callback\n");
    return 0;
}
k_draw_device dev={
    .init=screen_init,
    .setcanvas=screen_setcanvas,
    .setcolorpixels=screen_setcolorpixels,
};

int main(){
    kdevmount(&dev);
    W25Q64_Init();
    key_init();
    // KSC_window* screen = kscreeninit(&dev,0,0,160,160,0xFFFF);
    KSC_window  screen2={
        .bottoncallback=main_botton_callback,
        .ssx=20,
        .ssy=40,
        .width=100,
        .height=100,
        .bk=wwhite,
    };
    // kfull(&dev,&screen2,bblue,0,0,100,100);
    // kstring(&dev,&screen2,"KSCdraw",0,0,wwhite,bblack);
    ksc_menu_t* menu = filemenu_init(&dev,&screen2);
    kmenu_draw(&dev,&screen2,menu);
    screen2.bottoncallback();
    // kfillbox(&screen2,bblue,0,0,100,100);
    while(1){
        kmenu_update(&dev,&screen2,menu);
    }
    return 0;
}


