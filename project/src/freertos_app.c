/* add user code begin Header */
/**
  ******************************************************************************
  * File Name          : freertos_app.c
  * Description        : Code for freertos applications
  */
/* add user code end Header */

/* Includes ------------------------------------------------------------------*/
#include "freertos_app.h"

/* private includes ----------------------------------------------------------*/
/* add user code begin private includes */
#include "main_appl.h"
/* add user code end private includes */

/* private typedef -----------------------------------------------------------*/
/* add user code begin private typedef */

/* add user code end private typedef */

/* private define ------------------------------------------------------------*/
/* add user code begin private define */

/* add user code end private define */

/* private macro -------------------------------------------------------------*/
/* add user code begin private macro */

/* add user code end private macro */

/* private variables ---------------------------------------------------------*/
/* add user code begin private variables */

/* add user code end private variables */

/* private function prototypes --------------------------------------------*/
/* add user code begin function prototypes */

/* add user code end function prototypes */

/* private user code ---------------------------------------------------------*/
/* add user code begin 0 */

/* add user code end 0 */

/* task handler */
TaskHandle_t daemon_task_handle;

void vApplicationIdleHook( void )
{
  /* vApplicationIdleHook() will only be called if configUSE_IDLE_HOOK is set
   to 1 in FreeRTOSConfig.h. It will be called on each iteration of the idle
   task. It is essential that code added to this hook function never attempts
   to block in any way (for example, call xQueueReceive() with a block time
   specified, or call vTaskDelay()). If the application makes use of the
   vTaskDelete() API function (as this demo application does) then it is also
   important that vApplicationIdleHook() is permitted to return to its calling
   function, because it is the responsibility of the idle task to clean up
   memory allocated by the kernel to any task that has since been deleted. */
   
/* add user code begin vApplicationIdleHook */
	static portTickType LastTick = 0U;
	static portTickType counter;                                //наш трудяга счетчик
	static portTickType max_count ;                             //максимальное значение счетчика, вычисляется при калибровке и соответствует 100% CPU idle

	++counter;                                                  //приращение счетчика

	if((LastTick + 250) < xTaskGetTickCount())                  //если прошло 250 тиков (250 мсек для моей платфрмы)    
	{                             
			LastTick = xTaskGetTickCount();
			if(counter > max_count) max_count = counter;            //это калибровка
			health.load = 10000U * (max_count - counter) / max_count;   //вычисляем текущую загрузку
			counter = 0;                                            //обнуляем счетчик

			/* Reload watchdog counter (Period - 200 ms) */
			wdt_counter_reload();
	}
/* add user code end vApplicationIdleHook */
}

void vApplicationTickHook( void )
{
  /* This function will be called by each tick interrupt if
   configUSE_TICK_HOOK is set to 1 in FreeRTOSConfig.h. User code can be
   added here, but the tick hook is called from an interrupt context, so
   code must not attempt to block, and only the interrupt safe FreeRTOS API
   functions can be used (those that end in FromISR()). */

/* add user code begin vApplicationTickHook */

/* add user code end vApplicationTickHook */
}

/* add user code begin 1 */

/* add user code end 1 */

/**
  * @brief  initializes all task.
  * @param  none
  * @retval none
  */
void freertos_task_create(void)
{
  /* create daemon_task task */
  xTaskCreate(daemon_task_func,
              "daemon_task",
              128,
              NULL,
              0,
              &daemon_task_handle);
}

/**
  * @brief  freertos init and begin run.
  * @param  none
  * @retval none
  */
void wk_freertos_init(void)
{
  /* enter critical */
  taskENTER_CRITICAL();

  freertos_task_create();
	
  /* exit critical */
  taskEXIT_CRITICAL();

  /* start scheduler */
  vTaskStartScheduler();
}

/**
  * @brief daemon_task function.
  * @param  none
  * @retval none
  */
void daemon_task_func(void *pvParameters)
{
  /* add user code begin daemon_task_func 0 */
	appl.start();
  /* add user code end daemon_task_func 0 */

  /* Infinite loop */
  while(1)
  {
  /* add user code begin daemon_task_func 1 */

  appl.daemon();
  /* add user code end daemon_task_func 1 */
  }
}


/* add user code begin 2 */

/* add user code end 2 */

