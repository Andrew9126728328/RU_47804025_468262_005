/**
*@file
*@authors ГрВПО
*@copyright ООО "ЦПТ Агроцифра"
*@brief Файл реализации объектов ADCN
*@details В данном файле содержатся все необходимые includ, и реализация ADC BSP.\n
* Только на этом уровне подключаются и используются библиотеки производителя микроконтроллера.
*/

#define R21  (20000.0f)
#define R22  (2000.0f)
#define REF_INT	(1.2f)
#define V_REF (3.3f)
#define RESOLUTION (4096U)
				
static void adc_start(void);
static void adc_calibrate(void);

#include "adc.h"

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"
#include "timers.h"
/*******************************************************************************/
/**
* @brief Экземпляр объекта управления ADC1 AT32F415
*/ 
const adc_t adc = 
{
	.resolution = RESOLUTION,
  .v_ref_int = REF_INT,
  .v_ref = V_REF,
	.offset = 0.6f,
  .scaler = ((( V_REF / RESOLUTION) / R22)*(R21 + R22)),
	.overvoltage = 33.5f,
  .undervoltage = 21.6f,
	.start = adc_start,
	.calibrate = adc_calibrate,
};

static void adc_start(void)
{
	adc_enable(ADC1, TRUE);
  adc_ordinary_software_trigger_enable(ADC1, TRUE);
}
void adc_calibrate(void)
{
    /* adc calibration-------------------------------------------------------- */
    adc_calibration_init(ADC1);
    while(adc_calibration_init_status_get(ADC1));
    adc_calibration_start(ADC1);
    while(adc_calibration_status_get(ADC1));    
}