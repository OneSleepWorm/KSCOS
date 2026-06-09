//#include "main.h"
#include "../inc/key.h"
#if __USE_KEY__

#if __USE_STM32__
void key_init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    __HAL_RCC_GPIOA_CLK_ENABLE();

    // PA0~PA3：推挽输出
    GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    // PA4~PA7：上拉输入
    GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    // 所有行输出高
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, GPIO_PIN_SET);
}

//按键扫描,获取按键物理状态
uint8_t key_scan(void)
{
    uint8_t key = KEY_NONE;

    // 行0（PA0）→ A0 A1 A2 A3
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, GPIO_PIN_SET);
    if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_4)==0) key=KEY_A0;
    else if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_5)==0) key=KEY_A4;
    else if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_6)==0) key=KEY_A8;
    else if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_7)==0) key=KEY_A12;

    if(key != KEY_NONE) return key;

    // 行1（PA1）→ A4 A5 A6 A7
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0|GPIO_PIN_2|GPIO_PIN_3, GPIO_PIN_SET);
    if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_4)==0) key=KEY_A1;
    else if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_5)==0) key=KEY_A5;
    else if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_6)==0) key=KEY_A9;
    else if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_7)==0) key=KEY_A13;

    if(key != KEY_NONE) return key;

    // 行2（PA2）→ A8 A9 A10 A11
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_3, GPIO_PIN_SET);
    if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_4)==0) key=KEY_A2;
    else if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_5)==0) key=KEY_A6;
    else if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_6)==0) key=KEY_A10;
    else if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_7)==0) key=KEY_A14;

    if(key != KEY_NONE) return key;

    // 行3（PA3）→ A12 A13 A14 A15
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2, GPIO_PIN_SET);
    if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_4)==0) key=KEY_A3;
    else if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_5)==0) key=KEY_A7;
    else if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_6)==0) key=KEY_A11;
    else if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_7)==0) key=KEY_A15;

    return key;
}
#endif

#ifdef __USE_PC__
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
    return input_add((input_t){pc_key_scan(),PC_KEY_DEVICE_ID});
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


input_device key_default_device{
    .device_id = KEY_DEVICE_ID,
    .input_init = key_init,
    .input_create = key_create,
    .input_deinit = key_deinit,
};



#endif
