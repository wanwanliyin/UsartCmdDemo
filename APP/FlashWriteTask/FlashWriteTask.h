#ifndef FLASHWRITETASK_H
#define FLASHWRITETASK_H


#define FLASH_WRITE_MAX_LEN    64

typedef struct FLASH_DATA_STRUCT
{
    unsigned char mode;    //mode = 1 : 可能含多个连续地址写入  mode = 2： 可能有多个相同地址数据写入
    unsigned int addr;
    unsigned short len;                    
    unsigned char data[FLASH_WRITE_MAX_LEN];
}FLASH_DATA_STRUCT, *PFLASH_DATA_STRUCT;


void FlashWriteTask(void *pvParameters);


#endif





