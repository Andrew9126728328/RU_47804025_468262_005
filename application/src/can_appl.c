/**
*@file
*@authors ГрВПО
*@copyright ООО "ЦПТ Агроцифра"
*@brief Файл реализации задач приложеня CAN
*@details В данном файле содержатся все необходимые includ, и реализация задач и функций, обеспечивающих CAN интерфейс.
*/
#include "freertos_app.h"
#include "main_appl.h"
#include "bsp.h"


static int appl_can_send(can_message_t *message);
const appl_can_t appl_can =
{
	.j1939_my_address = J1939_ADDRESS,
	.send = appl_can_send,
};

#define txQueueLength 				(64U)
#define rxQueueLength 				(64U)

QueueHandle_t txQueue = NULL;		///< Очередь (буфферизация) на передачу CAN сообщений
QueueHandle_t rxQueue = NULL;		///< Очередь (буфферизация) на прием CAN сообщений

static void can_rx_process(can_message_t *message);
static int can_is_xcp_connect(can_message_t *message);
/**
* @brief Задача обработки приема CAN сообщений
* @param[in] pvParameters принимает значение таймаута по приему в мс 
*/  
void can_rx_task_func(void *pvParameters)
{
	int timeout = (NULL != pvParameters)? (int)pvParameters: 500;
	can_message_t message = {0};
	//vTaskDelay(pdMS_TO_TICKS(300U));
	rxQueue = xQueueCreate(rxQueueLength, sizeof(can_message_t));
  /* Infinite loop */
  while(1)
  {
     if(pdPASS == xQueueReceive(rxQueue, &message, (TickType_t)timeout))
		 {
			 can_rx_process(&message);
			 health.can_rx_status = pdTRUE;
		 }
		 else
		 {
			 health.can_rx_status = pdFALSE;
		 }
  }	
}
/**
* @brief Задача обработки передачи CAN сообщений
* @param[in] pvParameters принимает значение времени ожидания при занятом передатчике в мс 
*/ 
void can_tx_task_func(void *pvParameters)
{
	int wait_time = (NULL != pvParameters)? (int)pvParameters: 1;
	can_message_t message = {0};
	//vTaskDelay(pdMS_TO_TICKS(300U));
	txQueue = xQueueCreate(txQueueLength, sizeof(can_message_t));
	failure_pgn_65408.init();
	for(;;)
	{
			/* Wait tx message */
			xQueueReceive(txQueue, &message, portMAX_DELAY);
			/* Send message */
			while(pdTRUE != bsp.can->send(&message))
			{
					/* Wait until mailboxes are free */
					vTaskDelay(wait_time);
			}
	}
}
/**
* @brief Функция обработки принятого CAN сообщения
* @param[in] message принимает CAN сообщение для парсинга и дальнейшей обработки 
*/ 
static void can_rx_process(can_message_t *message)
{
	if(message->ext)                             /* Extended ID only */
	{
		/* Processing the reset command to the Bootloader */
		if(pdTRUE == can_is_xcp_connect(message))
		{
				vTaskSuspendAll();
				while(1)                        /* WDT Reset to Bootloader */
				{
						__NOP();
				}
		}
	}
}
/**
* @brief Функция проверки принятого CAN сообщения на команду reboot
* @param[in] message принимает CAN сообщение для анализа 
*/
static int can_is_xcp_connect(can_message_t *message)
{
    int ret = pdFALSE;
    if(XCP_MASTER_ID == (message->id & 0x03ffffff))                			/* XCP master CAN ID validation w/o priority */
    {
        if( (0x0ff == message->data[0]) && (0x00 == message->data[1]))  /* XCP Connect command validation */
        {
            ret = pdTRUE;
        }
    }
    return ret;
}
static int appl_can_send(can_message_t *message)
{
	extern QueueHandle_t txQueue;
	int ret = pdFALSE;
	if(NULL != txQueue)
	{
		/* Send to Queue */
		ret = xQueueSend(txQueue, (void*)message,0);
	}
	return ret;
}