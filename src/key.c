//#include "main.h"
#include "../inc/key.h"
#if __USE_KEY__

#if __USE_STM32__
#include "stm32f1xx_hal.h"


#define KEY_TICK_TIME 10

#define keyrow0 GPIO_PIN_0
#define keyrow1 GPIO_PIN_1
#define keyrow2 GPIO_PIN_2
#define keyrow3 GPIO_PIN_3
#define keycol0 GPIO_PIN_4
#define keycol1 GPIO_PIN_5
#define keycol2 GPIO_PIN_6
#define keycol3 GPIO_PIN_7

#define keysetgpio(pin,level) HAL_GPIO_WritePin(GPIOA, pin, level)
#define keygetgpio(pin) HAL_GPIO_ReadPin(GPIOA, pin)
ki8 key_init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    __HAL_RCC_GPIOA_CLK_ENABLE();

    // PA0~PA3：推挽输出
    GPIO_InitStruct.Pin = keyrow0|keyrow1|keyrow2|keyrow3;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    // PA4~PA7：下拉输入
    GPIO_InitStruct.Pin = keycol0|keycol1|keycol2|keycol3;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    // 所有行输出低
    keysetgpio(keyrow0|keyrow1|keyrow2|keyrow3,0);
    return 0;
}

//按键扫描,获取按键物理状态
uint32_t key_scan(void)
{
    uint32_t key = 0;

    // 行0（PA0）→ A0 A1 A2 A3
    keysetgpio(keyrow0,1);
    keysetgpio(keyrow1|keyrow2|keyrow3,0);
    key|=keygetgpio(keycol0)<<0;
    key|=keygetgpio(keycol1)<<1;
    key|=keygetgpio(keycol2)<<2;
    key|=keygetgpio(keycol3)<<3;
    keysetgpio(keyrow1,1);
    keysetgpio(keyrow0|keyrow2|keyrow3,0);
    key|=keygetgpio(keycol0)<<4;
    key|=keygetgpio(keycol1)<<5;
    key|=keygetgpio(keycol2)<<6;
    key|=keygetgpio(keycol3)<<7;
    keysetgpio(keyrow2,1);
    keysetgpio(keyrow0|keyrow1|keyrow3,0);
    key|=keygetgpio(keycol0)<<8;
    key|=keygetgpio(keycol1)<<9;
    key|=keygetgpio(keycol2)<<10;
    key|=keygetgpio(keycol3)<<11;
    keysetgpio(keyrow3,1);
    keysetgpio(keyrow0|keyrow1|keyrow2,0);
    key|=keygetgpio(keycol0)<<12;
    key|=keygetgpio(keycol1)<<13;
    key|=keygetgpio(keycol2)<<14;
    key|=keygetgpio(keycol3)<<15;
    return key;
}
uint32_t key_scan_ex(void){
    static uint32_t lastkey0 = 0;
    static uint32_t lastkey1 = 0;
    static uint32_t statekey0 = 0;
    static uint32_t statekey1 = 0;
    uint32_t nowkey = key_scan();
    // 状态机获取,得到三次消抖后的按键状态
    statekey0 = lastkey0 & nowkey;
    // statekey0 = nowkey;
    // 状态机更新,得到按键状态变化,低十六位记录按键状态，高十六位记录按键上下沿
    uint32_t realkey = ((statekey1^statekey0)<<16)|(statekey0&0xFFFF);
    // 状态机复位
    lastkey1 = lastkey0;
    lastkey0 = nowkey;

    statekey1 = statekey0;
    // 状态机返回按键状态变化
    return realkey;
}
input_t key_create(void){
    // 扫描按键,获取按键状态变化
    return input_add((input_t){key_scan_ex(),KEY_DEVICE_ID});

}
ki8 key_deinit(void){
    return 0;
}
#endif

#if __USE_PC__
#include "../third_party/easyx/include/graphics.h"
#include "../third_party/easyx/include/easyx.h"
#ifdef __cplusplus
extern "C" {
#endif
// const uint32_t pc_key_device_id = PC_KEY_DEVICE_ID;
ExMessage key_msg;


uint16_t pc_key_scan(void){
    if (!peekmessage(&key_msg,EX_KEY)) return KEY_NONE;
    if(key_msg.message == WM_KEYDOWN){
        switch (key_msg.vkcode){
            case '1':  return KEY_A0;break;
            case '2':  return KEY_A1;break;
            case '3':  return KEY_A2;break;
            case '4':  return KEY_A3;break;
            case 'Q':  return KEY_A4;break;
            case 'W':  return KEY_A5;break;
            case 'E':  return KEY_A6;break;
            case 'R':  return KEY_A7;break;
            case 'A':  return KEY_A8;break;
            case 'S':  return KEY_A9;break;
            case 'D':  return KEY_A10;break;
            case 'F':  return KEY_A11;break;
            case 'Z':  return KEY_A12;break;
            case 'X':  return KEY_A13;break;
            case 'C':  return KEY_A14;break;
            case 'V':  return KEY_A15;break;
            default:  return KEY_NONE;break;
        }
    }
    return KEY_NONE;
}

input_t pc_key_create(void){
    return input_add((input_t){pc_key_scan(),KEY_DEVICE_ID});
}
#define key_create pc_key_create
#ifdef __cplusplus
}
#endif
ki8 key_init(void){
    return 0;
}    
ki8 key_deinit(void){
    return 0;
}
#endif //__USE_PC__ > 0

/* @brief    默认按键驱动
 * @note  每次调用input_create()函数，都会将当前按键状态添加到输入队列中
 *           并返回一个input_t结构体，包含按键状态和设备ID，key值为32位，
 *           但通常我们会将create函数调用在定时任务上，
 *           因此可以调用input_get()函数统一获取按键状态
 * @details  pc端的key位定义与stm不同，PC端的key值实际为16位，高16位不会被使用，
 *           且按键状态为第三方库插值后的逻辑状态，无需处理，
 *           而stm端的key值实际为32位，低16位为按键物理状态，高16位为按键上下沿
 *           每次调用都是物理按键状态，需要额外用户端处理状态机
 * 
 */
input_device key_default_device ={
    .device_id = KEY_DEVICE_ID,
    .input_init = key_init,
    .input_create = key_create,
    .input_deinit = key_deinit,
};



#endif
