#ifndef __J1939_APPL_H__
#define __J1939_APPL_H__

#include <stdint.h>
#include <stddef.h>
#include "can.h"

enum 
{
		J1939_PRIORITY_0 = 0U,    /**< CAN j1939 message priority 0 */
		J1939_PRIORITY_1 = 1U,    /**< CAN j1939 message priority 1 */
		J1939_PRIORITY_2 = 2U,    /**< CAN j1939 message priority 2 */
		J1939_PRIORITY_3 = 3U,    /**< CAN j1939 message priority 3 */
		J1939_PRIORITY_4 = 4U,    /**< CAN j1939 message priority 4 */
		J1939_PRIORITY_5 = 5U,    /**< CAN j1939 message priority 5 */
		J1939_PRIORITY_6 = 6U,    /**< CAN j1939 message priority 6 */
		J1939_PRIORITY_7 = 7U,     /**< CAN j1939 message priority 7 */
		J1939_PRIO_MASK = 0xFC000000,
};

typedef struct j1939_s
{
	int (*send_tp_bam)(uint32_t prio, uint8_t self_address, uint32_t pgn, const uint8_t *data, const size_t size);
	int (*is_request)(const can_message_t *msg, uint8_t selfSourceAddress);
	int (*is_request_sw)(const can_message_t *msg, uint8_t selfSourceAddress);
	int (*send_sw)(int priority, uint8_t selfSourceAddress, uint8_t numberOfFields, const char *str, const size_t len);
}j1939_t;

extern const j1939_t j1939;

#endif /* __J1939_APPL_H__ */
