#ifndef __HV_MOTOR_APPL_H__
#define __HV_MOTOR_APPL_H__

typedef struct appl_hv_motor_s
{
	int (*start)(void);
}appl_hv_motor_t;

extern const appl_hv_motor_t appl_hv_motor;
#endif /* __HV_MOTOR_APPL_H__ */
