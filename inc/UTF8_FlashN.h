#ifndef __UTF8_FLASHN_H__
#define __UTF8_FLASHN_H__
#include "KSCconfig.h"
#include <stdint.h>
#if __USE_FLASH__ ==1
#include "W25Q64.h"
#endif
#ifndef __USE_FLASH__

#endif
//分配各个功能块的内存大小
// #define OS_Flash_SYSTEM_Size       0x000000
// #define OS_Flash_Font_GB_Size      0x100000
// #define OS_Flash_Font_UTF8_Size    0x100000
// #define OS_Flash_User_Size         0x300000
// //分配各个功能块的首地址
// #define OS_Flash_SYSTEM_Address    0x000000
// #define OS_Flash_Font_GB_Address   0x100000
// #define OS_Flash_Font_UTF8_Address 0x200000
// #define OS_Flash_User_Address      0x500000
//最大写入数据大小
#define Max_Write_Count 256

/**
 * @file UTF8_FlashN.h
 * @brief UTF-8字符串解析与Flash字库操作库
 * @version 1.0
 * @date 2026-02-21
 * 
 * 本库提供UTF-8字符串解析、Unicode码位转换、Flash字库地址映射
 * 及字模读写等功能，适用于嵌入式系统中的文本显示应用。
 */
#if __USE_FLASH__ ==0
void flash_init(void);
#endif


uint8_t utf8getulen(const char* str, uint8_t idx);

uint8_t* utf8getufont(const char* str, uint8_t idx);

uint32_t utf8get(const char* str, uint8_t idx,uint8_t mode);

#endif // __UTF8_FLASHN_H__
