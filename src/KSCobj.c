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


