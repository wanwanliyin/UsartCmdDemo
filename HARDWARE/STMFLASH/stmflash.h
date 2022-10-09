#ifndef __STMFLASH_H__
#define __STMFLASH_H__
#include "sys.h"  
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK miniSTM32开发板
//STM32 FLASH 驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/13
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////
//用户根据自己的需要设置
#define STM32_FLASH_SIZE 64 	 		//所选STM32的FLASH容量大小(单位为K)
#define STM32_FLASH_WREN 0              //使能FLASH写入(0，不是能;1，使能)
//////////////////////////////////////////////////////////////////////////////////////////////////////

//FLASH起始地址
#define STM32_FLASH_BASE 0x08000000 	//STM32 FLASH的起始地址
 
#define STM32_FLASH_DATABASE    (STM32_FLASH_BASE + 40*1024)
#define STM32_FLASH_DATASIZE    (24*1024)
#define EFLASH_PAGE_SIZE_BYTE    1024
#define PAGE_MASK               (EFLASH_PAGE_SIZE_BYTE - 1)
#define FUN_IN                   printf("--->%s:%d:%s", __FILE__, __LINE__, __FUNCTION__)

#define METANAL_DATA_BASE        STM32_FLASH_DATABASE
#define METANAL_DATA_SIZE        (20*1024)
#define METANAL_NUM_BASE         (STM32_FLASH_DATABASE + METANAL_DATA_SIZE) 
#define METANAL_NUM_SIZE          4


void Eflash_Write(u32 WriteAddr,u8 *pBuffer,u16 len, u8 eraseFlag);
void Eflash_Read(u32 ReadAddr,u8 *pBuffer,u16 NumToRead);
void Eflash_Page_Erase(u32 addr);

//测试写入
void Test_Write(u32 WriteAddr,u16 WriteData);								   
#endif

















