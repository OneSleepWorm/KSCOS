#include "osconnect.h"
extern UART_HandleTypeDef huart1;

int main(void)
{
    kconnect_init();
    kconnect_receive_init();
    kprintf("Hello world!\n");
    while(1){
        HAL_Delay(100);
        kprintf(kfgetc());
    }
    return 0;
}