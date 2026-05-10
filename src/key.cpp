//#include "main.h"
#include "../inc/key.h"
#ifdef __USE_INPUT_KEY__

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

#ifdef __USE_INPUT_KEY_SIMU__
#include <graphics.h>

ExMessage key_msg;

void key_init(void){
    // initgraph(160,80);
    return;
}

uint8_t key_scan(void){
    if (!peekmessage(&key_msg,EX_KEY)) return KEY_NONE;
    if(key_msg.message == WM_KEYDOWN){
        switch (key_msg.vkcode){
            case VK_NUMPAD0:
                return KEY_A0;
                break;
            case VK_NUMPAD1:
                return KEY_A1;
                break;
            case VK_NUMPAD2:
                return KEY_A2;
                break;
            case VK_NUMPAD3:
                return KEY_A3;
                break;
            case VK_NUMPAD4:
                return KEY_A4;
                break;
            case VK_NUMPAD5:
                return KEY_A5;
                break;
            case VK_NUMPAD6:
                return KEY_A6;
                break;
            case VK_NUMPAD7:
                return KEY_A7;
                break;
            case VK_NUMPAD8:
                return KEY_A8;
                break;
            case VK_NUMPAD9:
                return KEY_A9;
                break;
            default:
                return KEY_NONE;
                break;
        }
    }
    return KEY_NONE;
}

#endif


//按键读取,获取按键持续状态
uint8_t key_read(void){
    #if __BUTTON_SIMU__
    return key_scan();
    #else 
    static uint8_t lastkey= KEY_NONE;
    static uint8_t nowkey=KEY_NONE;
    static uint8_t realkey=KEY_NONE;
    static uint16_t timer =0;
    nowkey = key_scan();
    if(nowkey != lastkey){
        if(nowkey != KEY_NONE){
            realkey = nowkey|KEY_PRESS;
        }else if(nowkey == KEY_NONE && lastkey != KEY_NONE){
            realkey = lastkey|KEY_RELEASE;
            timer =0;
        }
    }else{
        if(nowkey == KEY_NONE){realkey = nowkey;
        }else{
            timer++;
            #if _USE_KEY_KEEP_ > 0
            if(timer >= _USE_KEY_KEEP_){
                realkey = nowkey|KEY_KEEP_FULL;
            }else{
                realkey = nowkey|KEY_KEEP_NOFULL;
            }
            #else
            realkey = nowkey|KEY_KEEP_NOFULL;
            #endif
        }
    }
    lastkey = nowkey;
    return realkey;
    #endif
}


