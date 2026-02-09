/**
*@file
*@authors ГрВПО
*@copyright ООО "ЦПТ Агроцифра"
*@brief Файл реализации объектов управления светодиодами
*@details В данном файле содержатся все необходимые includ, и реализация объекта leds BSP.\n
* Только на этом уровне подключаются и используются библиотеки производителя микроконтроллера.
*/
#include <string.h>
#include "leds.h"
#include "at32f415_wk_config.h"

enum
{
	D7_74HCT595,
	D8_74HCT595,
	D9_74HCT595,
	D10_74HCT595,
	D11_74HCT595,
	TOTAL_74HCT595,
};
#define LED_PER_REG (LED_TOTAL/TOTAL_74HCT595)

static uint8_t led_screen[TOTAL_74HCT595];
static TaskHandle_t leds_to_74HCT595_task_handle = NULL;
static void leds_to_74HCT595_task_func(void *pvParameters);
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
	gpio_bits_reset(SR_MR_GPIO_PORT, SR_MR_PIN);
	vTaskDelay(pdMS_TO_TICKS(2));
	gpio_bits_set(SR_MR_GPIO_PORT, SR_MR_PIN);
	xTaskCreate(leds_to_74HCT595_task_func, "leds_task",  128,  (void*)10, 4,  &leds_to_74HCT595_task_handle);
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
static void leds_to_74HCT595_task_func(void *pvParameters)
{
	int refresh_time = (NULL != pvParameters)? (int)pvParameters: 20;
	for(;;)
	{
		vTaskDelay(pdMS_TO_TICKS(refresh_time));
		gpio_bits_reset(SR_ST_GPIO_PORT, SR_ST_PIN);
		for(int reg=0;reg<TOTAL_74HCT595;++reg)
		{
			while(SET == spi_i2s_flag_get(SPI2, SPI_I2S_TDBE_FLAG))
			{
				taskYIELD();
			}
			spi_i2s_data_transmit(SPI2, led_screen[reg]);
		}
		gpio_bits_set(SR_ST_GPIO_PORT, SR_ST_PIN);
	}
}
