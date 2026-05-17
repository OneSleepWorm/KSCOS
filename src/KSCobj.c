#include "../inc/KSCobj.h"

void kobjdraw(k_draw_device* dev,KSC_window* screen,const ksc_obj_t* obj){
    if(!dev || !screen || !obj)return;
    switch (obj->_type&_type_mask)
    {
    case _circle:
        /* code */
        kcircle(dev,screen,obj->colorck,obj->sdx,obj->sdy,obj->width);
        break;
    case _box:
        /* code */
        kbox(dev,screen,obj->colorck,obj->sdx,obj->sdy,obj->width,obj->height);
        break;
    case _string:
        /* code */
        kstring(dev,screen,(char*)obj->data,obj->sdx,obj->sdy,obj->colorck,screen->bk);
        break;
    case _image:
        /* code */
        kdrawimage(dev,screen,obj->data,obj->sdx,obj->sdy
        ,obj->width,obj->height);
        break;
    case _imagebig:
        /* code */
        kdrawimagebig(dev,screen,obj->data,obj->sdx,obj->sdy
        ,obj->width,obj->height,obj->d_and_r&_d_mask);
        break;
    case _line:
        /* code */
        kline(dev,screen,obj->colorck,obj->sdx,obj->sdy
            ,obj->width,obj->height);
        break;
    case _fillcircle:
        /* code */
        kfillcircle(dev,screen,obj->colorck,obj->sdx,obj->sdy,obj->width);
        break;
    case _fillbox:
        /* code */
        kfillbox(dev,screen,obj->colorck,obj->sdx,obj->sdy,obj->width,obj->height);
        break;
    case _roundrect:
        /* code */
        kroundrect(dev,screen,obj->colorck,obj->sdx,obj->sdy
            ,obj->width,obj->height,obj->_type&_r_mask);
        break;
    case _fillroundrect:
        /* code */
        kfillroundrect(dev,screen,obj->colorck,obj->sdx,obj->sdy
            ,obj->width,obj->height,obj->_type&_r_mask);
        break;
    default:
        break;
    }
}

void kobjsdraw(k_draw_device* dev,KSC_window* screen,const ksc_obj_t* obj,uint8_t num){
    if(!dev || !screen || !obj)return;
    for(uint8_t i=0;i<num;i++){
        kobjdraw(dev,screen,&obj[i]);
    }
}


