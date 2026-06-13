#include "../inc/input.h"

//输入队列(预分配4个槽位)
static input_t input_queue[INPUT_QUEUE_SIZE];

/* @brief    输入获取函数
 * @note  从输入队列中获取指定ID的输入状态
 * @param input_id 输入ID
 * @return input_t 输入状态
 * @details  输入状态为input_t结构体，包含注册设备ID和key状态，具体的key状态根据设备类型而定
 *           输入缓冲队列大小为INPUT_QUEUE_SIZE，每个槽位可以存储一个输入状态，
 */
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

/* @brief    输入添加函数
 * @note  将输入状态添加到输入队列中
 * @param input 输入状态
 * @return input_t 输入状态(即将弃用而是返回状态码)
 * @details  输入状态为input_t结构体，包含注册设备ID和key状态，具体的key状态根据设备类型而定
 *           输入缓冲队列大小为INPUT_QUEUE_SIZE，每个槽位可以存储一个输入状态，
 *           如果输入队列已满，会跳过添加操作，
 */
input_t input_add(input_t input){
    for(uint8_t i=0;i<INPUT_QUEUE_SIZE;i++){
        if(input_queue[i].input_id==0){
            input_queue[i]=input;
            break;
        }
    }
    return input;
}
