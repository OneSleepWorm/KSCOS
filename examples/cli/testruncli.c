//--locale=english --no-multibyte-chars
//强调：uart1初始化一定要放系统自动初始化之后，否则中断配置会失败
//但屏幕初始化尽量放系统自动初始化之前，否则若初始化修改了系统分频，时序会异常

#include "osconnect.h"
extern UART_HandleTypeDef huart1;
#include "KSCbasicdrawN.h"
#include "CLI.h"

int main(){
    kinitscreen(240,320,NULL,0,0);
    kconnect_init();
    kconnect_receive_init();
    kprintf("Hello world!\n");
    while(1){
            run_cli_root(kfgetc());
    }
}
