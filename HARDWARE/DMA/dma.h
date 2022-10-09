#ifndef __DMA_H
#define	__DMA_H	   
#include "sys.h"
					    					    
#define DMA_RCVBUF_SIZE   100
extern u8 g_DmaRcvBuf[DMA_RCVBUF_SIZE];
void DmaConfig(DMA_Channel_TypeDef*DMA_CHx,u32 cpar,u32 cmar);//≈‰÷√DMA1_CHx

void DmaEnable(DMA_Channel_TypeDef*DMA_CHx);// πƒ‹DMA1_CHx
		   
#endif




