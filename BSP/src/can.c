/**
*@file
*@authors ГрВПО
*@copyright ООО "ЦПТ Агроцифра"
*@brief Файл реализации объектов CAN
*@details В данном файле содержатся все необходимые includ, и реализация CAN BSP.\n
* Только на этом уровне подключаются и используются библиотеки производителя микроконтроллера.
*/
#include "can.h"

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"
#include "timers.h"

static int can_send(can_message_t *message);
static int can_receive(can_rx_fifo_num_type fifo_number, can_message_t *message);

/*******************************************************************************/
/**
* @brief Экземпляр объекта управления CAN1 AT32F415
*/ 
const can_t can = 
{
	.send = can_send,
	.receive = can_receive,
};
/**
* @brief Реализация метода send() для CAN1 BSP на основе микроконтроллера AT32F415
* @param[in] message принимает указатель на сообщение для отправки 
* @param[out] ret возвращает код ошибки
*/   
static int can_send(can_message_t *message)
{
	int ret = pdFALSE;
	can_tx_message_type ll_can_tx_message;
	if(message->length <= 8)
	{
			ll_can_tx_message.dlc = message->length;
			ll_can_tx_message.frame_type = CAN_TFT_DATA;
			if(message->ext == pdTRUE)
			{
					ll_can_tx_message.id_type = CAN_ID_EXTENDED;
					ll_can_tx_message.extended_id = message->id;
			}else
			{
					ll_can_tx_message.id_type = CAN_ID_STANDARD;
					ll_can_tx_message.standard_id = message->id;
			}
			ll_can_tx_message.data[0] = message->data[0];
			ll_can_tx_message.data[1] = message->data[1];
			ll_can_tx_message.data[2] = message->data[2];
			ll_can_tx_message.data[3] = message->data[3];
			ll_can_tx_message.data[4] = message->data[4];
			ll_can_tx_message.data[5] = message->data[5];
			ll_can_tx_message.data[6] = message->data[6];
			ll_can_tx_message.data[7] = message->data[7];

			ret = (CAN_TX_STATUS_NO_EMPTY != can_message_transmit(CAN1, &ll_can_tx_message))? pdTRUE:pdFALSE; 
	}
	return ret;
}
/**
* @brief Реализация метода receive() для CAN1 BSP на основе микроконтроллера AT32F415
* @param[in] fifo_number принимает номер FIFO для проверки 
* @param[in] message принимает указатель на буффер для принятого сообщения 
* @param[out] ret возвращает код ошибки
*/  
static int can_receive(can_rx_fifo_num_type fifo_number, can_message_t *message)
{
	int ret = pdFALSE;
	can_rx_message_type ll_can_rx_message;
	can_message_receive(CAN1, fifo_number, &ll_can_rx_message);
	if(CAN_TFT_DATA == ll_can_rx_message.frame_type)
	{

			if(CAN_ID_EXTENDED == ll_can_rx_message.id_type)
			{
					message->ext = pdTRUE;
					message->id = ll_can_rx_message.extended_id;
			}else
			{
					message->ext = pdFALSE;
					message->id = ll_can_rx_message.standard_id;
			}
			message->length = ll_can_rx_message.dlc;
			message->data[0] = ll_can_rx_message.data[0];
			message->data[1] = ll_can_rx_message.data[1];
			message->data[2] = ll_can_rx_message.data[2];
			message->data[3] = ll_can_rx_message.data[3];
			message->data[4] = ll_can_rx_message.data[4];
			message->data[5] = ll_can_rx_message.data[5];
			message->data[6] = ll_can_rx_message.data[6];
			message->data[7] = ll_can_rx_message.data[7];
			ret = pdTRUE;
	}
	return ret;
}
