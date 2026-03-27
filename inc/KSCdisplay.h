#ifndef __KSC_DISPLAY_H__
#define __KSC_DISPLAY_H__

#include "KSCconfig.h"

#ifdef __USE_ARMCC__
#include "TFTDriver.h"
#endif

void screen_init();
void screen_setcolorpixel(KSCCOLOR color);
void screen_setcanvas(uintxy Gx,uintxy Gy, uintxy width,uintxy height);
// void setPix(uint8_t _pix);
// uint8_t getPix();


#endif
