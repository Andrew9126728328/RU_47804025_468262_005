#ifndef __MAIN_APPL_H__
#define __MAIN_APPL_H__

#include "bsp.h"
#include "can_appl.h"
#include "ai_appl.h"
#include "failure_pgn_65408.h"

#define STATISTIC_PERIOD			(100U)
#define J1939_ADDRESS					(207U)
#define J1939_BROADCAST				(255U)
#define J1939_REQ_PGN_0xEA00	(0xEA00)
#define XCP_MASTER_ID 				(0x02770801U) 

typedef struct health_s
{
    crm_clocks_freq_type crm_clocks_freq;
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
	const appl_can_t *can;
	const appl_ai_t *ai;
}api_t;
extern const api_t appl;


#endif /* __MAIN_APPL_H__ */
