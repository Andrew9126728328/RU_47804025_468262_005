#ifndef __AI_APPL_H__
#define __AI_APPL_H__

enum 
	{
			/* ADC1 Channels */
			AI_PWRvs  = 0U,           /**< ECU Power supply voltage */
			AI_VREF   = 1U,           /**< Reference voltage */
			AI_TSENS  = 2U,           /**< Themperature sensor voltage */
			AI_TOTAL_CHANNELS = 3U    /**< Total channels ADC1 */
	};
	
typedef struct appl_ai_s
{
	int (*start)(void);
	float (*read_PWRvs)(void);
	float (*read_v3v3)(void);
	float (*read_temperature)(void);
}appl_ai_t;
extern const appl_ai_t appl_ai;

#endif /* __AI_APPL_H__ */
	