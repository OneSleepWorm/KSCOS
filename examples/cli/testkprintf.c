//testkprintf.c
//强调：uart1初始化一定要放系统自动初始化之后，否则中断配置会失败

//--locale=english --no-multibyte-chars
#include "osconnect.h"
extern UART_HandleTypeDef huart1;

int main(void)
{
    kconnect_init();
    kconnect_receive_init();
    kprintf("Hello world!\n");
    while(1){
        HAL_Delay(100);
        kconnect_sendstring(kfgetc());
        //kprintf(kfgetc());
    }
    return 0;
}
