#ifndef __STMFLASH_H__
#define __STMFLASH_H__
#include "sys.h"  
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK miniSTM32������
//STM32 FLASH ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/13
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////
//�û������Լ�����Ҫ����
#define STM32_FLASH_SIZE 64 	 		//��ѡSTM32��FLASH������С(��λΪK)
#define STM32_FLASH_WREN 0              //ʹ��FLASHд��(0��������;1��ʹ��)
//////////////////////////////////////////////////////////////////////////////////////////////////////

//FLASH��ʼ��ַ
#define STM32_FLASH_BASE 0x08000000 	//STM32 FLASH����ʼ��ַ
 
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

//����д��
void Test_Write(u32 WriteAddr,u16 WriteData);								   
#endif

















