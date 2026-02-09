/**
*@file
*@authors ГрВПО
*@copyright ООО "ЦПТ Агроцифра"
*@brief Основной файл реализации задач приложеня 
*@details В данном файле содержатся все необходимые includ, и реализация функций, обеспечивающих запуск всех задач.
*/
#include <string.h>

#include "freertos_app.h"
#include "main_appl.h"
#include "bsp.h"

health_t health;			

/* All tasks of application */
extern TaskHandle_t keyboard_task_handle;
extern TaskHandle_t encoder_task_handle;
extern TaskHandle_t can_rx_task_handle;
extern TaskHandle_t can_tx_task_handle;
extern TaskHandle_t hv_motor_task_handle;

extern void keyboard_task_func(void *pvParameters);
extern void encoder_task_func(void *pvParameters);
extern void can_rx_task_func(void *pvParameters);
extern void can_tx_task_func(void *pvParameters);
extern void hv_motor_task_func(void *pvParameters);

void appl_start(void)
{
	bsp.init();
	memset(&health,0,sizeof(health));			/* Clear health */
	vTaskDelay(300);											/* Wait for calculate zero load */
	crm_clocks_freq_get(&health.crm_clocks_freq);		/* Read SYS frequency */
	
	/* Create and start all tasks */
	xTaskCreate(keyboard_task_func, "keyboard_task",  128*4,  (void*)5, 	1,  &keyboard_task_handle);
  xTaskCreate(encoder_task_func,  "encoder_task",   128*2,  (void*)5, 	1,  &encoder_task_handle);
  xTaskCreate(can_rx_task_func,   "can_rx_task",    128*8,  (void*)500, 2,  &can_rx_task_handle);
  xTaskCreate(can_tx_task_func,   "can_tx_task",    128*8,  (void*)1, 	3,  &can_tx_task_handle);
  xTaskCreate(hv_motor_task_func, "hv_motor_task",  128*4,  (void*)10, 	1,  &hv_motor_task_handle);
}
void appl_daemon(void)
{
	vTaskDelay(pdMS_TO_TICKS(STATISTIC_PERIOD));
	health.heap = xPortGetFreeHeapSize();				/* Current free heap */
  health.tasks = uxTaskGetNumberOfTasks();		/* Current tasks number */
}
