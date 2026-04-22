#ifndef KSCobj_h
#define KSCobj_h

#include "KSCconfig.h"
#include "KSCdraw.h"
#include "KSCimg.h"
#include "key.h"

#define _circle 0
#define _box 1
#define _rect 1
#define _string 2
#define _image 3
#define _roundrect 4
#define _fillroundrect 5
#define _fillbox 6
#define _fillrect 6
#define _fillcircle 7
#define _line 8
#define _imagebig 9

#define l_file 0xF0
#define l_string 0xF1

typedef uint8_t ku8;
typedef uint16_t ku16;

typedef void* (*ksc_menu_data_t)(ksc_menu_t* menu,uint8_t index);

//对象属性定义ksc_style_t
typedef struct ksc_style_t{
    void* data;//对象数据指针
    KSCCOLOR colorck;//对象颜色
    KSCCOLOR colorbk;//对象背景颜色
    ku8 width;//对象宽度
    ku8 height;//对象高度
    ku8 d_and_r;//对象半径和深度 低5位为半径，高3位为深度
    ku8 _type;//对象类型
    ku8 x;
    ku8 y;
    ku8 h;
    ku8 w;
}ksc_style_t;//size:16

typedef struct ksc_obj_t{
    
    ku8 x;
    ku8 y;
    ku8 h;
    ku8 w;
}ksc_obj_t;//size:8


typedef struct ksc_menu_config_t{
    ku8 mdw;//菜单项宽度
    ku8 mdh;//菜单项高度
    ku8 menu_wnum;//菜单项行最大数量
    ku8 menu_hnum;//菜单项列最大数量
    ku8 menu_style_num;
    ku8 menu_obj_num;
    ku8 menu_num;//菜单项数量
    ku8 menu_index;//当前选中项索引
} ksc_menu_config_t;//size:8



typedef struct ksc_menu_t{
    ksc_style_t* style;
    ksc_menu_config_t* config;
}ksc_menu_t;//size:8

void kobjdraw(KSC_buf* screen,ksc_style_t* style,ksc_obj_t* obj);
void KSC_menu_draw(KSC_buf* screen,ksc_menu_t* menu,uintxy x,uintxy y);
void KSC_menu_update(KSC_buf* screen,ksc_menu_t* menu,uintxy x,uintxy y);


#endif

