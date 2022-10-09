#include <stdio.h>	
#include <string.h>
#include "FlashWriteTask.h"
#include "sys.h"
#include "UsartTask.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "stmflash.h"


void FlashWriteTask(void *pvParameters)
{
    short sRc; 
    FLASH_DATA_STRUCT stFlashData;
    u32 writeAddr = 0;
    u8 writeTmp[1152];
    u16 writeLen = 0;
    u32 writeAddr2 = 0;
    u8 writeTmp2[64];
    u16 writeLen2 = 0;
    TickType_t xTicks = xTaskGetTickCount();
    while(1)
    {
        if((xTaskGetTickCount() - xTicks) > 1000)
        {
            if(writeLen)
            {
                taskENTER_CRITICAL(); 
                Eflash_Write(writeAddr, writeTmp, writeLen, 1);
                taskEXIT_CRITICAL();
                writeLen = 0;
                writeAddr = 0;
            }
            if(writeLen2)
            {
                taskENTER_CRITICAL(); 
                Eflash_Write(writeAddr2, writeTmp2, writeLen2, 1);
                taskEXIT_CRITICAL();
                writeLen2 = 0;
                writeAddr2 = 0;
            }
        }
        
        sRc = GetFlashWriteDataQueue(100, (u8 *)&stFlashData, NULL);
        if( sRc == 0 ) 
        { 
            xTicks = xTaskGetTickCount();
    //        printf("WriteAddr = %p\r\n", stFlashData.addr);
   //         PrintStrBuf("WriteData: ", stFlashData.data, stFlashData.len);
   //         printf("WriteRealAddr = %p\r\n", writeAddr);
   //         printf("writeLen = %d\r\n", writeLen);
   
            if(stFlashData.mode == 1)        //可能有连续地址写入数据的整合到1024后写入
            {
                if(!writeAddr)
                {
                   writeAddr = stFlashData.addr;
                }
            
                if((writeAddr + writeLen) == stFlashData.addr)   
                {
                    if((writeLen + stFlashData.len) <= sizeof(writeTmp))
                    {
                       memcpy(writeTmp + writeLen, stFlashData.data, stFlashData.len);
                    }
                    else
                    {
                       FUN_IN;
                       printf("OVERERR\r\n");
                    }
                    
                    writeLen += stFlashData.len;
                    
                    if((writeAddr + writeLen) >= (STM32_FLASH_DATABASE + STM32_FLASH_DATASIZE))
                    {
                        writeLen = writeLen - (STM32_FLASH_DATABASE + STM32_FLASH_DATASIZE - writeAddr - writeLen);
                    }
                    else
                    {
                        if(writeLen >= 1024)
                        {
                            taskENTER_CRITICAL(); 
                            Eflash_Write(writeAddr, writeTmp, 1024, 1);
                            taskEXIT_CRITICAL();
                            writeLen = writeLen - 1024;
                            writeAddr += 1024;
                            memmove(writeTmp, writeTmp + 1024 , writeLen); 
                        }
                    }
                }
                else
                {
                    Eflash_Write(stFlashData.addr, stFlashData.data, stFlashData.len, 1);
                }
            }
            else if(stFlashData.mode == 2)    //可能多次重复地址写入的
            {
                if(!writeAddr2)
                {
                   writeAddr2 = stFlashData.addr;
                }
                
                if(writeAddr2 == stFlashData.addr)      
                {
                    if(stFlashData.len <= sizeof(writeTmp2))
                    {
                       if(stFlashData.len >writeLen2)
                       {
                         writeLen2 = stFlashData.len;
                       }
                       memcpy(writeTmp2, stFlashData.data, writeLen2);
                    }
                    else
                    {
                       Eflash_Write(stFlashData.addr, stFlashData.data, stFlashData.len, 1);
                       writeLen2 = 0;
                       writeAddr2 = 0;
                    }
                }

            }
            else
            {
                Eflash_Write(stFlashData.addr, stFlashData.data, stFlashData.len, 1);
            }
        }
        vTaskDelay(20);	
    }
}

