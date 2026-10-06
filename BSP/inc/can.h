#ifndef __CAN_H__
#define __CAN_H__
#include <stdint.h>

/*******************************************************************************/ 
/**
* @brief Объект can message для использования в приложении
* @details Структура этого объекта отличается от внутреннего представления can message миктроконтроллера
*/ 
typedef struct can_message_s
{
    uint32_t id;      /* Message ID  */
    uint32_t ext;     /* Extended    */
    uint32_t length;  /* Data length */
    uint8_t  data[8]; /* Data        */
}can_message_t; 
/*******************************************************************************/ 
/**
* @brief Объект can для работы с CAN1 BSP
* @details Вся работа с аппаратурой CAN1 осуществляется только через этот объект
*/ 
typedef struct can_s
{
	int (*send)(can_message_t *message);
	int (*receive)(int fifo_number, can_message_t *message);
}can_t;
extern const can_t can;
#endif /* __CAN_H__ */
