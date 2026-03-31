#include "KSCbasicdrawN.h"

int main(){
KSC_buf* screen = kinitscreen(160,80,NULL,0,0);
    ksetpencolor(screen,GREEN,1);
    kroundrect(screen,20,20,50,50,10);
    kfillcircle(screen,70,40,15);
    kcircle(screen,100,40,10);
    kdrawimagebig(screen,120,40,Wechat,16,16,2);
    ksetpencolor(screen,RED,1);
    kstring(screen,"Hello World",10,0);
}