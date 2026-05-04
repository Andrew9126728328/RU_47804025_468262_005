#ifndef __ADC_H__
#define __ADC_H__


#include <stdint.h>

#include "at32f415.h" 

/*******************************************************************************/ 
/**
* @brief Объект adc для работы с ADC BSP
* @details Вся работа с аппаратурой ADC1 осуществляется только через этот объект
*/ 
typedef struct adc_s
{
	const uint16_t resolution;
  const float v_ref_int;
  const float v_ref;
	const float offset;
	const float scaler;
	const float overvoltage;
  const float undervoltage;
	void (*start)(void);
	void (*calibrate)(void);
}adc_t;
extern const adc_t adc;

#endif /* __ADC_H__ */
