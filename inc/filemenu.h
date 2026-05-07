#ifndef _MENU_H_
#define _MENU_H_

#include "KSCobj.h"
#include "KSCimg.h"

ksc_menu_t* filemenu_init(KSC_window* screen);
void kmenu_draw(KSC_window* screen, ksc_menu_t* menu);
void kmenu_update(KSC_window* screen, ksc_menu_t* menu);
void filemenu_flashscreen(KSC_window* screen, ksc_menu_t* menu,uint8_t key, uint8_t x, uint8_t y);

#endif