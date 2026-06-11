#include "inc/master.h"
#include <windows.h>

char diskey(uint32_t keydata){
    switch(keydata){
        case KEY_A0:return '0';
        case KEY_A1:return '1';
        case KEY_A2:return '2';
        case KEY_A3:return '3';
        case KEY_A4:return '4';
        case KEY_A5:return '5';
        case KEY_A6:return '6';
        case KEY_A7:return '7';
        case KEY_A8:return '8'; 
        case KEY_A9:return '9';
        case KEY_A10:return 'A';  
        case KEY_A11:return 'B';  
        case KEY_A12:return 'C';  
        case KEY_A13:return 'D';    
        case KEY_A14:return 'E';
        case KEY_A15:return 'F';
        default:printf("keydata:%d\n",keydata);return '?';
    }
}

// 输入回调函数
ki8 keycallback(void* user_data){
    input_device* keydevice=(input_device*)user_data;
    keydevice->input_create();
    // printf("keyinput:%d,%d\n",keyinput.input_id,keyinput.data);
    return 0;
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
  // 初始化显示字符对象
  char cdata = '1';
  ksc_obj_t obj1;
  obj1._type = _char;
  obj1.colorck = bblack;
  obj1.data = (void*)&cdata;
  obj1.sdx = 10;
  obj1.sdy = 10;
  kobjdraw(devp,&screen,&obj1);
  // 初始化输入设备
  input_device keydevice= key_default_device;
  uint32_t keyid = key_default_device.device_id;
  printf("keyid:%d\n",keyid);
  keydevice.input_init();
  // 创建输入任务
  clock_task_t clocktask=
  clock_task_create(NULL,keycallback,(void*)&keydevice,100);
  clocktask.run(&clocktask);//绑定自己编写的输入回调函数，里面应input_create()
  cdata = '1';
  input_t keyinput;
  // 主循环
  while(1){
    Sleep(10);
    // 获取输入
    keyinput = input_get(keyid);
    // 显示输入字符
    if(keyinput.input_id==keyid && keyinput.data!=255){
      cdata = diskey(keyinput.data);

    printf("[main] id:%d,data:%d\n",keyinput.input_id,keyinput.data);
    }
    kobjdraw(devp,&screen,&obj1);
  }
  // clocktask.stop(&clocktask);
    return 0;
}