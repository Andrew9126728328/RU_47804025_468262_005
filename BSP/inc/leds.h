#ifndef __LEDS_H__
#define __LEDS_H__

#include <stdint.h>

#include "at32f415.h" 

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"
#include "timers.h" 

enum
{
	LED_1,
	LED_2,
	LED_3,
	LED_4,
	LED_5,
	LED_6,
	LED_7,
	LED_8,
	LED_9,
	LED_10,
	LED_11,
	LED_12,
	LED_13,
	LED_14,
	LED_15,
	LED_16,
	LED_17,
	LED_18,
	LED_19,
	LED_20,
	LED_TOTAL,
};
enum
{
	LED_NONE,
	LED_RED,
	LED_WHITE,
};

typedef struct leds_s
{
	void (*init)(void);
	int  (*turn)(int led, int color, int bright);
}leds_t;
extern const leds_t leds;

#endif /* __LEDS_H__ */
