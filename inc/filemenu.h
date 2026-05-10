#ifndef _MENU_H_
#define _MENU_H_

#include "KSCobj.h"
#include "KSCimg.h"

#define ksc_style_t ksc_obj_t


typedef struct ksc_menu_config_t{
    ku8 mdw;//菜单项宽度
    ku8 mdh;//菜单项高度
    ku8 menu_wnum;//菜单项行最大数量
    ku8 menu_hnum;//菜单项列最大数量
    ku8 menu_list_num;//菜单项列表缓冲区数量
    ku8 menu_obj_num;//菜单项对象数量
    ku8 menu_num;//菜单项数量
    ku8 menu_index;//当前选中项索引
} ksc_menu_config_t;//size:8

typedef struct ksc_menu_t{
    ksc_style_t** style;
    Img_File* list;
    ksc_menu_config_t* config;
}ksc_menu_t;//size:12

ksc_menu_t* filemenu_init(k_draw_device* dev,KSC_window* screen);
void kmenu_draw(k_draw_device* dev,KSC_window* screen, ksc_menu_t* menu);
void kmenu_update(k_draw_device* dev,KSC_window* screen, ksc_menu_t* menu);
void filemenu_flashscreen(k_draw_device* dev,KSC_window* screen, ksc_menu_t* menu,uint8_t key, uint8_t x, uint8_t y);

#endif