#include <string.h>
#include "stmflash.h"
#include "delay.h"
#include "usart.h"
 
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
 
//��ȡָ����ַ�İ���(16λ����)
//faddr:����ַ(�˵�ַ����Ϊ2�ı���!!)
//����ֵ:��Ӧ����.

void STMFLASH_Write_NoCheck(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite)   
{ 			 		 
	u16 i;
	for(i=0;i<NumToWrite;i++)
	{
		FLASH_ProgramHalfWord(WriteAddr,pBuffer[i]);
	    WriteAddr+=2;//��ַ����2.
	}  
} 

#define SAREG32(addr) (*(volatile unsigned int *)(addr))

void Eflash_Read(u32 ReadAddr,u8 *pBuffer,u16 NumToRead)
{
   	u16 i;
    if(ReadAddr + NumToRead > STM32_FLASH_DATABASE + STM32_FLASH_DATASIZE)
    {
       FUN_IN;
       return;
    }
    if(ReadAddr < STM32_FLASH_DATABASE)
    {
       FUN_IN;
       return;
    } 
	for(i=0;i<NumToRead;i++)
	{
		pBuffer[i]=SAREG32(ReadAddr++);
	}
}


void Eflash_Write(u32 WriteAddr,u8 *pBuffer,u16 len, u8 eraseFlag)
{
    u16 offset;
	u16 pageleft;
	u32 dest = WriteAddr;
	u8* src  = (u8 *)pBuffer;
	u16  eflash_buffer[EFLASH_PAGE_SIZE_BYTE/2];
    if(WriteAddr + len > STM32_FLASH_DATABASE + STM32_FLASH_DATASIZE)
    {
       FUN_IN;
       return;
    }
    if(WriteAddr < STM32_FLASH_DATABASE)
    {
       FUN_IN;
       return;
    } 

	offset  = ((u32)dest & PAGE_MASK);
	pageleft  = EFLASH_PAGE_SIZE_BYTE - offset;

    FLASH_Unlock();	
	/*~{Hg9{R3C;SP6TFk#,TrOH6T5ZR;R32YWw~}*/
	if (offset != 0)
	{
		dest -= offset;/*dest~{V8UkLxW*5=51G0R3M7~}*/
		memcpy((u8*)eflash_buffer, (u8*)dest, EFLASH_PAGE_SIZE_BYTE);  
		if (len > pageleft)
		{
			memcpy((u8*)eflash_buffer + offset, src, pageleft);
			len  -= pageleft;
			src  += pageleft;
		}
		else
		{
			memcpy((u8*)eflash_buffer + offset, src, len);
			len=0;
		}
        if(eraseFlag)
        {
		       FLASH_ErasePage(dest);
        }
        STMFLASH_Write_NoCheck(dest, eflash_buffer,EFLASH_PAGE_SIZE_BYTE/2);
		dest += EFLASH_PAGE_SIZE_BYTE;
		//offset = 0;
	}
	while (len > 0)
	{
		/*~{Hg9{4fTZVP<dR3~}*/
		if (len >= EFLASH_PAGE_SIZE_BYTE)
		{
			memcpy((u8*)eflash_buffer, src, EFLASH_PAGE_SIZE_BYTE);
            if(eraseFlag)
            {
              FLASH_ErasePage(dest);
            }
            STMFLASH_Write_NoCheck(dest, eflash_buffer,EFLASH_PAGE_SIZE_BYTE/2);

			dest += EFLASH_PAGE_SIZE_BYTE;
			src  += EFLASH_PAGE_SIZE_BYTE;
			len  -= EFLASH_PAGE_SIZE_BYTE;
		}
		else
		{   /*~{WvWn:sR32YWw~}*/
			memcpy((u8*)eflash_buffer, (u8*)dest, EFLASH_PAGE_SIZE_BYTE);
			memcpy((u8*)eflash_buffer, src, len);
            if(eraseFlag)
            {
                FLASH_ErasePage(dest);
             
            }
            STMFLASH_Write_NoCheck(dest, eflash_buffer,EFLASH_PAGE_SIZE_BYTE/2);
			len = 0;
		}
	}
    FLASH_Lock();
	return;
}



void Eflash_Page_Erase(u32 addr)
{
    FLASH_Unlock();
    FLASH_ErasePage(addr);
    FLASH_Lock();
}











