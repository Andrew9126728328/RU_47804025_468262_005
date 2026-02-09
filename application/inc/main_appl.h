#ifndef __MAIN_APPL_H__
#define __MAIN_APPL_H__

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

extern void appl_start(void);
extern void appl_daemon(void);

#endif /* __MAIN_APPL_H__ */
