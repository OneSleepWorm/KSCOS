#ifndef __UTF8_FLASHN_H__
#define __UTF8_FLASHN_H__
#include "KSCconfig.h"
#include <stdint.h>
#if __USE_FLASH__ ==1
#include "W25Q64.h"
#endif

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
