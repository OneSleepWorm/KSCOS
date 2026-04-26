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


#define _string_extra (0x82)
#define _image_extra (0x83)
#define _imagebig_extra (0x89)

typedef uint8_t ku8;
typedef uint16_t ku16;

//对象属性定义ksc_obj_t
typedef struct ksc_obj_t{
    void* data;//对象数据指针
    KSCCOLOR colorck;//对象颜色
    KSCCOLOR colorbk;//对象背景颜色
    ku8 width;//对象宽度
    ku8 height;//对象高度
    ku8 state;//对象状态
    ku8 y;//对象y坐标
    ku8 sdx;//对象x偏移量
    ku8 sdy;//对象y偏移量
    ku8 d_and_r;//对象半径和深度 低5位为半径，高3位为深度
    ku8 _type;//对象类型
}ksc_obj_t;//size:12

#define ksc_style_t ksc_obj_t


typedef struct ksc_menu_config_t{
    ku8 mdw;//菜单项宽度
    ku8 mdh;//菜单项高度
    ku8 menu_wnum;//菜单项行最大数量
    ku8 menu_hnum;//菜单项列最大数量
    // ku8 mimgsdx;//图片与顶点之间的x间距
    // ku8 mimgsdy;//图片与顶点之间的y间距
    // ku8 mstrsdx;//字符串与顶点之间的x间距
    // ku8 mstrsdy;//字符串与顶点之间的y间距
    ku8 menu_list_num;
    ku8 menu_obj_num;
    ku8 menu_num;//菜单项数量

    ku8 menu_index;//当前选中项索引
} ksc_menu_config_t;//size:12

typedef struct ksc_menu_t{
    ksc_style_t* style;
    Img_File* list;
    ksc_menu_config_t* config;
}ksc_menu_t;//size:12

#if __USE_LCD__ > 0
void kobjdraw(KSC_buf* screen,const ksc_obj_t* obj,uintxy x,uintxy y,const void* extradata);
void KSC_menu_draw(KSC_buf* screen,ksc_menu_t* menu,uintxy x,uintxy y);
#endif


#if __USE_KEY__ > 0
#include "key.h"
void KSC_menu_update(KSC_buf* screen,ksc_menu_t* menu,uintxy x,uintxy y);
#endif

#endif

