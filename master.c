#include "inc/master.h"

k_draw_device dev={
    .init=screen_init,
    .setcanvas=screen_setcanvas,
    .setcolorpixels=screen_setcolorpixels,
};

ksc_obj_t objbox(uint8_t x,uint8_t y,uint8_t width,uint8_t height){
    return (ksc_obj_t){
        .sdx=x,
        .sdy=y,
        .width=width,
        .height=height,
        .colorck=rred,
        ._type=_box,
    };
}

int main(void){
    kscreenmount(&dev);
  static ksc_obj_t objb[20];
  static ksc_dirty_rect drect[5];
  KSC_window screen={
      .ssx=0,
      .ssy=0,
      .width=240,
      .height=160,
      .bk=wwhite,
      .objbuf=objb,
      .dirty_rect_buf=drect,
      .dirty_rect_num=0,
      .objnum=4,
  };

  kfull(&dev,&screen,wwhite,0,0,240,160);
  objb[0]=objbox(0,0,30,30);
  objb[1]=objbox(35,0,30,30);
  objb[2]=objbox(0,35,30,30);
  objb[3]=objbox(35,35,30,30);
  kobjsdraw(&dev,&screen,screen.objbuf,4);
  kfull(&dev,&screen,wwhite,0,0,240,160);
  drect[0]=(ksc_dirty_rect){20,20,10,50};
  drect[1]=(ksc_dirty_rect){20,40,60,50};
  screen.dirty_rect_num=2;
  kscreenupdate(&dev,&screen);

  while(1){
    //kscreenupdate(&dev,&screen);
  }
    return 0;
}