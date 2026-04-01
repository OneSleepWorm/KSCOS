#include "../inc/osconnect.h"
#include "../inc/KSCconfig.h"

extern uint8_t pubilcdatabuf[CONNECT_BUFFER_SIZE];
#if defined(__USE_UART__) && defined(__USE_ARMCC__)
#include "../inc/Serial.h"

/**
 * @brief 初始化串口连接
 * 
 */
void kconnect_init(void){
    serial_init();
}

/**
 * @brief 发送字节
 * 
 * @param Byte 要发送的字节
 */
void kconnect_sendbyte(uint8_t Byte){
    serial_sendbyte(Byte);
}

/**
 * @brief 发送数组
 * 
 * @param Array 要发送的数组
 * @param Length 数组长度
 */
void kconnect_sendarray(uint8_t *Array, uint16_t Length){
    for(uint16_t i = 0; i < Length; i++){
        serial_sendbyte(Array[i]);
    }
}

/**
 * @brief 发送字符串
 * 
 * @param String 要发送的字符串
 */
void kconnect_sendstring(char *String){
    serial_sendstring(String);	
}

/**
 * @brief 初始化接收
 * 
 */
void kconnect_receive_init(void){
    serial_receive_init();
}

/**
 * @brief 反初始化接收
 * 
 */
void kconnect_receive_deinit(void){
    serial_receive_deinit();
}

/**
 * @brief 获取接收字节
 * 
 */
void kconnect_receive_getbyte(void){
    serial_getbyte();
}

/**
 * @brief 获取接收字符串
 * 
 * @return char* 接收的字符字符串
 */
char* kfgetc(void){
    return serial_fgetc();
}

#endif
