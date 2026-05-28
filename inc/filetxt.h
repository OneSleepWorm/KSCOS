#ifndef __FILETXT_H
#define __FILETXT_H

#include "KSCdraw.h"
#include "KSCconfig.h"
#if __USE_TEXT__

typedef struct k_txt_config{
    char* data;
    uint16_t lenmax;
    uint16_t index;
}k_txt_config;

k_txt_config txtconfig_set(char* data);
KSC_window txtscreeninit(k_draw_device* dev);
void txtdataupdate(KSC_window* screen,k_txt_config* config);



#endif

#endif
