#include "freertos_app.h"
#include "main_appl.h"
#include "bsp.h"

static void encoder_task_func(void *pvParameters);
static int enc_start(void);
	
const appl_enc_t appl_enc = 
{
	.start = enc_start,
};
static int enc_start(void)
{
	return xTaskCreate(encoder_task_func,  "encoder_task",   configMINIMAL_STACK_SIZE*2,  (void*)5, 		tskIDLE_PRIORITY + 1,		NULL);
}

static void encoder_task_func(void *pvParameters)
{
int scan_time = (NULL != pvParameters)? (int)pvParameters: 50;
  /* Infinite loop */
  while(1)
  {

     vTaskDelay(scan_time);

  }	
}