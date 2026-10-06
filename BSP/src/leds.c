/**
*@file
*@authors ГрВПО
*@copyright ООО "ЦПТ Агроцифра"
*@brief Файл реализации объектов управления светодиодами
*@details В данном файле содержатся все необходимые includ, и реализация объекта leds BSP.\n
* Только на этом уровне подключаются и используются библиотеки производителя микроконтроллера.
*/
#include <string.h>
#include "bsp.h"
#include "i2c_application.h"

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"
#include "timers.h"

enum
{
	AW9523_D7,
	AW9523_D8,
	AW9523_D9,
	AW9523_TOTAL,
};
#define LED_PER_REG (LED_TOTAL/AW9523_TOTAL)

static uint16_t led_screen[AW9523_TOTAL];
static void leds_task_func(void *pvParameters);
static void leds_init(void);
static int  leds_turn(int led, int color, int bright);

const leds_t leds = 
{
	.init = leds_init,
	.turn = leds_turn,
};

static void leds_init(void)
{
	memset(led_screen,0,sizeof(led_screen));
	vTaskDelay(pdMS_TO_TICKS(2));
	xTaskCreate(leds_task_func, "leds_task",  configMINIMAL_STACK_SIZE,  (void*)10, tskIDLE_PRIORITY + 4,  NULL);
}

static int  leds_turn(int led, int color, int bright)
{
	int ret = pdFALSE;
	if(led < LED_TOTAL)
	{ 
		int reg = led / LED_PER_REG;
		int pos = led % LED_PER_REG;
		/* Turn off */
		led_screen[reg] &= ~(3<<pos);
		if(0 != bright)
		{
			uint8_t mask;
			switch(color)
			{
				case LED_RED:
					mask = 1;
					break;
				case LED_WHITE:
					mask = 2;
					break;
				default:
					mask = 0;
					break;
			}
			/* Turn on */
			led_screen[reg] |= (mask<<pos);
		}
		ret = pdTRUE;
	}
	return ret;
}
static void leds_task_func(void *pvParameters)
{
	int refresh_time = (NULL != pvParameters)? (int)pvParameters: 20;
	for(;;)
	{
		vTaskDelay(pdMS_TO_TICKS(refresh_time));
		for(int reg=0;reg<AW9523_TOTAL;++reg)
		{
			while(SET == spi_i2s_flag_get(SPI2, SPI_I2S_TDBE_FLAG))
			{
				taskYIELD();
			}
			spi_i2s_data_transmit(SPI2, led_screen[reg]);
		}
	}
}
