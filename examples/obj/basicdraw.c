#include "../inc/master.h"

k_draw_device dev={
    .init=screen_init,
    .setcanvas=screen_setcanvas,
    .setcolorpixels=screen_setcolorpixels,
};

int main(void){
    kscreenmount(&dev);

  KSC_window screen={
      .ssx=0,
      .ssy=0,
      .width=240,
      .height=160,
      .bk=wwhite,
  };

  kfull(&dev,&screen,wwhite,0,0,240,160);

  kstring(&dev,&screen,"KSCdraw Basic Shapes",5,2,rred,wwhite);

  kline(&dev,&screen,bblue,5,20,100,20);
  kstring(&dev,&screen,"kline",5,22,bblue,wwhite);

  kbox(&dev,&screen,rred,5,35,30,30);
  kstring(&dev,&screen,"kbox",5,68,rred,wwhite);

  kfull(&dev,&screen,ggreen,5,78,30,30);
  kstring(&dev,&screen,"kfillbox",5,110,ggreen,wwhite);

  kcircle(&dev,&screen,bblack,75,50,15);
  kstring(&dev,&screen,"kcircle",55,68,bblack,wwhite);

  kfillcircle(&dev,&screen,bblue,75,95,15);
  kstring(&dev,&screen,"kfillcircle",55,112,bblue,wwhite);

  kroundrect(&dev,&screen,rred,120,20,50,40,8);
  kstring(&dev,&screen,"kroundrect",115,62,rred,wwhite);

  kfillroundrect(&dev,&screen,ggreen,120,78,50,40,8);
  kstring(&dev,&screen,"kfillroundrect",110,120,ggreen,wwhite);

  kstring(&dev,&screen,"Hello World!",120,140,bblack,wwhite);
  kfillcircle(&dev,&screen,bblack,75,50,15);
    return 0;
}