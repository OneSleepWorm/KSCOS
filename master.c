#include "inc/master.h"
#include <windows.h>

ksc_obj_t obj1;

void myclocktask_callback(void* user_data){
    (*(char*)user_data)+=2;
    printf("data:%c\n",*(char*)user_data);
}

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

  // kstring(devp,&screen,"KSCdraw Basic Shapes",5,2,rred,wwhite);

  // kline(devp,&screen,bblue,5,20,100,20);
  // kstring(devp,&screen,"kline",5,22,bblue,wwhite);

  // kbox(devp,&screen,rred,5,35,30,30);
  // kstring(devp,&screen,"kbox",5,68,rred,wwhite);

  // kfull(devp,&screen,ggreen,5,78,30,30);
  // kstring(devp,&screen,"kfillbox",5,110,ggreen,wwhite);

  // kcircle(devp,&screen,bblack,75,50,15);
  // kstring(devp,&screen,"kcircle",55,68,bblack,wwhite);

  // kfillcircle(devp,&screen,bblue,75,95,15);
  // kstring(devp,&screen,"kfillcircle",55,112,bblue,wwhite);

  // kroundrect(devp,&screen,rred,120,20,50,40,8);
  // kstring(devp,&screen,"kroundrect",115,62,rred,wwhite);

  // kfillroundrect(devp,&screen,ggreen,120,78,50,40,8);
  // kstring(devp,&screen,"kfillroundrect",110,120,ggreen,wwhite);

  // kstring(devp,&screen,"Hello World!",120,140,bblack,wwhite);

  // sleep(4);
  char cdata = '1';
  ksc_obj_t obj1;
  obj1._type = _char;
  obj1.colorck = bblack;
  obj1.data = (void*)&cdata;
  obj1.sdx = 10;
  obj1.sdy = 10;
  kobjdraw(devp,&screen,&obj1);
  clock_task_t clocktask=pc_clock_task;
  // clocktask.init(&clocktask);
  clocktask.callback = myclocktask_callback;
  clocktask.user_data = (void*)&cdata;
  clocktask.run(&clocktask);
  // clocktask.stop(&clocktask);
  while(1){
    Sleep(1000);
    printf("[main]data:%s\n",&cdata);
    kobjdraw(devp,&screen,&obj1);
  }
  clocktask.stop(&clocktask);
    return 0;
}