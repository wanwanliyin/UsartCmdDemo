#include <stdio.h>	
#include <string.h>
#include "sys.h"
#include "MetanalDataProcessTask.h"
#include "UsartTask.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "stmflash.h"
#include "rtc.h"


void ReadMetanalData(void)
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

void MetanalDataProcessTask(void *pvParameters)
{
    short sRc; 
    u8 auctmp[METANAL_SAVE_LEN + 2];
    u8 aucReadTmp[METANAL_SAVE_LEN + 2];
    int i;
    u32 addr = METANAL_DATA_BASE;
    int metanalNum = 0;
    
    while(1)
    {
        sRc = GetMetanalDataQueue(100, auctmp + 3, NULL);
        if( sRc == 0 ) 
        { 
            printf("Rcv:");
            for(i=3; i<METANAL_SAVE_LEN; i++)
            {
               printf( "0x%x ", auctmp[i] ); 
            }
            printf("\r\n");

            if(auctmp[4] == METANAL_TYPE)
            {
            //    if(calendar.sec == 0)
                {
                    if(addr < (METANAL_DATA_BASE + METANAL_DATA_SIZE))
                    {
                       auctmp[0] = calendar.hour;
                       auctmp[1] = calendar.min;
                       auctmp[2] = calendar.sec;
                       taskENTER_CRITICAL(); 
                       Eflash_Write(addr, auctmp, METANAL_SAVE_LEN, 1);

                       Eflash_Read(addr, aucReadTmp, METANAL_SAVE_LEN);
                       if(0 != memcmp(auctmp, aucReadTmp, METANAL_SAVE_LEN))
                       {
                           printf("write err\r\n");
                        //   while(1);
                       }
                       
                       metanalNum++;
                       Eflash_Write(METANAL_NUM_BASE, (u8 *)&metanalNum, 4, 1);
                       taskEXIT_CRITICAL();
                       addr += METANAL_SAVE_LEN;
                    }
                }
            }
            else if(auctmp[4] == METANAL_READ)
            {
                ReadMetanalData();
            }

        }
        vTaskDelay(20);	
    }
}

