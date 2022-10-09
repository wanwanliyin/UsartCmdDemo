#ifndef FLASHWRITETASK_H
#define FLASHWRITETASK_H


#define FLASH_WRITE_MAX_LEN    64

typedef struct FLASH_DATA_STRUCT
{
    unsigned char mode;    //mode = 1 : ���ܺ����������ַд��  mode = 2�� �����ж����ͬ��ַ����д��
    unsigned int addr;
    unsigned short len;                    
    unsigned char data[FLASH_WRITE_MAX_LEN];
}FLASH_DATA_STRUCT, *PFLASH_DATA_STRUCT;


void FlashWriteTask(void *pvParameters);


#endif





