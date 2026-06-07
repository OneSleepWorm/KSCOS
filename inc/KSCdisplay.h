#ifndef __KSC_DISPLAY_H__
#define __KSC_DISPLAY_H__

#include "KSCconfig.h"
#if __USE_ARMCC__
#include "TFTDriver.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

void screen_init(void);
void screen_setcolorpixels(const KSCCOLOR* color,uint16_t num);
void screen_setcanvas(uintxy Gx,uintxy Gy, uintxy width,uintxy height);

#ifdef __cplusplus
}
#endif

#endif
