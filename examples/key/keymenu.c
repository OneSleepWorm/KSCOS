#include "main.h"
extern SPI_HandleTypeDef hspi1;
#include "key.h"
#include "menu.h"

int main(void)
{
    KSC_buf* screen = kinitscreen(240, 320, NULL, 0, 0);
  //kfillrect(screen,0,0,239,319);
  flash_init();
  kconnect_init();
  kreceive_init();
    key_init();
    menu_init(screen);
    uint8_t key;
    while(1){
        key=key_read();
        if(KEY_LOG(key)==1)menu_move(KEY_PSY(key));
    }

}