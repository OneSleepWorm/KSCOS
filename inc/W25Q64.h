#ifndef __W25Q64_H
#define __W25Q64_H

#include <stdint.h>
#include "KSCconfig.h"

#ifdef __cplusplus
extern "C" {
#endif

#if __USE_STM32__
void W25Q64_Init(void);
void W25Q64_DeInit(void);
void W25Q64_ReadID(uint8_t *MID, uint16_t *DID);
void W25Q64_PageProgram(uint32_t Address, uint8_t *DataArray, uint16_t Count);
void W25Q64_PageProgram_NeedNum(uint32_t Address, uint8_t *DataArray, uint16_t Count, uint16_t Start_Count);
void W25Q64_SectorErase(uint32_t Address);
void W25Q64_ReadData(uint32_t Address, uint8_t *DataArray, uint32_t Count);


#endif
#if __USE_PC__
void W25Q64_Init(void);
void W25Q64_DeInit(void);
void W25Q64_PageProgram(uint32_t Address, uint8_t *DataArray, uint16_t Count);
void W25Q64_SectorErase(uint32_t Address);
void W25Q64_ReadData(uint32_t Address, uint8_t *DataArray, uint32_t Count);

#endif

#define flash_init  W25Q64_Init
#define flash_deinit  W25Q64_DeInit
#define flash_write  W25Q64_PageProgram
#define flash_read  W25Q64_ReadData
#define flash_clear  W25Q64_SectorErase

#ifdef __cplusplus
}
#endif

#endif


