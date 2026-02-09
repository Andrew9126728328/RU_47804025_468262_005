#include "freertos_app.h"
#include "bsp.h"

TaskHandle_t encoder_task_handle = NULL;

void encoder_task_func(void *pvParameters)
{
int scan_time = (NULL != pvParameters)? (int)pvParameters: 50;
  /* Infinite loop */
  while(1)
  {

     vTaskDelay(scan_time);

  }	
}