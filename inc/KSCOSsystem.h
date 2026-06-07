#ifndef __KSCOSSYSTEM_H__
#define __KSCOSSYSTEM_H__


#define KSCOS_LOW_CLOCK 0
#define KSCOS_NORMAL_CLOCK 1
#define KSCOS_HIGH_CLOCK 2

void KSCOSsystem_Init(void);
void KSCOSSystemClock_Init(unsigned char clock_type);


#endif
