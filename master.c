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

char txt_big_data[700]="Key design decisions:\
- **Static memory**: A single `static uint8_t draw_buf[_STATICBUF_SIZE]` (512 bytes) in KSCdraw.c serves all temporary buffer needs (`kfull`, `kimagebin`, `kchar`). \
These operations are mutually exclusive (no reentrancy needed), so a single static buffer eliminates heap allocation overhead.\
- **Fast paths**: Horizontal and vertical lines are detected in `kline` and delegated to `kfull` for batch pixel writes, avoiding per-pixel `ksetpixel` overhead.\
- `kfillcircle` uses a fixed-size `int left[256]` stack array (no VLA) for portability.";

int main(void){
    kdevmount(&dev);
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
  KSC_window txt_screen=txtscreeninit(&dev);
//   kfull(&dev,&txt_screen,rred,0,0,100,100);
 char txt_data[13]="Hello World!";
  k_txt_config txt_config=txtconfig_set(txt_big_data);
  
  


  while(1){
    sleep(2);
    txtdataupdate(&txt_screen,&txt_config);

    kscreendraw(&dev,&txt_screen);
  }
    return 0;
}