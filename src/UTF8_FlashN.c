#include "../inc/UTF8_FlashN.h"
#include "Serial.h"

//#define NULL ((void*)0)
/**
 * @brief 检测UTF-8字符串中指定位置的字符编码长度
 * @param str UTF-8编码的字符串指针
 * @param pos 字符串中要检测索引
 * @return 编码长度(1-4字节)，0表示无效编码
 */
static uint8_t utf8_char_len(const uint8_t *str, uint16_t pos)
{
    // 空指针
    if (str == NULL)
        return 0;

    uint8_t c = str[pos];

    // 字符串结束
    if (c == 0x00)
        return 0;

    // 1 字节 (ASCII)
    if ((c & 0x80) == 0x00)
        return 1;

    // 2 字节字符 110xxxxx
    if ((c & 0xE0) == 0xC0)
    {
        // 必须检查下一个字节不越界
        if (str[pos + 1] == 0)
            return 0;
        // 后续字节必须是 10xxxxxx
        if ((str[pos + 1] & 0xC0) != 0x80)
            return 0;
        return 2;
    }

    // 3 字节字符 1110xxxx
    if ((c & 0xF0) == 0xE0)
    {
        if (str[pos + 1] == 0 || str[pos + 2] == 0)
            return 0;
        if ((str[pos + 1] & 0xC0) != 0x80 || (str[pos + 2] & 0xC0) != 0x80)
            return 0;
        return 3;
    }

    // 4 字节字符 11110xxx
    if ((c & 0xF8) == 0xF0)
    {
        if (str[pos + 1] == 0 || str[pos + 2] == 0 || str[pos + 3] == 0)
            return 0;
        if ((str[pos + 1] & 0xC0) != 0x80 ||
            (str[pos + 2] & 0xC0) != 0x80 ||
            (str[pos + 3] & 0xC0) != 0x80)
            return 0;
        return 4;
    }

    // 非法 UTF8 编码
    return 0;
}
// 功能：获取 UTF-8 字符串前 n 个 Unicode 字符 占用的总字节数
// 参数：str = UTF8 串；n = 要取的字符个数
// 返回：总字节数（遇到字符串结束 / 非法编码会提前返回已读长度）
uint16_t utf8_prev_n_chars_bytes(const uint8_t *str, uint16_t n)
{
    if (str == NULL || n == 0)
        return 0;

    uint16_t total_bytes = 0;  // 累计字节数
    uint16_t char_count = 0;   // 已经读到的 Unicode 字符数

    while (1)
    {
        // 取当前位置一个 UTF8 字符的长度
        uint8_t c_len = utf8_char_len(str, total_bytes);

        // 遇到结束 / 非法编码，直接退出
        if (c_len == 0)
            break;

        char_count++;
        total_bytes += c_len;

        // 已经取够 n 个字符
        if (char_count >= n)
            break;
    }

    return total_bytes;
}
//获取字符串的总Unicode数量
uint16_t utf8_get_unicode_count(const uint8_t *str)
{
    uint16_t char_count = 0;
    uint16_t unicode_count = 0;
    uint8_t c_len = 0;
    while (str[char_count] != 0)
    {
        if((c_len = utf8_char_len(str, char_count)) == 0)return 0;
        char_count += c_len;
        unicode_count++;
    }
    return unicode_count;
}

/**
 * @brief 将UTF-8编码转换为Unicode码位
 * @param str UTF-8编码的字符串指针
 * @param start_idx 编码的起始位置索引
 * @param len 编码长度
 * @return Unicode码位
 */
uint32_t utf8_to_code(const uint8_t* str, uint16_t start_idx, uint8_t len) {
    uint32_t code = 0xFFFFFFFFU;
    
    switch (len) {
        case 1:
            code = str[start_idx];
            break;
        case 2:
            code = (((str[start_idx] & 0x1F) << 6) | (str[start_idx+1] & 0x3F));
            break;
        case 3:
            code = (((str[start_idx] & 0x0F) << 12) | ((str[start_idx+1] & 0x3F) << 6) | 
                    (str[start_idx+2] & 0x3F));
            break;
        case 4:
            code = (((str[start_idx] & 0x07) << 18) | ((str[start_idx+1] & 0x3F) << 12) | 
                    ((str[start_idx+2] & 0x3F) << 6) | (str[start_idx+3] & 0x3F));
            break;
        default:
            code = str[start_idx];
            break;
    }
    
    return code;
}
#define OS_Flash_Font_UTF8_Address 0x25C000


#define LEGAL_UNICODE_RANGE_START 0x4E00
#define LEGAL_UNICODE_RANGE_END   0x9FFF
#define UTF8_FLASH_NUM_SIZE 32      // 每个字模占用32字节(16*16)
/**
 * @brief 将Unicode码位映射到Flash存储地址
 * @param code Unicode码位
 * @return 对应的Flash存储地址
 */
uint32_t code_to_addr(uint32_t code) {
    /* 映射思路：
     * 字库文件的地址范围为OS_Flash_Font_UTF8_Address
     * 每个字占用32字节
     * 码位映射的区域：2010-201F,2030-205F,2100-2FFF
     *                3000-31FF
     *                4E00-9FFF
     * 
     * 地址大小为：OS_Flash_Font_UTF8_Size = 0x100000
     * 
     * Flash地址分区映射：
     * 存储一个字模需要32字节，所以可以存的字符数为:100000/32 = 8000
     * 
     * 将码位二次压缩，将码位范围映射到0-7FFF，按规律映射：
     * 压缩：2000-201F -> 1000-101F (-1000)
     * 压缩：2030-205F -> 1030-105F (-1000)
     * 压缩：2100-2FFF -> 1100-1FFF (-1000)
     *       3000-31FF -> 2300-23FF (-1000)
     *       4E00-9FFF -> 2E00-7FFF (-2000)
     * 
     * 每个字的地址为:
     * 若码位范围为：2000~3200：OS_Flash_Font_UTF8_Address+(Code-0x1000)*32
     * 若码位范围为：4E00~9FFF：OS_Flash_Font_UTF8_Address+(Code-0x2000)*32
     */
    
    // if (code >= 0x2000 && code <= 0x3200) {
    //     return OS_Flash_Font_UTF8_Address + (code - 0x1000) * UTF8_FLASH_NUM_SIZE;
    // } else 
    if (code >= LEGAL_UNICODE_RANGE_START && code <= LEGAL_UNICODE_RANGE_END) {
        return OS_Flash_Font_UTF8_Address + (code - 0x4E00) * UTF8_FLASH_NUM_SIZE;
    } else {
        // 其他无定义码位范围，返回安全码位
        return 0xFFFFFFFFU;
    }
}
#if __USE_FLASH__ == 0
const char* flash_file_path = "E:\\Stm32Project\\CubeProject\\cmaketest2\\KSCdraw\\.data\\flash.bin";
FILE* flash_file = NULL;
#include <stdio.h>
void flash_init(void) {
    flash_file = fopen(flash_file_path, "rb");
    if(flash_file==NULL){
        printf("flash file open failed\n");
    }
}
void flash_readdata(uint32_t addr, uint8_t* data, uint32_t count) {
    fseek(flash_file, addr, SEEK_SET);
    fread(data, 1, count, flash_file);
    printf("flash_readdata addr:0x%08x,count:%d\n",addr,count);
    //fclose(flash_file);
}

#endif
/**
 * @brief 从Flash读取字模数据
 * @param addr 读取的起始地址
 * @param data 存储读取结果的缓冲区(至少32字节)
 */
void flash_read(uint32_t addr, uint8_t* data) {
    uint16_t byte_count = UTF8_FLASH_NUM_SIZE;
    flash_readdata(addr, data, byte_count);
    // kprintf("flash_read addr:0x%08x,count:%d\n",addr,byte_count);
}

uint8_t utf8getulen(const char* str, uint8_t idx) {
    if(str==NULL){return 0;}
    uint16_t idxlen = utf8_prev_n_chars_bytes((const uint8_t *)str, idx);
    return utf8_char_len((const uint8_t *)str, idxlen);
}

uint8_t* utf8getufont(const char* str, uint8_t idx) {
    static uint8_t fontbuf[32]={0};
    if(str==NULL){return 0;}
    uint16_t idxlen = utf8_prev_n_chars_bytes((const uint8_t *)str, idx);
    uint8_t char_len = utf8_char_len((const uint8_t *)str, idxlen);
    //printf("utf8getufont idxlen:%d,char_len:%d\n",idxlen,char_len);
    uint32_t code = utf8_to_code((const uint8_t *)str, idxlen, char_len);
    uint32_t addr = code_to_addr(code);
    flash_read(addr, fontbuf);
    return fontbuf;
}

/**
 * @brief 获取字符串中指定位置的Unicode字符的Flash地址
 * @param str UTF-8编码的字符串指针
 * @param idx 第idx个字符(从0开始)
 * @param mode 获取模式
 * 0:获取字符串总Unicode数量
 * 1:获取前idx个字符的总字符长度
 * 2:获取第idx个字符的宽度
 * 3:获取字符Unicode码位
 * 4:获取字符Flash地址
 * @return 对应的Flash存储地址或Unicode码位
 */
uint32_t utf8get(const char* str, uint8_t idx,uint8_t mode){
    static uint8_t fontbuf[32];
    if(str==NULL){return 0;}
    uint16_t idxlen = utf8_prev_n_chars_bytes((const uint8_t *)str, idx);
    uint8_t char_len = utf8_char_len((const uint8_t *)str, idxlen);
    uint32_t code = utf8_to_code((const uint8_t *)str, idxlen, char_len);
    uint32_t addr = code_to_addr(code);
    flash_read(addr, fontbuf);
    //printf("utf8get idxlen:%d,char_len:%d,code:0x%08x,addr:0x%08x\n",idxlen,char_len,code,addr);
    switch(mode){
        case 0:
            return (uint32_t)utf8_get_unicode_count((const uint8_t *)str);
        case 1:
            return (uint32_t)idxlen;
        case 2:
            return (uint32_t)char_len;
        case 3:
            return code;
        case 4:
            return addr;
        default:
            return 0;
    }
}

// /**
//  * @brief 获取字符串中指定位置的Unicode字符的Flash地址
//  * @param str UTF-8编码的字符串指针
//  * @param idx 要获取的字符索引(从0开始)
//  * @return 对应的Flash存储地址
//  */
// uint32_t utf8_get_char_addr(char* str, uint8_t idx) {
//     if (str == NULL) {
//         return 0x200000;  // 空指针返回安全地址
//     }
    
//     uint8_t current_idx = 0;
//     uint8_t char_count = 0;
    
//     while (str[current_idx] != '\0' && char_count <= idx) {
//         uint8_t len = utf8_len(str, current_idx);
//         if (len == 0) {
//             // 无效编码，跳过
//             current_idx++;
//             continue;
//         }
        
//         if (char_count == idx) {
//             // 找到目标字符
//             uint32_t code = utf8_to_code(str, current_idx, len);
//             return code_to_addr(code);
//         }
        
//         current_idx += len;
//         char_count++;
//     }
    
//     return 0x200000;  // 索引超出范围，返回安全地址
// }

// /**
//  * @brief 将UTF-8字符串转换为Flash地址数组
//  * @param str UTF-8编码的字符串指针
//  * @param addr_array 存储地址的数组指针(至少32个元素)
//  * @return 解析的字符数量，异常返回0
//  */
// uint8_t utf8_str_to_addr_array(char* str, uint32_t* addr_array) {
//     if (str == NULL || addr_array == NULL) {
//         return 0;  // 空指针返回0
//     }
    
//     uint8_t current_idx = 0;
//     uint8_t char_count = 0;
    
//     while (str[current_idx] != '\0' && char_count < 32) {
//         uint8_t len = utf8_len(str, current_idx);
//         if (len == 0) {
//             // 无效编码，跳过
//             current_idx++;
//             continue;
//         }
        
//         uint32_t code = utf8_to_code(str, current_idx, len);
//         addr_array[char_count] = code_to_addr(code);
        
//         current_idx += len;
//         char_count++;
//     }
    
//     return char_count;
// }

// // /**
// //  * @brief 向Flash写入字模数据
// //  * @param addr 写入的起始地址
// //  * @param data 要写入的字模数据
// //  * @param count 字模数量
// //  */
// // void flash_write_big(uint32_t addr, uint8_t* data, uint16_t count) {
// //     uint16_t byte_count = count * UTF8_FLASH_NUM_SIZE;
// //     flash_writedata_big(addr, data, byte_count);
// // }



// /**
//  * @brief 直接获取字符串对应的字模数据
//  * @param str UTF-8编码的字符串指针
//  * @param font_data 存储字模数据的数组指针
//  */
// void utf8_get_font_data(char* str, uint8_t* font_data) {
//     if (str == NULL || font_data == NULL) {
//         return;  // 空指针直接返回
//     }
    
//     //uint32_t addr_array[UTF8_FLASH_MAX_STRING_SIZE + 2];
//     uint32_t addr = utf8_get_char_addr(str, 0);
    
//     if (addr == 0x200000) {
//         return;  // 解析失败直接返回
//     }
//     // 读取字符的字模数据
//         flash_read(addr, font_data);
// }
