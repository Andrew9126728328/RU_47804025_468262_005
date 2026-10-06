#ifndef __MAIN_APPL_H__
#define __MAIN_APPL_H__

#include "bsp.h"
#include "signature.h"
#include "can_appl.h"
#include "ai_appl.h"
#include "kbd_appl.h"
#include "enc_appl.h"
#include "hv_motor_appl.h"

#define STATISTIC_PERIOD			(100U)
#define J1939_ADDRESS					(207U)
#define J1939_BROADCAST				(255U)
#define XCP_MASTER_ID 				(0x02770801U) 

typedef struct health_s
{
    uint32_t 						 sclk_freq; 
    uint32_t             tasks;
    uint32_t             heap;
    uint32_t             load;
    uint64_t             uptime;
    uint32_t             can_rx_error_cnt;
    uint32_t             can_rx_status;
}health_t;
extern health_t health;

typedef struct api_s
{
	void (*start)(void);
	void (*daemon)(void);
	void (*idle)(void);
	void (*tick)(void);
	const appl_can_t *can;
	const appl_ai_t *ai;
	const appl_kbd_t *kbd;
	const appl_enc_t *enc;
	const appl_hv_motor_t *hv_motor;
}api_t;
extern const api_t appl;


#endif /* __MAIN_APPL_H__ */
