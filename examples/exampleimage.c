#include "KSCbasicdrawN.h"
void testimg(KSC_buf* screen){
	for(uint8_t dy=0;dy<3;dy++){
		for(uint8_t dx=0;dx<4;dx++){
			kdrawimagebig(screen,dx*60,dy*80,Systemimg[dy*4+dx].data,16,16,3);
			kstring(screen,Systemimg[dy*4+dx].name,dx*60,dy*80+54);
		}
	}
}
int main(){
    KSC_buf* screen = kinitscreen(240,320,NULL,0,0);
    kdrawimagebig(kgetscreen(),0,0,OneSleepWorm,40,40,6);
	ksetpencolor(screen,GREEN,0);
	ksetpencolor(screen,BLUE,1);
    testimg(screen);
}