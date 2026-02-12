/**
*@file
*@authors ГрВПО
*@copyright ООО "ЦПТ Агроцифра"
*@brief Файл реализации задач приложеня протокола J1939
*@details В данном файле содержатся все необходимые includ, и реализация задач и функций, обеспечивающих J1939 протокол.
*/
#include "freertos_app.h"
#include "main_appl.h"
#include "bsp.h"
#include <string.h>


#define J1939_TP_PAYLOAD  (7U)       		/**< Data payload per one CAN message */
enum
{
	J1939_REQ_PGN_0xEA00 							= 0xEA00,			/**< Request PGN */
	J1939_BROADCAST_REQ_PGN_0xEAFF 		= 0xEAFF,			/**< Request PGN broadcast */
	J1939_TP_PGN_CM_0xECFF 						= 0xECFF,    	/**< Transport protocol - connection management PGN */
	J1939_TP_PGN_DT_0xEBFF 						= 0xEBFF,    	/**< Transport protocol - data transfer PGN */
	J1939_PGN_SW_0xFEDA 							= 0xFEDA,			/**< SoftWare Identificator PGN */
};
static int tpSendMessage(uint32_t priority, uint8_t selfSourceAddress, uint32_t requestedPgn, const uint8_t *data, const size_t size);
static int j1939_is_request(const can_message_t *msg, uint8_t selfSourceAddress);
static int j1939_is_request_sw(const can_message_t *msg, uint8_t selfSourceAddress);
static int j1939_send_sw(int priority, uint8_t selfSourceAddress, uint8_t numberOfFields, const char *str, const size_t len);
const j1939_t j1939 = 
{
	.send_tp_bam = tpSendMessage,
	.send_sw = j1939_send_sw,
	.is_request_sw = j1939_is_request_sw,
	.is_request = j1939_is_request,
};
/**
 * \brief   Собирает идентификатор CAN сообщения по J1939
 * \param   priority - приоритет сообщения по J1939
 * \param   pgn - номер параметра по J1939 (PGN)
 * \param   sourceAddress - адрес источника сообщения по J1939
 * \return  идентификатор CAN сообщения (uint32_t)
 */
static uint32_t buildId(uint32_t priority, uint32_t pgn, uint8_t sourceAddress)
{
		return (((uint32_t)priority << 26U) | (pgn << 8U) | (uint32_t)sourceAddress);
}

/**
 * \brief   Извлекает из идентификатора CAN сообщения номер параметра (PGN)
 * \param   msgId - идентификатор CAN сообщения
 * \return  номер параметра (PGN) по J1939 (uint32_t)
 */
static uint32_t extractPgn(uint32_t msgId)
{
		return (~J1939_PRIO_MASK & msgId) >> 8U;
}

/**
 * \brief   Возвращает число CAN сообщений необходимое для отправки указанного
 *          количества байт данных транспортным протоколом J1939
 * \param   payloadSize - размер данных на отправку в байтах
 * \return  число CAN сообщений необходимое для отправки указанного
 *          количества байт данных транспортным протоколом J1939 (uint32_t)
 */
static uint32_t tpCountOfMessages(uint32_t payloadSize)
{
		uint32_t numberOfPackages = payloadSize / J1939_TP_PAYLOAD;

		if(payloadSize % J1939_TP_PAYLOAD)
		{
				++numberOfPackages;
		}

		return numberOfPackages;
}

/**
 * \brief   Отправить данные транспортным протоколом J1939
 * \param   priority - приоритет сообщения по J1939
 * \param   selfSourceAddress - адрес источника сообщения по J1939
 * \param   requestedPgn - номер парамерта (PGN) по J1939 данные которого необходимо отправить
 *                         транспортным протоколом
 * \param   data - указатель на данные которые необходимо отправить транспортным протоколом
 * \param   size - размер данных в байтах
 * \return  статус отправки данных (Status)
 *              @n Status::OK - сообщение успешно отправленно
 *              @n Status::BUSY - буффер CAN модуля переполнен
 */
static int tpSendMessage(uint32_t priority, uint8_t selfSourceAddress,
														uint32_t requestedPgn, const uint8_t *data, const size_t size)
{
		int status = pdFALSE;
		can_message_t msg;

		uint32_t messageCounter = 0U;
		uint32_t totalMessages = tpCountOfMessages(size);

		/* All messages have a length of 8 bytes */
		msg.length = 8U;
		msg.ext = pdTRUE;

		/* Part 1 BAM Send */
		msg.id = buildId(priority, J1939_TP_PGN_CM_0xECFF, selfSourceAddress);

		/* Transport protocol BAM control byte always 32 */
		msg.data[0U] = 32U;
		/* Message Size (Number of bytes) */
		msg.data[1U] = (uint8_t)size;
		msg.data[2U] = (uint8_t)(size >> 8U);
		/* Total number of packages */
		msg.data[3U] = (uint8_t)totalMessages;
		/* Reserved, must be 0xFF */
		msg.data[4U] = 0xFF;
		/* Parameter Group Number of the multi-packet message */
		msg.data[5U] = (uint8_t)requestedPgn;
		msg.data[6U] = (uint8_t)(requestedPgn >> 8U);
		msg.data[7U] = (uint8_t)(requestedPgn >> 16U);

		status = appl_can.send(&msg);

		/* Part 2 - data send */
		msg.ext = pdTRUE;
		msg.id = buildId(priority, J1939_TP_PGN_DT_0xEBFF, selfSourceAddress);

		while((status == pdTRUE) && (messageCounter < totalMessages))
		{
				/* Sequence Number (1 to 255) */
				msg.data[0U] = messageCounter + 1U;

				/* Data */
				for(uint32_t i = 0U; i < J1939_TP_PAYLOAD; ++i)
				{
						uint32_t pos = messageCounter * J1939_TP_PAYLOAD + i;

						if(pos < size)
						{
								msg.data[i + 1] = data[pos];
						}
						else
						{
								/* All unused data bytes in the last package are being set to 0xFF */
								msg.data[i + 1] = 0xFF;
						}
				}

				/* Send message */
				status = appl_can.send(&msg);
				++messageCounter;
		}
		
		return status;
}
/**
* \brief   Извлекает из идентификатора CAN сообщения номер параметра (PGN)
* \param   msgId - идентификатор CAN сообщения
* \return  номер параметра (PGN) по J1939 (uint32_t)
*/
static uint32_t extract_pgn(uint32_t msgId)
{

		return (~J1939_PRIO_MASK & msgId) >> 8U;
}
/**
* \brief   Проверяет запрос на отправку версии программного обеспечения ECU
* \param   msg - указатель на принятое can сообещение
* \param   selfSourceAddress - source address блока (по J1939)
* \return  статус запроса версии программного обеспечения ECU
*              @n pdTRUE - принят запрос на отправку версии программного обеспечения ECU
*              @n pdFALSE - запрос на отправку версии программного обеспечения ECU не найден
*/
static int j1939_is_request_sw(const can_message_t *msg, uint8_t selfSourceAddress)
{
	int ret = pdFALSE;
	uint16_t pgn = extract_pgn(msg->id);
	if((pgn == J1939_BROADCAST_REQ_PGN_0xEAFF) || (pgn == (pgn | selfSourceAddress)))
	{
			/* Check payload */
			if(*((uint32_t *)msg->data) == J1939_PGN_SW_0xFEDA)
			{
					ret = pdTRUE;
			}
	}

	return ret;	
}
/**
* \brief   Проверяет запрос 
* \param   msg - указатель на принятое can сообещение
* \param   selfSourceAddress - source address блока (по J1939)
* \return  статус запроса 
*              @n pdTRUE - принят запрос
*              @n pdFALSE - запрос на не найден
*/
static int j1939_is_request(const can_message_t *msg, uint8_t selfSourceAddress)
{
	int ret = pdFALSE;
	uint16_t pgn = extract_pgn(msg->id);
	if((pgn == J1939_BROADCAST_REQ_PGN_0xEAFF) || (pgn == (pgn | selfSourceAddress)))
	{
			ret = pdTRUE;
	}

	return ret;	
}
/**
* \brief   Отправить версию программного обеспечения ECU (максимум 64 ASCII символа)
*          Пример строки - char data[] = "01*24.1*25.09.24*"; Символ "*" является
*          разделителем полей сообщения
* \param   can - номер модуля can (CAN_1 или CAN_2)
* \param   priority - приоритет сообщения по J1939
* \param   selfSourceAddress - адрес источника сообщения по J1939
* \param   numberOfFields - количество полей в сообщении разделённых "*" по J1939
* \param   str - указатель на строку с версей программного обеспечения ECU
* \param   len - длина строки с версей программного обеспечения ECU (максимум 64 ASCII символа)
* \return  статус отправки данных (Status)
*              @n pdTRUE - сообщение успешно отправленно
*              @n pdFALSE - буффер CAN модуля переполнен
*/
static int j1939_send_sw(int priority, uint8_t selfSourceAddress, uint8_t numberOfFields, const char *str, const size_t len)
{
	
	uint8_t data[65U];

	//assert(len < 65U);

	data[0U] = numberOfFields;
	memcpy(&data[1U], str, len);

	return tpSendMessage(priority, selfSourceAddress, J1939_PGN_SW_0xFEDA, data, len + 1U);
}

