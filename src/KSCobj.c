#include "../inc/KSCobj.h"
#include <stdlib.h>
#define k_malloc(size) malloc(size)

void kobjdraw(KSC_buf* screen,ksc_style_t* obj,uintxy x,uintxy y){
    if(!obj)
    return;
    switch(obj->_type){
        case _circle:
            kcircle(screen,obj->colorck,x,y,obj->d_and_r&0x1F);
            break;
        case _box:
            kbox(screen,obj->colorck,x,y,obj->width,obj->height);
            break;
        case _string:
            kstring(screen,(char*)obj->data,x,y,obj->colorbk,obj->colorck);
            break;
        case _image:
            kdrawimage(screen,(uint16_t*)obj->data,x,y,obj->width,obj->height);
            break;
        case _roundrect:
            kroundrect(screen,obj->colorck,x,y,obj->width,obj->height,obj->d_and_r&0x1F);
            break;
        case _fillroundrect:
            kfillroundrect(screen,obj->colorck,x,y,obj->width,obj->height,obj->d_and_r&0x1F);
            break;
        case _fillbox:
            kfillbox(screen,obj->colorck,x,y,obj->width,obj->height);
            break;
        case _fillcircle:
            kfillcircle(screen,obj->colorck,x,y,obj->d_and_r&0x1F);
            break;
        case _line:
            kline(screen,obj->colorck,x,y,x+obj->width,y+obj->height);
            break;
        case _imagebig:
            kdrawimagebig(screen,(uint16_t*)obj->data,x,y,obj->width,obj->height,(obj->d_and_r>>5));
            break;
        default:
            break;
    }
}

void* list_data(Img_File* list,uint8_t index){
    return list[index].data;
}

ksc_menu_t* KSC_menu_addfile(ksc_style_t* style,uint8_t style_num,Img_File* list,ku8 list_type,ku8 list_num){
    if(!style) return NULL;
    ksc_menu_t* menu = k_malloc(sizeof(ksc_menu_t));
    ksc_obj_t obj[list_num]=k_malloc(sizeof(ksc_obj_t)*list_num);
    menu->style = style;
    //menu->config = config;
    switch(list_type){
        case l_file:
        for(uint8_t i=0;i<list_num;i++){
            if(style[i]._type == _image)
                obj[i]
        }
        case l_string:
            list->name;
        default:
            break;
    }
    
}   

void KSC_menu_draw(KSC_buf* screen,ksc_menu_t* menu,uintxy x,uintxy y){
    uint8_t hnum = menu->config->menu_hnum;
    uint8_t wnum = menu->config->menu_wnum;
    for(uint8_t j=0;j<hnum;j++){
        for(uint8_t i=0;i<wnum;i++){
            //printf("index:%d\r\n",j*wnum+i);
            if(menu->config->menu_use_img)
            kdrawimage(screen,(uint16_t*)menu->list[j*wnum+i].data+4
                ,x+menu->config->mdw*i+menu->config->mimgsdx
                ,y+menu->config->mdh*j+menu->config->mimgsdy
                ,*(menu->list[j*menu->config->menu_wnum+i].data+3)
                ,*(menu->list[j*menu->config->menu_wnum+i].data+5));
            if(menu->config->menu_use_str)
            kstring(screen,menu->list[j*wnum+i].name
                ,x+menu->config->mdw*i+menu->config->mstrsdx
                ,y+menu->config->mdh*j+menu->config->mstrsdy
                ,menu->style->colorck,menu->style->colorbk);
            // if(j*menu->menu_wnum+i>=menu->menu_num)
            // return;
        }
    }
    kbox(screen,menu->style->colorck
        ,x-1+(menu->config->menu_index%wnum)*menu->config->mdw
        ,y-1+(menu->config->menu_index/wnum)*menu->config->mdh
        ,menu->config->mdw
        ,menu->config->mdh);
}

void KSC_menu_update(KSC_buf* screen,ksc_menu_t* menu,uintxy x,uintxy y){
    if(!menu) return;
    uint8_t key = key_scan();
    if(key==KEY_NONE)return;
    uint8_t wnum = menu->config->menu_wnum;
    uint8_t hnum = menu->config->menu_hnum;
    kbox(screen,menu->style->colorbk
        ,x-1+(menu->config->menu_index%wnum)*menu->config->mdw
        ,y-1+(menu->config->menu_index/wnum)*menu->config->mdh
        ,menu->config->mdw
        ,menu->config->mdh);
    uint8_t menu_max = wnum*hnum-1;
    switch(key){
        case KEY_A4:
            menu->config->menu_index>0?menu->config->menu_index--:0;
            break;
        case KEY_A6:
            menu->config->menu_index<menu_max?menu->config->menu_index++:0;
            break;
        case KEY_A8:
            menu->config->menu_index>=wnum?menu->config->menu_index-=wnum:menu->config->menu_index;
            break;
        case KEY_A2:
            menu->config->menu_index+wnum<=menu_max?menu->config->menu_index+=wnum:0;
            break;
        default:
            break;
    }
    kbox(screen,menu->style->colorck
        ,x-1+(menu->config->menu_index%wnum)*menu->config->mdw
        ,y-1+(menu->config->menu_index/wnum)*menu->config->mdh
        ,menu->config->mdw
        ,menu->config->mdh);
    // if(menu->config->menu_index<wnum*hnum)
    // menu->config->menu_index%=wnum*hnum;
}
