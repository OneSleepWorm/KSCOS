#ifndef _MENU_H_
#define _MENU_H_

#include "KSCobj.h"
#include "KSCimg.h"

ksc_menu_t* filemenu_init(KSC_buf* screen);
int filemenu_update(ksc_menu_t* menu,uint8_t key);
void filemenu_flashscreen(KSC_buf* screen, ksc_menu_t* menu,uint8_t key, uint8_t x, uint8_t y);

#endif