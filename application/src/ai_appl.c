/**
*@file
*@authors ГрВПО
*@copyright ООО "ЦПТ Агроцифра"
*@brief Файл реализации задач обработки аналоговых входов 
*@details В данном файле содержатся все необходимые includ, и реализация функций, обеспечивающих работу аналоговых входов.
*/
#include <string.h>

#include "freertos_app.h"
#include "main_appl.h"
#include "bsp.h"

static float ai_v24;
static float ai_v3v3;
static float ai_vref;
static float ai_temperature;	
uint16_t adc_raw[AI_TOTAL_CHANNELS]; 
	
static float appl_read_PWRvs(void){ return ai_v24;};		
static float appl_read_v3v3(void){ return ai_v3v3;};		
static float appl_read_temperature(void){ return ai_temperature;};	
const appl_ai_t appl_ai =
{
	.read_PWRvs = appl_read_PWRvs,
	.read_v3v3 = appl_read_v3v3,
	.read_temperature = appl_read_temperature,
};
void ai_measurement(void *pvParameters)
{
	ai_v24 = 0.0f;
	ai_v3v3 = 0.0f;
	ai_vref = 0.0f;
	ai_temperature = 0.0f;
	const float alpha = 0.975f;
	for(;;)
	{ 
		adc.start();
		vTaskDelay(pdMS_TO_TICKS(10U));
		//Adc::raw = adc_ordinary_conversion_data_get(ADC1); 
		ai_vref = adc_raw[AI_VREF] * adc.v_ref / adc.resolution;
		{
			float tmp_in,tmp_out;
			/* Obtain the temperature using the following formula:
					Temperature (in В°C) = {(V25 - VTS) / Avg_Slope} + 25.
			*/
			tmp_in = adc_raw[AI_TSENS] * (adc.v_ref_int / ai_vref) * adc.v_ref / adc.resolution;
			tmp_in = (1.32f - tmp_in) / (-4.34e-3)+ 25.0f;
			tmp_out = ai_temperature;
			/* READ!!! http://we.easyelectronics.ru/Theory/chestno-prostoy-cifrovoy-filtr.html */
			tmp_out = alpha * tmp_out + (1.0f - alpha) * tmp_in;    /* Canonical way EMA */ 
			ai_temperature = tmp_out;
		}
		{
			float tmp_in,tmp_out;
			tmp_in = adc_raw[AI_PWRvs] * adc.scaler * (adc.v_ref_int / ai_vref) + adc.offset;
			tmp_out = ai_v24;
			/* READ!!! http://we.easyelectronics.ru/Theory/chestno-prostoy-cifrovoy-filtr.html */
			tmp_out = alpha * tmp_out + (1.0f - alpha) * tmp_in;    /* Canonical way EMA */ 
			ai_v24 = tmp_out;
		}

		if(ai_v24 > adc.overvoltage)
		{
			appl.can->failure_pgn_65408->set(SPN_X3597,FMI_X3);
		}else if(ai_v24 < adc.undervoltage)
		{
			appl.can->failure_pgn_65408->set(SPN_X3597,FMI_X4);
		}else
		{
			appl.can->failure_pgn_65408->reset(SPN_X3597);
		}
		{
			float tmp_in,tmp_out;
			tmp_in = adc.v_ref * (adc.v_ref_int / ai_vref);
			tmp_out = ai_v3v3;
			/* READ!!! http://we.easyelectronics.ru/Theory/chestno-prostoy-cifrovoy-filtr.html */
			tmp_out = alpha * tmp_out + (1.0f - alpha) * tmp_in;    /* Canonical way EMA */ 
			ai_v3v3 = tmp_out;
		}
		if(ai_v3v3 > (adc.v_ref*1.1f))
		{
			appl.can->failure_pgn_65408->set(SPN_X3599,FMI_X3);
		}else if(ai_v3v3 < (adc.v_ref*0.9f))
		{
			appl.can->failure_pgn_65408->set(SPN_X3599,FMI_X4);
		}else
		{
			appl.can->failure_pgn_65408->reset(SPN_X3599);
		}
	}	
}