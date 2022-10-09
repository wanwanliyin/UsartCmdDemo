#ifndef USARTTASK_H
#define USARTTASK_H


void UsartTask(void *pvParameters);
short GetFlashWriteDataQueue(unsigned short          TicksToWait,unsigned char *pucOut, unsigned short *outLen);
short GetMetanalDataQueue(unsigned short         TicksToWait,unsigned char *pucOut, unsigned short *outLen);
void PrintBuf(unsigned char *buf, unsigned short len);
void PrintStrBuf(unsigned char *str, unsigned char *pbuf, unsigned short len);

typedef struct USART_CMD_STRUCT
{
    const char *pCmd;                    
    void (* Exec)(unsigned char *pRecv, unsigned short recvLen);
}USART_CMD_STRUCT, *PUSART_CMD_STRUCT;


#endif

