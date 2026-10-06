/**
*@file
*@authors ГрВПО
*@copyright ООО "ЦПТ Агроцифра"
*@brief Файл реализации объектов BSP
*@details В данном файле содержатся все необходимые includ, и реализация BSP.\n
* Только на этом уровне подключаются и используются библиотеки производителя микроконтроллера.
*/
#include "bsp.h"
#include "main_appl.h"

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"
#include "timers.h"

static int bsp_init(void);

const bsp_t bsp = 
{
	.init 		= bsp_init,
	.can 			= &can,
	.kbd 			= &kbd,
	.leds 		= &leds,
	.drv_2605 = &drv_2605,
};
/*******************************************************************************/
/**
* @brief Реализация метода init() для BSP микроконтроллера AT32F415
* @param[out] ret возвращает код ошибки
*/ 
static int bsp_init(void)
{
    int ret = pdTRUE;
    pwc_pvm_level_select(PWC_PVM_VOLTAGE_2V9);
		if(bsp.leds->init) 
			bsp.leds->init();
		if(bsp.drv_2605->init) 
			bsp.drv_2605->init();
		{
		  crm_clocks_freq_type crm_clocks_freq;
	    crm_clocks_freq_get(&crm_clocks_freq);		/* Read SYS frequency */
	    health.sclk_freq = crm_clocks_freq.sclk_freq;
		}
    return ret;
}          