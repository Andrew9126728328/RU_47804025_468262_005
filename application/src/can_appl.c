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
#include <string.h>
#include <stdio.h>

static void can_rx_task_func(void *pvParameters);
static void can_tx_task_func(void *pvParameters);
static int appl_can_start(void);
static int appl_can_send(can_message_t *message);
const appl_can_t appl_can =
{
	.j1939_my_address = J1939_ADDRESS,
	.send = appl_can_send,
	.j1939 =  &j1939,
	.failure_pgn_65408 = &failure_pgn_65408,
	.start = appl_can_start,
};

#define txQueueLength 				(64U)
#define rxQueueLength 				(64U)

QueueHandle_t txQueue = NULL;		///< Очередь (буфферизация) на передачу CAN сообщений
QueueHandle_t rxQueue = NULL;		///< Очередь (буфферизация) на прием CAN сообщений

static void can_rx_process(can_message_t *message);

static int appl_can_start(void)
{ 
	int ret1, ret2;
	ret1 = xTaskCreate(can_rx_task_func,   "can_rx_task",    configMINIMAL_STACK_SIZE*8,  (void*)500, 	tskIDLE_PRIORITY + 2,		NULL);
  ret2 = xTaskCreate(can_tx_task_func,   "can_tx_task",    configMINIMAL_STACK_SIZE*8,  (void*)1, 		tskIDLE_PRIORITY + 3,		NULL);
	return ret1 && ret2;
}

static int can_is_xcp_connect(can_message_t *message);
/**
* @brief Задача обработки приема CAN сообщений
* @param[in] pvParameters принимает значение таймаута по приему в мс 
*/  
static void can_rx_task_func(void *pvParameters)
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
static void can_tx_task_func(void *pvParameters)
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
	if(pdTRUE == message->ext)                             /* Extended ID only */
	{
		if(pdTRUE == appl_can.failure_pgn_65408->is_request(message, appl_can.j1939_my_address))
		{
				appl_can.failure_pgn_65408->send_answer();
		}
		else if(pdTRUE == appl_can.j1939->is_request_sw(message, appl_can.j1939_my_address))
		{
				char str[64];
				if(sizeof(str) >= (sizeof(PROJECT) + sizeof(VERSION)))
				{
						sprintf(str,"%s*%s*",PROJECT,VERSION);
						int len = strlen(str);
						appl_can.j1939->send_sw(J1939_PRIORITY_6, appl_can.j1939_my_address, 2, str, len);
				}
		}
		/* Processing the reset command to the Bootloader */
		else if(pdTRUE == can_is_xcp_connect(message))
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