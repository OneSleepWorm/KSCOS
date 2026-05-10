#include "../inc/KSCobj.h"

void kdrawimagefile(k_draw_device* dev,KSC_window* screen,Img_File* imgfile,uintxy x,uintxy y){
    if(!imgfile){return;}
    //解析图片数据
    #ifndef __USE_IMGNAME__
        const uint16_t* imgbuf = (const uint16_t*)imgfile->data+4;
        uint8_t width = *((uint8_t*)imgfile->data+3);
        uint8_t height = *((uint8_t*)imgfile->data+5);
        kdrawimage(dev,screen,imgbuf,x,y,width,height);
    #endif
}

void kdrawimagebigfile(k_draw_device* dev,KSC_window* screen,Img_File* imgfile,uintxy x,uintxy y,uint8_t scale){
    if(!imgfile){return;}
    //解析图片数据
    #ifndef __USE_IMGNAME__
        const uint16_t* imgbuf = (const uint16_t*)imgfile->data+4;
        uint8_t width = *((uint8_t*)imgfile->data+3);
        uint8_t height = *((uint8_t*)imgfile->data+5);
        kdrawimagebig(dev,screen,imgbuf,x,y,width,height,scale);
    #endif
}

void kobjdraw(k_draw_device* dev,KSC_window* screen,const ksc_obj_t* obj,uintxy x,uintxy y,const void* extradata){
    if(!screen || !obj)return;
    // x = obj->x;
    // y = obj->y;
    if(obj->state&0x01)return;
    switch(obj->_type&_type_mask){
        case _circle:
            kcircle(dev,screen,obj->colorck,x+obj->sdx,y+obj->sdy,obj->d_and_r&0x1F);
            break;
        case _box:
            kbox(dev,screen,obj->colorck,x+obj->sdx,y+obj->sdy,obj->width,obj->height);
            break;
        case _string:
            kstring(dev,screen,(char*)obj->data,x+obj->sdx,y+obj->sdy,obj->colorck,obj->colorbk);
            break;
        case _image:
            kdrawimagefile(dev,screen,((Img_File*)obj->data),x+obj->sdx,y+obj->sdy);
            break;
        case _roundrect:
            kroundrect(dev,screen,obj->colorck,x+obj->sdx,y+obj->sdy,obj->width,obj->height,obj->d_and_r&0x1F);
            break;
        case _fillroundrect:
            kfillroundrect(dev,screen,obj->colorck,x+obj->sdx,y+obj->sdy,obj->width,obj->height,obj->d_and_r&0x1F);
            break;
        case _fillbox:
            kfillbox(dev,screen,obj->colorck,x+obj->sdx,y+obj->sdy,obj->width,obj->height);
            break;
        case _fillcircle:
            kfillcircle(dev,screen,obj->colorck,x+obj->sdx,y+obj->sdy,obj->d_and_r&0x1F);
            break;
        case _line:
            kline(dev,screen,obj->colorck,x+obj->sdx,y+obj->sdy,x+obj->sdx+obj->width,y+obj->sdy+obj->height);
            break;
        case _imagebig:
            kdrawimagebigfile(dev,screen,((Img_File*)obj->data),x+obj->sdx,y+obj->sdy,(obj->d_and_r>>5));
            break;
        case _string_extra:
            kstring(dev,screen,(char*)extradata,x+obj->sdx,y+obj->sdy,obj->colorck,obj->colorbk);
            break;
        case _image_extra:
            kdrawimagefile(dev,screen,((Img_File*)extradata),x+obj->sdx,y+obj->sdy);
            break;
        case _imagebig_extra:
            kdrawimagebigfile(dev,screen,((Img_File*)extradata),x+obj->sdx,y+obj->sdy,(obj->d_and_r>>5));
            break;
        default:
            break;
    }
}

void kobjsdraw(k_draw_device* dev,KSC_window* screen,const ksc_obj_t** obj,uintxy x,uintxy y,const void** extradata,uint8_t num){
    if(!dev || !screen || !obj || !extradata || !num)return;
    for(uint8_t i=0;i<num;i++){
        kobjdraw(dev,screen,obj[i],x,y,extradata[i]);
    }
}
/*我想到了一个更好的方法，以下均为废弃代码
void KSC_menu_clear(KSC_window* screen,ksc_menu_t* menu,uintxy x,uintxy y){
    if(!screen || !menu || !menu->config) return;
    kfull(screen, screen->bk, x, y,
          menu->config->mdw * menu->config->menu_wnum,
          menu->config->mdh * menu->config->menu_hnum);
}

void KSC_menu_draw(KSC_window* screen,ksc_menu_t* menu,uintxy x,uintxy y){
    if(!screen || !menu || !menu->config || !menu->style || !menu->list) return;
    uint8_t hnum = menu->config->menu_hnum;
    uint8_t wnum = menu->config->menu_wnum;
    for(uint8_t j=0;j<hnum;j++){
        for(uint8_t i=0;i<wnum;i++){
            if(menu->config->menu_num<=i+wnum*j) return;
            for(uint8_t k=0;k<menu->config->menu_obj_num;k++){
                void* extradata = NULL;
                if(menu->style[k]->_type==(_image_extra&_type_mask))
                    extradata = (void*)&menu->list[i+wnum*j].data;
                else if(menu->style[k]->_type==(_string_extra|_custom_label1)){
                    extradata = (void*)menu->list[i+wnum*j].name;
                }else if(menu->style[k]->_type==(_string_extra|_custom_label2)){
                        if(menu->list[i+wnum*j].type==2){
                            extradata = "[dir]";
                        }else if(menu->list[i+wnum*j].type==1){
                            extradata = "[file]";
                        }
                }else if((menu->style[k]->_type&_custom_mask)==_custom_label3){
                    if(menu->config->menu_index!=i+wnum*j){
                        continue;
                    }
                }else{
                    extradata = NULL;
                }
                kobjdraw(screen,menu->style[k]
                    ,x+menu->config->mdw*i
                    ,y+menu->config->mdh*j,
                    extradata);
                

            }
        }
    }
}

#if __USE_KEY__ > 0

void KSC_menu_update(KSC_window* screen,ksc_menu_t* menu,uintxy x,uintxy y,uint8_t key){
    if(!screen || !menu || !menu->config) return;
    // uint8_t key = key_scan();
    if(key==KEY_NONE)return;
    uint8_t wnum = menu->config->menu_wnum;
    uint8_t hnum = menu->config->menu_hnum;
    kbox(screen,screen->bk
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
    kbox(screen,screen->bk
        ,x-1+(menu->config->menu_index%wnum)*menu->config->mdw
        ,y-1+(menu->config->menu_index/wnum)*menu->config->mdh
        ,menu->config->mdw
        ,menu->config->mdh);
    // if(menu->config->menu_index<wnum*hnum)
    // menu->config->menu_index%=wnum*hnum;
}

#endif
*/




