/**
*@file
*@authors ГрВПО
*@copyright ООО "ЦПТ Агроцифра"
*@brief Файл реализации задач приложеня тактильного/вибрационного мотора
*@details В данном файле содержатся все необходимые includ, и реализация задач и функций, обеспечивающих работу тактильного/вибрационного мотора.
*/
#include <string.h>
#include "freertos_app.h"
#include "main_appl.h"
#include "bsp.h"

static void hv_motor_task_func(void *pvParameters);
static int hv_motor_start(void);
	
const appl_hv_motor_t appl_hv_motor = 
{
	.start = hv_motor_start,
};
static int hv_motor_start(void)
{
	return xTaskCreate(hv_motor_task_func, "hv_motor_task",  configMINIMAL_STACK_SIZE*4,  (void*)10, 	tskIDLE_PRIORITY + 1,		NULL);
}

/**
* @brief Задача управления тактильно/вибрационным мотором
* @param[in] pvParameters принимает значение времени сканирования ряда в мс 
*/ 
static void hv_motor_task_func(void *pvParameters)
{
	int scan_time = (NULL != pvParameters)? (int)pvParameters: 5;
  /* Infinite loop */
	for(;;)
	{
		vTaskDelay(pdMS_TO_TICKS(scan_time));			
	}
}