#include "periph_flash.h"
#include "stm32f4xx_flash.h"

u32 Flash_ReadFromADDR(u32 faddr)
{
  return *(vu32 *)faddr;
}

void FLASH_ReadBuff(u32 ReadAddr, u32 *pBuffer, u32 NumToRead)
{
  u32 i;
  for (i = 0; i < NumToRead; i++)
  {
    pBuffer[i] = Flash_ReadFromADDR(ReadAddr);
    ReadAddr += SIZE;
  }
}

u8 FLASH_GetFlashSector(u32 addr)
{
  if (addr < ADDR_FLASH_SECTOR_1)
    return FLASH_Sector_0;
  else if (addr < ADDR_FLASH_SECTOR_2)
    return FLASH_Sector_1;
  else if (addr < ADDR_FLASH_SECTOR_3)
    return FLASH_Sector_2;
  else if (addr < ADDR_FLASH_SECTOR_4)
    return FLASH_Sector_3;
  else if (addr < ADDR_FLASH_SECTOR_5)
    return FLASH_Sector_4;
  else if (addr < ADDR_FLASH_SECTOR_6)
    return FLASH_Sector_5;
  else if (addr < ADDR_FLASH_SECTOR_7)
    return FLASH_Sector_6;
  return FLASH_Sector_7;
}

void FLASH_Write(u32 WriteAddr, u32 *pBuffer, u32 NumToWrite)
{
  FLASH_Status status = FLASH_COMPLETE;
  u32 addrx = 0;
  u32 endaddr = 0;

  if (WriteAddr < STM32_FLASH_BASE || WriteAddr % SIZE)
    return;

  FLASH_Unlock();
  FLASH_DataCacheCmd(DISABLE);

  addrx = WriteAddr;
  endaddr = WriteAddr + NumToWrite * SIZE;

  if (addrx < 0X1FFF0000)
  {
    while (addrx < endaddr)
    {
      if (Flash_ReadFromADDR(addrx) != 0XFFFFFFFF)
      {
        status = FLASH_EraseSector(FLASH_GetFlashSector(addrx), VoltageRange_3);

        if (status != FLASH_COMPLETE)
          break;
      }
      else
        addrx += SIZE;
    }
  }
  if (status == FLASH_COMPLETE)
  {
    while (WriteAddr < endaddr)
    {
      if (FLASH_ProgramWord(WriteAddr, *pBuffer) != FLASH_COMPLETE)
      {
        break;
      }

      WriteAddr += SIZE;
      pBuffer++;
    }
  }

  FLASH_DataCacheCmd(ENABLE);
  FLASH_Lock();
}

FLASH_Status FLASH_ERASE_PAGE(u32 ERA_ADDR, uint32_t data_size)
{
  FLASH_Status status;
  u32 addrx = 0;
  u32 endaddr = 0;

  FLASH_Unlock();
  FLASH_DataCacheCmd(DISABLE);

  addrx = ERA_ADDR;
  endaddr = ERA_ADDR + data_size * SIZE;

  if (addrx < 0X1FFF0000)
  {
    while (addrx < endaddr)
    {
      if (Flash_ReadFromADDR(addrx) != 0XFFFFFFFF)
      {
        status = FLASH_EraseSector(FLASH_GetFlashSector(addrx), VoltageRange_3);

        if (status != FLASH_COMPLETE)
          break;
      }
      else
        addrx += SIZE;
    }
  }

  FLASH_Lock();

  return status;
}
