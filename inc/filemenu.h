#ifndef _MENU_H_
#define _MENU_H_

#include "KSCobj.h"
#include "KSCimg.h"

ksc_menu_t* filemenu_init(KSC_buf* screen);
int filemenu_update(ksc_menu_t* menu,uint8_t key);


#endif