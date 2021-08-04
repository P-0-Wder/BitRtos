#ifndef __PERIPH_FLASH_H
#define __PERIPH_FLASH_H
#include "stm32f4xx.h"

#define SIZE   4
//STM32 FLASH ��ʼ��ַ
#define STM32_FLASH_BASE 0x08000000  
//STM32 FLASH ������ַ
#define ADDR_FLASH_SECTOR_0     ((u32)0x08000000) //����0, 16 Kbytes  
#define ADDR_FLASH_SECTOR_1     ((u32)0x08004000) //����1, 16 Kbytes  
#define ADDR_FLASH_SECTOR_2     ((u32)0x08008000) //����2, 16 Kbytes  
#define ADDR_FLASH_SECTOR_3     ((u32)0x0800C000) //����3, 16 Kbytes  
#define ADDR_FLASH_SECTOR_4     ((u32)0x08010000) //����4, 64 Kbytes  
#define ADDR_FLASH_SECTOR_5     ((u32)0x08020000) //����5, 128 Kbytes  
#define ADDR_FLASH_SECTOR_6     ((u32)0x08040000) //����6, 128 Kbytes  
#define ADDR_FLASH_SECTOR_7     ((u32)0x08060000) //����7, 128 Kbytes  

#define FLASH_OPERATE_FINISH			1
#define FLASH_OPERATE_UNFINISH		0

#define BIT_8 										VoltageRange_1
#define BIT_16 										VoltageRange_2
#define BIT_32										VoltageRange_3

void FLASH_ReadBuff(u32 ReadAddr,u32 *pBuffer,u32 NumToRead);
void FLASH_Write(u32 WriteAddr,u32 *pBuffer,u32 NumToWrite);
u32 Flash_ReadFromADDR(u32 faddr);
FLASH_Status FLASH_ERASE_PAGE(u32 ERA_ADDR,uint32_t data_size);
#endif



