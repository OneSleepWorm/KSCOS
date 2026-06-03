#include "inc/master.h"

int main(void){
    k_draw_device* devp = kscreenmount();

  KSC_window screen={
      .ssx=0,
      .ssy=0,
      .width=240,
      .height=160,
      .bk=wwhite,
  };

  kfull(devp,&screen,wwhite,0,0,240,160);

  kstring(devp,&screen,"KSCdraw Basic Shapes",5,2,rred,wwhite);

  kline(devp,&screen,bblue,5,20,100,20);
  kstring(devp,&screen,"kline",5,22,bblue,wwhite);

  kbox(devp,&screen,rred,5,35,30,30);
  kstring(devp,&screen,"kbox",5,68,rred,wwhite);

  kfull(devp,&screen,ggreen,5,78,30,30);
  kstring(devp,&screen,"kfillbox",5,110,ggreen,wwhite);

  kcircle(devp,&screen,bblack,75,50,15);
  kstring(devp,&screen,"kcircle",55,68,bblack,wwhite);

  kfillcircle(devp,&screen,bblue,75,95,15);
  kstring(devp,&screen,"kfillcircle",55,112,bblue,wwhite);

  kroundrect(devp,&screen,rred,120,20,50,40,8);
  kstring(devp,&screen,"kroundrect",115,62,rred,wwhite);

  kfillroundrect(devp,&screen,ggreen,120,78,50,40,8);
  kstring(devp,&screen,"kfillroundrect",110,120,ggreen,wwhite);

  kstring(devp,&screen,"Hello World!",120,140,bblack,wwhite);

  while(1){

  }
    return 0;
}