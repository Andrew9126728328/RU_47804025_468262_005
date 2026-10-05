#ifndef __CAN_APPL_H__
#define __CAN_APPL_H__

#include <stdint.h>
#include "can.h"
#include "j1939_appl.h"
#include "failure_pgn_65408.h"

typedef struct appl_can_s
{
	const uint8_t	j1939_my_address;
	int (*send)(can_message_t *message);
	const j1939_t *j1939;
	const failure_pgn_65408_t *failure_pgn_65408;
	int (*start)(void);
}appl_can_t;

extern const appl_can_t appl_can;

#endif /* __CAN_APPL_H__ */
