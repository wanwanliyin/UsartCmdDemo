#include <stdio.h>	
#include <string.h>
#include <stdlib.h>
#include "UsartTask.h"
#include "dma.h"
#include "usart.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "MetanalDataProcessTask.h"
#include "rtc.h"
#include "stmflash.h"
#include "FlashWriteTask.h"

#define RTC_SET     0xCC
#define PROCPTR(Address)            (Address)
#define USART_MINDATA_LEN           9 
#define USE_FLASH_TASK              1

QueueHandle_t MetanalDataQueue = NULL;
#if USE_FLASH_TASK
QueueHandle_t FlashWriteDataQueue = NULL;
#endif

void PrintBuf(unsigned char *buf, unsigned short len)
{
    u16 i = 0;

    for (i = 0; i < len; i++)
    {
        printf("%02x ", buf[i]);
    }
    
    printf("\r\n");
}

void PrintStrBuf(unsigned char *str, unsigned char *pbuf, unsigned short len)
{
    int i;
    u16 temp = 0;

    if (str)
    {
        printf("%s", str);
    }

    for (i = 0; i < len; i++)
    {    
        temp = (u16)(pbuf[i]);
        printf("%02x ", temp);
    }

    printf("\r\n");
}

short Bcd2Asc(unsigned char *pucBcd, char *pcAsc, unsigned short usLen)
{
    unsigned short i;
    unsigned char ucTmp;

    for(i=0;i<usLen;i++)
    {
        ucTmp = pucBcd[i]>>4;
        pcAsc[i*2] = (ucTmp>9)? (ucTmp-10+'A'):(ucTmp+'0');
        ucTmp = pucBcd[i]&0xF;
        pcAsc[i*2+1] = (ucTmp>9)? (ucTmp-10+'A'):(ucTmp+'0');
    }

    pcAsc[usLen*2] = 0;

    return 0;
}

short Bcd2Hex(unsigned char *pucBcd, unsigned short usLen, unsigned char *pucHex)
{
    char acTemp[3];
    while(usLen--)
    {
        Bcd2Asc(pucBcd, (char *)acTemp, 1);
        acTemp[2] = 0;
        *pucHex = (atoi(acTemp));
        pucBcd++;
        pucHex++;
    }
    return 0;
}

short GetMetanalDataQueue(u16         TicksToWait,u8 *pucOut, u16 *outLen)
{
   BaseType_t xStatus; 
   const TickType_t xTicksToWait = pdMS_TO_TICKS( TicksToWait );
   if(pucOut == NULL)
   {
       return -1;
   }
   if(MetanalDataQueue == NULL)
   {
       return -1;
   }
   
   xStatus = xQueueReceive( MetanalDataQueue, pucOut, xTicksToWait );
   if(xStatus == pdPASS)
   {
       if(outLen != NULL)
       {
          *outLen = METANAL_DATA_LEN;
       }
       return 0;
   }
   else
   {
       return -1;
   }
}

short GetFlashWriteDataQueue(u16           TicksToWait,u8 *pucOut, u16 *outLen)
{
#if USE_FLASH_TASK
    BaseType_t xStatus; 
    const TickType_t xTicksToWait = pdMS_TO_TICKS( TicksToWait );
    if(pucOut == NULL)
    {
        return -1;
    }
    if(FlashWriteDataQueue == NULL)
    {
        return -1;
    }
    
    xStatus = xQueueReceive( FlashWriteDataQueue, pucOut, xTicksToWait );
    if(xStatus == pdPASS)
    {
        if(outLen != NULL)
        {
           *outLen = METANAL_DATA_LEN;
        }
        return 0;
    }
#endif
    return -1;
    

}

void MetanalWriteDataProcess(u8 *pRecv, u16 recvLen)
{
    u8 auctmp[METANAL_SAVE_LEN + 2] = {0};
    static u32 addr = METANAL_DATA_BASE;
    static int metanalNum = 0;
#if USE_FLASH_TASK
    FLASH_DATA_STRUCT stFlashData;
#endif
    if(recvLen != METANAL_DATA_LEN)
    {
        return;
    }
    
    memcpy(auctmp + 3, pRecv, recvLen);

    PrintStrBuf("Rcv: ", auctmp + 3, METANAL_SAVE_LEN - 3);
#if 1
//    if(calendar.sec == 0)
    {
       auctmp[0] = calendar.hour;
       auctmp[1] = calendar.min;
       auctmp[2] = calendar.sec;
       ++metanalNum;
#if USE_FLASH_TASK
       if(metanalNum <= METANAL_DATA_MAXNUM)
       {
          stFlashData.addr = addr;
          stFlashData.len = METANAL_SAVE_LEN;
          stFlashData.mode = 1;
          memcpy(stFlashData.data, auctmp, METANAL_SAVE_LEN);
          
          if(pdPASS == xQueueSendToBack(FlashWriteDataQueue, (u8 *)&stFlashData, 10))
          {
             stFlashData.addr = METANAL_NUM_BASE;
             stFlashData.len = 4;
             stFlashData.mode = 2;
             memcpy(stFlashData.data, (u8 *)&metanalNum, 4);
             if(pdFAIL == xQueueSendToBack(FlashWriteDataQueue, (u8 *)&stFlashData, portMAX_DELAY))
             {
                  printf( "Could not send to the queue.\r\n" );
             }  
             addr += METANAL_SAVE_LEN;
          }
          else
          {
             printf( "Could not send to the queue.\r\n" );
          
          }
       }
#else
       taskENTER_CRITICAL(); 
       Eflash_Write(addr, auctmp, METANAL_SAVE_LEN, 1); 
       Eflash_Write(METANAL_NUM_BASE, (u8 *)&metanalNum, 4, 1);
       taskEXIT_CRITICAL();
       addr += METANAL_SAVE_LEN;
#endif

    }
#endif

}

void MetanalReadDataProcess(u8 *pRecv, u16 recvLen)
{
    int readNum = 0;
    u8 auctmp[METANAL_SAVE_LEN + 2];
    u32 addr = METANAL_DATA_BASE;
    int i,j;
    
    Eflash_Read(METANAL_NUM_BASE, (u8 *)&readNum, 4);
    printf("readNum = %d\r\n", readNum);
    for(j=0; j<readNum; j++)
    {
        Eflash_Read(addr, auctmp, METANAL_SAVE_LEN);
        printf("Read[%d]:", j);
        for(i=0; i<METANAL_SAVE_LEN; i++)
        {
           if(i<=2)
           {
               printf( "%02d ", auctmp[i] ); 
           }
           else
           {
              printf( "0x%02x ", auctmp[i] ); 
           }
        }
        printf("\r\n");
        addr += METANAL_SAVE_LEN;
    }


}

void RtcSetProcess(u8 *pRecv, u16 recvLen)
{
    int i;
    u16 year;
    
    Bcd2Hex(pRecv + 2, 7, pRecv + 2);
    for(i =0; i<METANAL_DATA_LEN; i++)
    {
       printf("%d ", pRecv[i]);
    }
    year = 2000 + pRecv[3];
    RTC_Set(year, pRecv[4], pRecv[5], pRecv[6], pRecv[7], pRecv[8]);

}

static USART_CMD_STRUCT g_UsartCmdList[] =
{
   {"\xFF\x17",PROCPTR(MetanalWriteDataProcess)},
   {"\xFF\xF7",PROCPTR(MetanalReadDataProcess)},
   {"\xFF\xCC",PROCPTR(RtcSetProcess)},

};

static void ExecUsartCmd(unsigned char i,u8 *pRecv, u16 recvLen)
{
    g_UsartCmdList[i].Exec(pRecv, recvLen);
}



void UsartTask(void *pvParameters)
{
   u8 recvBuf[DMA_RCVBUF_SIZE];
   u16 len = 0;
   int i;
   DmaConfig(DMA1_Channel5,(u32)&USART1->DR,(u32)g_DmaRcvBuf);
   USART_DMACmd(USART1,USART_DMAReq_Rx,ENABLE);  
   DmaEnable(DMA1_Channel5);
#if USE_FLASH_TASK   
   FlashWriteDataQueue =  xQueueCreate(5, FLASH_WRITE_MAX_LEN);
#endif
   while(1)
   {
      ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
      len = UartCheckData();
      if(len)
      {
         UartRxData(recvBuf, len);
         for (i = 0; i < sizeof(g_UsartCmdList)/sizeof(g_UsartCmdList[0]); i++)
    	 {
    		if (0 == memcmp(recvBuf, g_UsartCmdList[i].pCmd, strlen(g_UsartCmdList[i].pCmd)))
    		{
    			ExecUsartCmd(i,recvBuf,len);
                break;
    		}
    	 } 
      }
      vTaskDelay(10);	
   }
   
}
