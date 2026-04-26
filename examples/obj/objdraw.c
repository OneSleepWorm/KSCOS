#include "../inc/KSCobj.h"

void obj_demo(void){
    KSC_buf* screen = kinitscreen(0,0,240,160);
    ksc_obj_t obj;
    obj._type = _string;
    obj.state=0x00;
    obj.data = (void*)"Hello";
    obj.width = 20;
    obj.height = 20;
    obj.d_and_r = 1<<5|10;
    obj.colorck = rred;
    obj.colorbk = wwhite;
    obj.sdx=0;
    obj.sdy=0;
    ksc_menu_t menu;
    ksc_obj_t obj1[4];
    obj1[0]._type = _fillbox;
    obj1[0].width = 20;
    obj1[0].height = 20;
    obj1[0].d_and_r = 1<<5|10;
    obj1[0].colorck = bblue;
    obj1[0].colorbk = wwhite;
    obj1[0].state=0x01;
    obj1[0].sdx=0;
    obj1[0].sdy=0;
    //ksc_obj_t obj;
    obj1[1]._type = _box;
    obj1[1].width = 20;
    obj1[1].height = 20;
    obj1[1].d_and_r = 1<<5|10;
    obj1[1].colorck = rred;
    obj1[1].colorbk = wwhite;
    obj1[1].state=0x01;
    obj1[1].sdx=0;
    obj1[1].sdy=0;
    /////////////
    obj1[2]._type = _image_extra;
    obj1[2].width = 16;
    obj1[2].height = 16;
    obj1[2].d_and_r = 1<<5|10;
    obj1[2].colorck = rred;
    obj1[2].colorbk = wwhite;
    obj1[2].state=0x00;
    obj1[2].sdx=4;
    obj1[2].sdy=4;
    ///////////////
    obj1[3]._type = _string_extra;
    obj1[3].width = 16;
    obj1[3].height = 16;
    obj1[3].d_and_r = 1<<5|10;
    obj1[3].colorck = rred;
    obj1[3].colorbk = wwhite;
    obj1[3].state=0x00;
    obj1[3].sdx=4;
    obj1[3].sdy=24;
    menu.style = obj1;
    menu.list = Systemimg;
    ksc_menu_config_t config;
    menu.config = &config;
    config.mdw = 50;
    config.mdh = 30;
    config.menu_wnum = 3;
    config.menu_hnum = 4;
    config.menu_num = 12;
    config.menu_obj_num = 4;

    KSC_menu_draw(screen,&menu,10,10);
    while(1){
        KSC_menu_update(screen,&menu,10,10);
        //printf("%s\r\n",key==KEY_NONE?"":"key);
        
    }
}
