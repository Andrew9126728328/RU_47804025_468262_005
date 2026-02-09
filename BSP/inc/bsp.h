#ifndef __BSP_H__
#define __BSP_H__
#include "can.h"
#include "kbd.h"
#include "leds.h"
#include "drv_2605.h"

/*******************************************************************************/ 
/**
* @brief Объект bsp для работы с периферией микроконроллера
* @details Вся работа с переферийными устройствами микроконтроллера
* осуществляется только через этот объект, используя методы вложенного
* объекта конкретного устройства
*/  
typedef struct bsp_s
{
	int (*init)(void);                          	///< Метод инициализации BSP
	const can_t 			*can;												///< Доступ к CAN1 
	const kbd_t 			*kbd;												///< Доступ к клавиатуре 
	const leds_t			*leds;											///< Доступ к светодиодам 
	const drv_2605_t	*drv_2605;									///< Доступ к DRV2605 
}bsp_t;
extern const bsp_t bsp;
#endif /* __BSP_H__ */