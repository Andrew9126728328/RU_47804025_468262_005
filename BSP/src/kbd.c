/**
*@file
*@authors ГрВПО
*@copyright ООО "ЦПТ Агроцифра"
*@brief Файл реализации объектов CAN
*@details В данном файле содержатся все необходимые includ, и реализация объекта kbd BSP.\n
* Только на этом уровне подключаются и используются библиотеки производителя микроконтроллера.
*/
#include "bsp.h"

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"
#include "timers.h"

/* PORT/PIN для строк клавиатуры */
struct row_ctrl_s
{
    gpio_type *port;
    uint16_t   pin;
};
static const struct row_ctrl_s row_ctrl[ROW_TOTAL] =
{
#define key_row_xmacro(_ndx, _port, _pin) { .port=_port, .pin=_pin},
	KEY_ROW_XLIST(key_row_xmacro)
#undef key_row_xmacro   
};
/* PORT/PIN для столбцов клавиатуры */
struct col_ctrl_s
{
    gpio_type *port;
    uint16_t   pin;
};
static const struct col_ctrl_s col_ctrl[COL_TOTAL] =
{
#define key_column_xmacro(_ndx, _port, _pin) { .port=_port, .pin=_pin},
	KEY_COL_XLIST(key_column_xmacro)
#undef key_column_xmacro   
};

static void kbd_set_column(size_t column);
static void kbd_reset_column(size_t column);
static int kbd_read_row(size_t row);
/*******************************************************************************/
/**
* @brief Экземпляр объекта управления kbd AT32F415
*/
const kbd_t kbd = 
{
	.set_column = kbd_set_column,
	.reset_column = kbd_reset_column,
	.read_row = kbd_read_row,
};
/**
* @brief Реализация метода kbd_set_column() для kbd BSP на основе микроконтроллера AT32F415
* @param[in] column принимает enum столбца сканирования клавиатуры для установки в 1
*/ 
static void kbd_set_column(size_t column)
{
	gpio_bits_set(col_ctrl[column].port, col_ctrl[column].pin);
}
/**
* @brief Реализация метода kbd_reset_column() для kbd BSP на основе микроконтроллера AT32F415
* @param[in] column принимает enum столбца сканирования клавиатуры для установки в 0
*/ 
static void kbd_reset_column(size_t column)
{
	gpio_bits_reset(col_ctrl[column].port, col_ctrl[column].pin);   /* Set low level for each columns */
}
/**
* @brief Реализация метода kbd_read_row() для kbd BSP на основе микроконтроллера AT32F415
* @param[in] row принимает enum строки сканирования клавиатуры для считывания
* @param[out] возвращает считанное состояние
*/ 
static int kbd_read_row(size_t row)
{
	int ret = pdFALSE;
	if(SET == gpio_input_data_bit_read(row_ctrl[row].port, row_ctrl[row].pin))   /* Read each rows */
	{
		ret = pdTRUE;
	}
	return ret;
}