#ifndef MATANALDATAPROCESSTASK_H
#define MATANALDATAPROCESSTASK_H

#define METANAL_DATA_LEN    9
#define METANAL_HEAD_LEN    2
#define METANAL_BEGAIN      0xFF
#define METANAL_TYPE        0x17
#define METANAL_READ        0xFE

#define METANAL_SAVE_LEN    (METANAL_DATA_LEN + 3)

#define METANAL_DATA_MAXNUM  2048       //规划的存储空间÷METANAL_SAVE_LEN


void MetanalDataProcessTask(void *pvParameters);

#endif





