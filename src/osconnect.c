#include "../inc/osconnect.h"
#include "../inc/KSCconfig.h"

extern uint8_t pubilcdatabuf[CONNECT_BUFFER_SIZE];
#if defined(__USE_UART__) && defined(__USE_ARMCC__)
#include "Serial.h"

void kconnect_init(void){
    serial_init();
}

void kconnect_sendbyte(uint8_t Byte){
    serial_sendbyte(Byte);
}

void kconnect_sendarray(uint8_t *Array, uint16_t Length){
    for(uint16_t i = 0; i < Length; i++){
        serial_sendbyte(Array[i]);
    }
}

void kconnect_sendstring(char *String){
    serial_sendstring(String);	
}

#endif
