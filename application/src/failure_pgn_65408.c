
#include "freertos_app.h"
#include "main_appl.h"
#include "bsp.h"

static int failure_init(void);
static int failure_set(int spn,int fmi);
static int failure_reset(int spn);
static int failure_send_answer(void);
static int failure_is_request(can_message_t *msg, uint8_t my_address);

const failure_pgn_65408_t failure_pgn_65408 = 
{
	.pgn = 65408U,
	.init = failure_init,
	.set = failure_set,
	.reset = failure_reset,
	.send_answer = failure_send_answer,
	.is_request = failure_is_request,
};

/*
 *              SPN_NDX  	  	 SPN_VALUE   FMI         STATUS 	            	        
 */  
#define SPN_XLIST(spn_xmacro) \
		spn_xmacro(	SPN_3597,      SPN_X3597,  FMI_X4,     pdTRUE) \
		spn_xmacro(	SPN_3598,      SPN_X3598,  FMI_X4,     pdFALSE) \
		spn_xmacro(	SPN_3599,      SPN_X3599,  FMI_X4,     pdTRUE) \
		spn_xmacro(	SPN_0152,      SPN_X0152,  FMI_X14,    pdTRUE) \
		spn_xmacro(	SPN_2802,      SPN_X2802,  FMI_X12,    pdFALSE)
enum
{
#define spn_xmacro(_ndx, _spn, _fmi, _status) _ndx,
	SPN_XLIST(spn_xmacro)
#undef spn_xmacro 
    SPN_TOTAL
};

struct spn_state_s
{
    int spn;
    int fmi;
    int status;
};
static struct spn_state_s failure[SPN_TOTAL] = 
{
#define spn_xmacro(_ndx, _spn, _fmi, _status) {.spn = _spn, .fmi = _fmi, .status = _status},
	SPN_XLIST(spn_xmacro)
#undef spn_xmacro     
};

static const uint8_t priority = 6U;
static int build_can_message(size_t ndx, can_message_t *message);

static int failure_init(void)
{
    int ret = pdFALSE;
    ret = failure_pgn_65408.reset(SPN_X0152);     /* Send Board Reset SPN */
    return ret;
}     
static int failure_set(int spn,int fmi)
{
	int ret = pdFALSE;
	for (size_t ndx=0;ndx<SPN_TOTAL;++ndx)          /* Find among all SPNs */
	{
		if(failure[ndx].spn == spn)                 /* Found SPN */
		{
			if((failure[ndx].fmi != fmi) || (failure[ndx].status != pdTRUE))  /* Found FMI  & Need to change status*/
			{
					can_message_t message = {0};
					failure[ndx].fmi = fmi;                 /* Remember new status */                 
					failure[ndx].status = pdTRUE;
					/* Build CAN message */
					build_can_message(ndx,&message);        /* Create message */
					ret = appl.can->send(&message);   		  /* Send it */            
			}
			break;
		}
	}
	return ret;
}     
static int failure_reset(int spn)
{
	int ret = pdFALSE;

	for (size_t ndx=0;ndx<SPN_TOTAL;++ndx)              /* Find among all SPNs */
	{
		if(failure[ndx].spn == spn)                     /* Found SPN */
		{
			if(failure[ndx].status != pdFALSE)            /* Need to change status */
			{
					can_message_t message = {0};
					failure[ndx].status = pdFALSE;            /* Remember new status */ 
					/* Build CAN message */
					build_can_message(ndx,&message);        /* Create message */
					ret = appl.can->send(&message);   			/* Send it */
				}
			break;
		}
	}
	return ret;
}     
static int failure_send_answer(void)
{
	int ret = pdFALSE;
	for (size_t ndx=0;ndx<SPN_TOTAL;++ndx)              /* Find among all SPNs */
	{
		if(failure[ndx].status == pdTRUE)                 /* Need to transmit */
		{
				can_message_t message = {0};
				/* Build CAN message */
				build_can_message(ndx,&message);            /* Create message */
				ret = appl.can->send(&message);   			/* Send it */
		}
	}
	if(pdFALSE == ret)                            /* No failures found */
	{
		can_message_t message = {0};
		/* Build CAN message */
		build_can_message(SPN_TOTAL,&message);          /* Create Zero answer */
		ret = appl.can->send(&message);   			/* Send it */
	}
	return ret;
} 
static int build_can_message(size_t ndx,can_message_t *message)
{
    int ret = pdFALSE;
    if(ndx <= SPN_TOTAL)
    {
        message->id = (priority<<26) | (failure_pgn_65408.pgn << 8) | appl_can.j1939_my_address;   /* Create ID */
        message->ext = pdTRUE;
        if(SPN_TOTAL == ndx)
        {
            message->length = 0U;               /* Zero answer */
        }else
        {
            message->length = 4U;               /* Pack failure into message */
            message->data[0] = (failure[ndx].spn) & 0x0ff;
            message->data[1] = ((failure[ndx].spn) >> 8) & 0x0ff;
            message->data[2] = (((failure[ndx].spn) >> 16) << 5)  & 0x0e0;
            message->data[2] |= (failure[ndx].fmi)  & 0x01f;
            message->data[3] = (failure[ndx].status)? 0x80U:0U;
        }
        ret = pdTRUE;
    }
    return ret;
} 

static int failure_is_request(can_message_t *message, uint8_t self_address)
{
	int ret = pdFALSE;
	if(pdTRUE == j1939.is_request(message,self_address))
	{
		/* PGN 59904 Failure request processing */
		uint32_t req_pgn = message->data[0] | (message->data[1]<<8) | (message->data[2]<<16);
		if(req_pgn == failure_pgn_65408.pgn)
		{
				ret = pdTRUE;
		}
	}
	return ret;
}