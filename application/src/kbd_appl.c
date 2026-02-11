/**
*@file
*@authors ГрВПО
*@copyright ООО "ЦПТ Агроцифра"
*@brief Файл реализации задач приложеня клавиатуры
*@details В данном файле содержатся все необходимые includ, и реализация задач и функций, обеспечивающих работу клавиатуры.
*/
#include <string.h>
#include "freertos_app.h"
#include "main_appl.h"
#include "bsp.h"

/***************************************************************************************
 * X macros for binding keys and CAN message
 *
 *                 	        Key_XX  	   COL_WEAK  COL_STRONG ROW_WEAK ROW_STRONG  Byte Pos Mask 	        
 */
#define KEY2MESSAGE_XLIST(key2message_xmacro) \
		key2message_xmacro(	    BUTTON_EXT,    COL_6,	 COL_6,	    ROW_5,  ROW_5,      1,   1,  0x0f) \
		key2message_xmacro(	    BUTTON_1,    	 COL_6,	 COL_6,	    ROW_1,  ROW_2,      1,   5,  0x03) \
		key2message_xmacro(	    BUTTON_2,      COL_1,	 COL_1,	    ROW_5,  ROW_5,      1,   7,  0x03) \
		key2message_xmacro(	    BUTTON_3,      COL_1,	 COL_1,	    ROW_1,  ROW_2,      2,   1,  0x03) \
		key2message_xmacro(	    BUTTON_4,      COL_1,	 COL_1,	    ROW_3,  ROW_4,      2,   7,  0x03) \
		key2message_xmacro(	    BUTTON_5,      COL_2,	 COL_2,	    ROW_1,  ROW_2,      3,   1,  0x03) \
		key2message_xmacro(	    BUTTON_6,      COL_2,	 COL_2,	    ROW_3,  ROW_4,      3,   3,  0x03) \
		key2message_xmacro(	    BUTTON_7,      COL_5,	 COL_5,	    ROW_1,  ROW_2,      3,   5,  0x03) \
		key2message_xmacro(	    BUTTON_8,      COL_5,	 COL_5,	    ROW_3,  ROW_4,      3,   7,  0x03) \
		key2message_xmacro(	    BUTTON_9,      COL_3,	 COL_3,	    ROW_1,  ROW_2,      4,   1,  0x03) \
		key2message_xmacro(	    BUTTON_10,     COL_3,	 COL_3,	    ROW_3,  ROW_4,      4,   3,  0x03) \
		key2message_xmacro(	    BUTTON_11,     COL_4,	 COL_4,	    ROW_1,  ROW_2,      4,   5,  0x03) \
		key2message_xmacro(	    BUTTON_12,     COL_4,	 COL_4,	    ROW_3,  ROW_4,      5,   1,  0x03) \
		key2message_xmacro(	    BUTTON_13,     COL_2,	 COL_2,	    ROW_5,  ROW_5,      5,   3,  0x03) \
		key2message_xmacro(	    BUTTON_14,     COL_3,	 COL_3,	    ROW_5,  ROW_5,      5,   5,  0x03) \
		key2message_xmacro(	    BUTTON_15,     COL_4,	 COL_4,	    ROW_5,  ROW_5,      5,   7,  0x03) \
		key2message_xmacro(	    BUTTON_16,     COL_5,	 COL_5,	    ROW_5,  ROW_5,      6,   1,  0x03) \
		key2message_xmacro(	    BUTTON_17,     COL_6,	 COL_6,	    ROW_4,  ROW_4,      6,   3,  0x03) \
		key2message_xmacro(	    BUTTON_18,     COL_6,	 COL_6,	    ROW_3,  ROW_3,      6,   5,  0x03) 


enum 
{
#define key2message_xmacro(_ndx, _col_weak, _col_strong, _row_weak, _row_strong, _byte, _pos, _mask) _ndx,
	KEY2MESSAGE_XLIST(key2message_xmacro)
#undef key2message_xmacro
    BUTTON_TOTAL
};

static size_t row,column;
static uint8_t scan_code[COL_TOTAL];
static uint8_t debounce_code[COL_TOTAL];
static SemaphoreHandle_t xSemaphoreEvent = NULL;		

static void keyboard_task_tx_engine(void *pvParameters);
static void keyboard_status2message(can_message_t *message);
/**
* @brief Задача сканирования клавиатуры
* @param[in] pvParameters принимает значение времени сканирования ряда в мс 
*/ 
void keyboard_task_func(void *pvParameters)
{
	int scan_time = (NULL != pvParameters)? (int)pvParameters: 5;
  /* Infinite loop */
	uint8_t raw_scan;
	//vTaskDelay(pdMS_TO_TICKS(300U));
	for(column = 0;column < COL_TOTAL;++column)
	{
			bsp.kbd->set_column(column);
			scan_code[column] = 0;
			debounce_code[column] = 0;
	}
	xSemaphoreEvent = xSemaphoreCreateBinary();
	xTaskCreate(keyboard_task_tx_engine, "keyboard_tx_engine",  configMINIMAL_STACK_SIZE*2,  (void*)5, tskIDLE_PRIORITY + 1,  NULL);
	for(;;)
	{
			for(column = 0;column < COL_TOTAL;++column)                     
			{
					raw_scan = 0;
					bsp.kbd->reset_column(column);									/* Set low level for each columns */
					vTaskDelay(pdMS_TO_TICKS(scan_time));
					for(row = 0;row < ROW_TOTAL;++row)                          
					{
							if(pdTRUE == bsp.kbd->read_row(row))   			/* Read each rows */
							{
									raw_scan &= ~(1<<row);
							}else
							{
									raw_scan |= (1<<row);
							}
					}
					bsp.kbd->set_column(column);
					if(raw_scan == scan_code[column])                 /* Debounce process */
					{
							if(scan_code[column] != debounce_code[column])
							{
									debounce_code[column] = scan_code[column];
									/* Send signal on change */
									xSemaphoreGive(xSemaphoreEvent);
							}
					}else
					{
							scan_code[column] = raw_scan;
					}
			}
	}
}
/**
* @brief Задача передачи в CAN сообщения о состоянии клавиатуры
* @param[in] pvParameters не используется 
*/ 
void keyboard_task_tx_engine(void *pvParameters)
{
	uint8_t repeat_time = 100U;
	uint8_t guard_time = 20U;
	extern QueueHandle_t txQueue;
	for(;;)
	{
			can_message_t message = {0};
			vTaskDelay(pdMS_TO_TICKS(guard_time));
			xSemaphoreTake(xSemaphoreEvent,pdMS_TO_TICKS(repeat_time-guard_time));
			keyboard_status2message(&message);
			/* Send to CAN Queue */
			if(NULL != txQueue)
			{
				xQueueSend(txQueue, (void*)&message,(TickType_t)0U);
			}
	}
}
/**
* @brief Функция формирования CAN сообщения о состоянии клавиатуры
* @param[in] message указатель на сформированное сообщение (должно быть выделено место) 
*/ 
static void keyboard_status2message(can_message_t *message)
{
    uint8_t priority = 3U;
    uint8_t pdu_format = 255U;
    uint8_t pdu_specific = 64U;
    uint8_t dlc = 8U;
    struct kbd2message_s
    {
        uint8_t col_weak;
        uint8_t col_strong;
        uint8_t row_weak;
        uint8_t row_strong;
        uint8_t byte;
        uint8_t bit;
        uint8_t mask;
    };
    struct kbd2message_s kbd2message[] = 
    {
#define key2message_xmacro(_ndx, _col_weak, _col_strong, _row_weak, _row_strong, _byte, _pos, _mask) \
    {.col_weak = _col_weak, .col_strong = _col_strong, .row_weak = _row_weak, .row_strong = _row_strong, .byte = _byte, .bit = _pos, .mask = _mask},
	KEY2MESSAGE_XLIST(key2message_xmacro)
#undef key2message_xmacro        
    };
    memset(message,0,sizeof(can_message_t));
    message->ext = pdTRUE;
    message->length = dlc;
    message->id = (priority<<26) | (pdu_format<<16) | (pdu_specific<<8) | J1939_ADDRESS;
    for(size_t button=0;button<BUTTON_TOTAL;++button)
    {
        uint8_t value = 0x00;;
        if(debounce_code[kbd2message[button].col_strong] & (1<<kbd2message[button].row_strong))
        {
            value = 0x02;
        }else if(debounce_code[kbd2message[button].col_weak] & (1<<kbd2message[button].row_weak))
        {
            value = 0x01;
        }
        message->data[kbd2message[button].byte-1] |= (value << (kbd2message[button].bit-1));
    }
    /* Put the measured voltage into the CAN message */
    uint16_t *msg_data_v24 = (uint16_t*)&message->data[6];
    *msg_data_v24 = 0x55aa;
    //*msg_data_v24 = (uint16_t)(Adc::getVoltage() * 100.0f);
}