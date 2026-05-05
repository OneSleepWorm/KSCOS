#include "../inc/KSCobj.h"

void kdrawimagefile(KSC_buf* screen,Img_File* imgfile,uintxy x,uintxy y){
    if(!imgfile){return;}
    //解析图片数据
    #ifndef __USE_IMGNAME__
        uint16_t* imgbuf = (uint16_t*)imgfile->data+4;
        uint8_t width = *((uint8_t*)imgfile->data+3);
        uint8_t height = *((uint8_t*)imgfile->data+5);
        kdrawimage(screen,imgbuf,x,y,width,height);
    #else
    const char* imgname = imgfile->name;//1,通过图片文件名获取图片属性
    if(!imgname){printf("imgname is null\n");return;}
    uint8_t namelen = strlen(imgname);
    //扫描文件名后四位
    if(strcmp(imgname+namelen-4,".bmp")==0)return;
    else if(strcmp(imgname+namelen-4,".jpg")==0)return;
    else if(strcmp(imgname+namelen-4,".png")==0)return;
    else if(strcmp(imgname+namelen-4,".gif")==0)return;
    else if(strcmp(imgname+namelen-4,".img")==0){
        uint16_t* imgbuf = (uint16_t*)imgfile->data+4;
        uint8_t width = *((uint8_t*)imgfile->data+3);
        uint8_t height = *((uint8_t*)imgfile->data+5);
        kdrawimage(screen,imgbuf,x,y,width,height);
    }else {printf("imgname:%s is not img file\n",imgname+namelen-4);}
    #endif
}

void kdrawimagebigfile(KSC_buf* screen,Img_File* imgfile,uintxy x,uintxy y,uint8_t scale){
    if(!imgfile){return;}
    //解析图片数据
    #ifndef __USE_IMGNAME__
        uint16_t* imgbuf = (uint16_t*)imgfile->data+4;
        uint8_t width = *((uint8_t*)imgfile->data+3);
        uint8_t height = *((uint8_t*)imgfile->data+5);
        kdrawimagebig(screen,imgbuf,x,y,width,height,scale);
    #else
    const char* imgname = imgfile->name;//1,通过图片文件名获取图片属性
    if(!imgname){printf("imgname is null\n");return;}
    uint8_t namelen = strlen(imgname);
    //扫描文件名后四位
    if(strcmp(imgname+namelen-4,".bmp")==0)return;
    else if(strcmp(imgname+namelen-4,".jpg")==0)return;
    else if(strcmp(imgname+namelen-4,".png")==0)return;
    else if(strcmp(imgname+namelen-4,".gif")==0)return;
    else if(strcmp(imgname+namelen-4,".img")==0){
        uint16_t* imgbuf = (uint16_t*)imgfile->data+4;
        uint8_t width = *((uint8_t*)imgfile->data+3);
        uint8_t height = *((uint8_t*)imgfile->data+5);
        kdrawimagebig(screen,imgbuf,x,y,width,height,scale);
    }else {printf("imgname:%s is not img file\n",imgname+namelen-4);}
    #endif
}

void kobjdraw(KSC_buf* screen,const ksc_obj_t* obj,uintxy x,uintxy y,const void* extradata){
    if(!obj)return;
    // x = obj->x;
    // y = obj->y;
    if(obj->state&0x01)return;
    switch(obj->_type&_type_mask){
        case _circle:
            kcircle(screen,obj->colorck,x+obj->sdx,y+obj->sdy,obj->d_and_r&0x1F);
            break;
        case _box:
            kbox(screen,obj->colorck,x+obj->sdx,y+obj->sdy,obj->width,obj->height);
            break;
        case _string:
            kstring(screen,(char*)obj->data,x+obj->sdx,y+obj->sdy,obj->colorck,obj->colorbk);
            break;
        case _image:
            kdrawimagefile(screen,((Img_File*)obj->data),x+obj->sdx,y+obj->sdy);
            break;
        case _roundrect:
            kroundrect(screen,obj->colorck,x+obj->sdx,y+obj->sdy,obj->width,obj->height,obj->d_and_r&0x1F);
            break;
        case _fillroundrect:
            kfillroundrect(screen,obj->colorck,x+obj->sdx,y+obj->sdy,obj->width,obj->height,obj->d_and_r&0x1F);
            break;
        case _fillbox:
            kfillbox(screen,obj->colorck,x+obj->sdx,y+obj->sdy,obj->width,obj->height);
            break;
        case _fillcircle:
            kfillcircle(screen,obj->colorck,x+obj->sdx,y+obj->sdy,obj->d_and_r&0x1F);
            break;
        case _line:
            kline(screen,obj->colorck,x+obj->sdx,y+obj->sdy,x+obj->sdx+obj->width,y+obj->sdy+obj->height);
            break;
        case _imagebig:
            kdrawimagebigfile(screen,((Img_File*)obj->data),x+obj->sdx,y+obj->sdy,(obj->d_and_r>>5));
            break;
        case _string_extra:
            kstring(screen,(char*)extradata,x+obj->sdx,y+obj->sdy,obj->colorck,obj->colorbk);
            break;
        case _image_extra:
            kdrawimagefile(screen,((Img_File*)extradata),x+obj->sdx,y+obj->sdy);
            break;
        case _imagebig_extra:
            kdrawimagebigfile(screen,((Img_File*)extradata),x+obj->sdx,y+obj->sdy,(obj->d_and_r>>5));
            break;
        default:
            break;
    }
}

void KSC_menu_clear(KSC_buf* screen,ksc_menu_t* menu,uintxy x,uintxy y){
    uint8_t hnum = menu->config->menu_hnum;
    uint8_t wnum = menu->config->menu_wnum;
    kfull(screen,screen->bk,x,y
        ,menu->config->mdw*menu->config->menu_wnum
        ,menu->config->mdh*menu->config->menu_hnum);
}

void KSC_menu_draw(KSC_buf* screen,ksc_menu_t* menu,uintxy x,uintxy y){
    uint8_t hnum = menu->config->menu_hnum;
    uint8_t wnum = menu->config->menu_wnum;
    KSCCOLOR obk;
    KSCCOLOR nbk;
    for(uint8_t j=0;j<hnum;j++){
        for(uint8_t i=0;i<wnum;i++){
            if(menu->config->menu_num<=i+wnum*j) return;
            KSCCOLOR bk = screen->bk;
            for(uint8_t k=0;k<menu->config->menu_obj_num;k++){
                void* extradata = NULL;
                if(menu->style[k]->_type==(_image_extra&_type_mask))
                    extradata = (void*)&menu->list[i+wnum*j].data;
                else if(menu->style[k]->_type==(_string_extra|_custom_label1)){
                    extradata = (void*)menu->list[i+wnum*j].name;
                    // printf("drawname:%s\n",extradata);
                }else if(menu->style[k]->_type==(_string_extra|_custom_label2)){
                        //文件属性
                        if(menu->list[i+wnum*j].type==2){//文件
                            extradata = "[dir]";
                        }else if(menu->list[i+wnum*j].type==1){//文件
                            extradata = "[file]"; 
                        }
                }else if((menu->style[k]->_type&_custom_mask)==_custom_label3){
                    //复选框，判断是否选中
                    if(menu->config->menu_index!=i+wnum*j){
                        continue;
                    }
                    nbk = menu->style[k]->colorbk;
                    obk = menu->style[k]->colorck;
                    uint8_t objnum = menu->config->menu_obj_num;
                }else{
                    extradata = NULL;
                }
                // printf("extradata:%s\n",extradata);
                //menu->style[k]->colorbk = bk;
                kobjdraw(screen,menu->style[k]
                    ,x+menu->config->mdw*i
                    ,y+menu->config->mdh*j,
                    extradata);
                

            }
        }
    }
    
    
}

#if __USE_KEY__ > 0

void KSC_menu_update(KSC_buf* screen,ksc_menu_t* menu,uintxy x,uintxy y,uint8_t key){
    if(!menu) return;
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
