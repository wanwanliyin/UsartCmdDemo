#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "event_groups.h"
#include "timers.h"
#include "list.h"
#include "UsartTask.h"
#include "MetanalDataProcessTask.h"
#include "stmflash.h"
#include "rtc.h"
#include "led.h"
#include "FlashWriteTask.h"

#define START_TASK_PRIO 1 //�������ȼ�
#define START_STK_SIZE 128 //�����ջ��С
TaskHandle_t StartTask_Handler; //������

#define USART_TASK_PRIO 3 //�������ȼ�
#define USART_STK_SIZE 256 //�����ջ��С
TaskHandle_t UsartTask_Handler; //������

#define MetanalDATAPROCESS_TASK_PRIO 3 //�������ȼ�
#define MetanalDATAPROCESS_STK_SIZE 384 //�����ջ��С
TaskHandle_t MetanalDataProcessTask_Handler; //������

#define FLASHWRITE_TASK_PRIO 2 //�������ȼ�
#define FLASHWRITE_STK_SIZE 640 //�����ջ��С
TaskHandle_t FlashWriteTask_Handler; //������

void start_task(void *pvParameters)
{
     taskENTER_CRITICAL(); 
     xTaskCreate(UsartTask, "UsartTask", USART_STK_SIZE, NULL, USART_TASK_PRIO, &UsartTask_Handler); 
 //    xTaskCreate(MetanalDataProcessTask, "MetanalDataProcessTask", MetanalDATAPROCESS_STK_SIZE, NULL, MetanalDATAPROCESS_TASK_PRIO, &MetanalDataProcessTask_Handler);   
     xTaskCreate(FlashWriteTask, "FlashWriteTask", FLASHWRITE_STK_SIZE, NULL, FLASHWRITE_TASK_PRIO, &FlashWriteTask_Handler);
     vTaskDelete(StartTask_Handler); //ɾ����ʼ����
     taskEXIT_CRITICAL(); 
}

void HandwareInit(void)
{
     NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
     DelayInit();	    	 
     Uart1Init(115200);	 
     LedInit();
     while(RTC_Init())	
     {
        printf("wait Rtc\r\n");
     }
}

int main(void)
{	
    HandwareInit();
    printf("start\n");
    xTaskCreate(start_task, "start_task", START_STK_SIZE, NULL, START_TASK_PRIO, &StartTask_Handler); 
    vTaskStartScheduler();
    
    while(1);
}








