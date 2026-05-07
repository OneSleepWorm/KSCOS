#ifndef KSCobj_h
#define KSCobj_h

#include "KSCconfig.h"
#include "KSCdraw.h"
#include "KSCimg.h"


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
#define _extra (1<<4)

#define _string_extra (_extra|_string)
#define _image_extra (_extra|_image)
#define _imagebig_extra (_extra|_imagebig)

#define _type_mask (0x1F)
#define _custom_mask (0xE0)

#define _custom_label1 (1<<5)
#define _custom_label2 (1<<6)
#define _custom_label3 (1<<7)

typedef uint8_t ku8;
typedef uint16_t ku16;

//对象属性定义ksc_obj_t
typedef struct ksc_obj_t{
    KSCCOLOR colorck;//对象颜色
    KSCCOLOR colorbk;//对象背景颜色

    void* data;//对象数据指针
    ku8 width;//对象宽度
    ku8 height;//对象高度
    ku8 sdx;//对象x偏移量
    ku8 sdy;//对象y偏移量
    
    ku8 state;//对象状态,bit0:0,正常,1,隐藏
    ku8 custom;//对象自定义属性
    ku8 d_and_r;//对象半径和深度 低5位为半径，高3位为深度
    ku8 _type;//对象类型
}ksc_obj_t;//size:12

/* 0, 菜单项样式定义,文件名和文件属性需要特殊处理
 * 1. 文件名需要加上_custom_label1,
 * 2. 文件属性需要加上_custom_label2
 */
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
} ksc_menu_config_t;//size:12

typedef struct ksc_menu_t{
    ksc_style_t** style;
    Img_File* list;
    ksc_menu_config_t* config;
}ksc_menu_t;//size:12


void kobjdraw(KSC_window* screen,const ksc_obj_t* obj,uintxy x,uintxy y,const void* extradata);
void kobjsdraw(KSC_window* screen,const ksc_obj_t** obj,uintxy x,uintxy y,const void** extradata,uint8_t num);
/*
void KSC_menu_draw(KSC_window* screen,ksc_menu_t* menu,uintxy x,uintxy y);
void KSC_menu_clear(KSC_window* screen,ksc_menu_t* menu,uintxy x,uintxy y);
#if __USE_KEY__ > 0
#include "key.h"
void KSC_menu_update(KSC_window* screen,ksc_menu_t* menu,uintxy x,uintxy y,uint8_t key);
#endif
*/
#endif

