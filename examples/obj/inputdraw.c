#include "inc/master.h"

/////////////////////////////////////////////////////////
/**
 * @antuor   OneSleepWorm(一只瞌睡虫)
 * @brief    定时任务,扫描按键,获取按键状态变化
 * @note     使用定时器1实现10ms的矩阵按键扫描并获取按键状态变化
 */
/////////////////////////////////////////////////////////

// 定时任务回调函数
ki8 myclocktask_callback(void* user_data){
    // 扫描按键,获取按键状态变化
    ((input_device_t*)user_data)->input_create();
    return 0;
}
// 数字转换为字符串
char* numtostr(uint32_t num){
    static char str[16];
    char* p = str + sizeof(str) - 1;
    *p = '\0';
    do {
        *--p = '0' + num % 10;
        num /= 10;
    } while (num);
    return p;
}
// 数字转换为二进制字符串
char* numtobin(uint32_t num, uint8_t bits){
    static char str[40];
    char* p = str;
    for(int i=bits-1; i>=0; i--){
        if(i!=bits-1 && (i+1)%4==0) *p++ = ' ';
        *p++ = (num >> i) & 1 ? '1' : '0';
    }
    *p = '\0';
    return str;
}
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
        default:return '?';
    }
}
int main(void)
{

  // 初始化系统
  KSCOSsystem_Init();

  KSCOSSystemClock_Init(KSCOS_NORMAL_CLOCK);

  /* USER CODE BEGIN 2 */
k_draw_device* devp = kscreenmount();
KSC_window screen={0};
// kfull(devp,&screen,wwhite,0,0,240,160);
  screen.ssx=0;
  screen.ssy=25;
  screen.width=160;
  screen.height=80;
  screen.bk=wwhite;

  kfull(devp,&screen,bblack,0,0,160,81);

  // kstring(devp,&screen,"KSCdraw Basic Shapes",5,2,rred,wwhite);

  // char cdata = '3';
  char cdatastr[40] ="hello world";
  ksc_obj_t obj1;
  obj1._type = _string;
  obj1.colorck = bblack;
  obj1.data = (void*)&cdatastr;
  obj1.sdx = 10;
  obj1.sdy = 10;
  // kobjdraw(devp,&screen,&obj1);
  // 创建按键输入结构体
  input_device_t keydevice = key_default_device;
  keydevice.input_init();
  // 创建定时任务初始化
  clock_task_t clocktask1= clock_default_task;
  clocktask1.task_cycle = 10;
  // 定时任务回调函数绑定create函数
  clocktask1.callback = myclocktask_callback;
  clocktask1.user_data = (void*)&keydevice;
  clocktask1.init(&clocktask1);
  clocktask1.run(&clocktask1);

  while (1)
  {
  
    input_t keyinput = input_get(KEY_DEVICE_ID);
    if(keyinput.data!=0){
        strcpy(cdatastr,numtobin(keyinput.data,16));
    }
    
    kobjdraw(devp,&screen,&obj1);


  }
}