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

static void appl_start(void);
static void appl_daemon(void);
static void appl_idle(void);
static void appl_tick(void);

const api_t appl = 
{
	.start 		= appl_start,
	.daemon 	= appl_daemon,
	.idle 		= appl_idle,
	.tick 		= appl_tick,
	.can 			= &appl_can,
	.ai 			= &appl_ai,
	.kbd 			= &appl_kbd,
	.enc			= &appl_enc,
};

static void appl_start(void)
{
	bsp.init();
	crm_clocks_freq_get(&health.crm_clocks_freq);		/* Read SYS frequency */
	
	/* Create and start all applications */
	appl.kbd->start();
	appl.enc->start();
	appl.can->start();
	appl.hv_motor->start();
	appl.ai->start();
}
static void appl_daemon(void)
{
	vTaskDelay(pdMS_TO_TICKS(STATISTIC_PERIOD));
	health.heap = xPortGetFreeHeapSize();				/* Current free heap */
  health.tasks = uxTaskGetNumberOfTasks();		/* Current tasks number */
}
static void appl_idle(void)
{
    static portTickType LastTick = 0U;
    static portTickType counter;                                		//наш трудяга счетчик
    static portTickType max_count ;                             		//максимальное значение счетчика, вычисляется при калибровке и соответствует 100% CPU idle

    ++counter;                                                  		//приращение счетчика

    if((LastTick + 250) < xTaskGetTickCount())                  		//если прошло 250 тиков (250 мсек для моей платфрмы)    
    {                             
        LastTick = xTaskGetTickCount();
        if(counter > max_count) max_count = counter;            		//это калибровка
        health.load = 10000U * (max_count - counter) / max_count;   //вычисляем текущую загрузку
        counter = 0;                                            		//обнуляем счетчик

        /* Reload watchdog counter (Period - 200 ms) */
        wdt_counter_reload();
    }	
}
static void appl_tick(void)
{
	health.uptime += 1;
}
