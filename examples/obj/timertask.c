#include "../inc/master.h"

/////////////////////////////////////////////////////////
/**
 * @antuor   OneSleepWorm(一只瞌睡虫)
 * @brief    定时任务回调函数
 * @note     使用定时器2实现1秒的定时任务回调
 */
/////////////////////////////////////////////////////////

void myclocktask_callback(void* user_data){
    (*(char*)user_data)++;
}

int main(void)
{


  KSCOSsystem_Init();

  KSCOSSystemClock_Init(KSCOS_NORMAL_CLOCK);
  
  MX_GPIO_Init();
  /* USER CODE BEGIN 2 */
k_draw_device* devp = kscreenmount();
KSC_window screen={0};
// kfull(devp,&screen,wwhite,0,0,240,160);
  screen.ssx=0;
  screen.ssy=25;
  screen.width=160;
  screen.height=80;
  screen.bk=wwhite;

  // kfull(devp,&screen,wwhite,0,0,160,81);

  // kstring(devp,&screen,"KSCdraw Basic Shapes",5,2,rred,wwhite);

  char cdata = '1';
  ksc_obj_t obj1;
  obj1._type = _char;
  obj1.colorck = bblack;
  obj1.data = (void*)&cdata;
  obj1.sdx = 10;
  obj1.sdy = 10;
  kobjdraw(devp,&screen,&obj1);
  stm_clock_task.init((clock_task_t*)&stm_clock_task);
  stm_clock_task.callback = myclocktask_callback;
  stm_clock_task.user_data = (void*)&cdata;
  stm_clock_task.run((clock_task_t*)&stm_clock_task);

  while (1)
  {
    kobjdraw(devp,&screen,&obj1);
    if(cdata>'z') cdata='1';

  }
}