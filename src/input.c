#include "../inc/input.h"

//输入队列(预分配4个槽位)
static input_t input_queue[INPUT_QUEUE_SIZE];

//输入获取函数
input_t input_get(uint32_t input_id){
    for(uint8_t i=0;i<INPUT_QUEUE_SIZE;i++){
        if(input_queue[i].input_id==input_id){
            input_t temp=input_queue[i];
            input_queue[i]=(input_t){0};
            return temp;
        }
    }
    return (input_t){0};
}

//输入添加函数
input_t input_add(input_t input){
    for(uint8_t i=0;i<INPUT_QUEUE_SIZE;i++){
        if(input_queue[i].input_id==0){
            input_queue[i]=input;
            break;
        }
    }
    return input;
}
