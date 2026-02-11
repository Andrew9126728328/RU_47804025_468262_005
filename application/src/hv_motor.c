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

/**
* @brief Задача управления тактильно/вибрационным мотором
* @param[in] pvParameters принимает значение времени сканирования ряда в мс 
*/ 
void hv_motor_task_func(void *pvParameters)
{
	int scan_time = (NULL != pvParameters)? (int)pvParameters: 5;
  /* Infinite loop */
	for(;;)
	{
		vTaskDelay(pdMS_TO_TICKS(scan_time));			
	}
}