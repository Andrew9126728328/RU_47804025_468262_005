/**
*@file
*@authors ГрВПО
*@copyright ООО "ЦПТ Агроцифра"
*@brief Файл реализации объектов управления тактильным двигателем
*@details В данном файле содержатся все необходимые includ, и реализация объекта drv_2605 BSP.\n
* Только на этом уровне подключаются и используются библиотеки производителя микроконтроллера.
*/
#include "drv_2605.h"
#include "at32f415_wk_config.h"
#include "i2c_application.h"

#define DRV2605_I2C								I2C2
/**************************************************************************/ 
#define DRV2605_ADDR							0x5A	///< Device I2C address
/**************************************************************************/ 
#define DRV2605_REG_STATUS				0x00	///< Status register
#define DRV2605_REG_MODE					0x01	///< Mode register
#define DRV2605_MODE_INTTRIG			0x00	///< Internal trigger mode
#define DRV2605_MODE_EXTTRIGEDGE	0x01	///< External edge trigger mode
#define DRV2605_MODE_EXTTRIGLVL		0x02	///< External level trigger mode
#define DRV2605_MODE_PWMANALOG		0x03	///< PWM/Analog input mode
#define DRV2605_MODE_AUDIOVIBE		0x04	///< Audio-to-vibe mode
#define DRV2605_MODE_REALTIME			0x05	///< Real-time playback (RTP) mode
#define DRV2605_MODE_DIAGNOS			0x06	///< Diagnostics mode
#define DRV2605_MODE_AUTOCAL			0x07	///< Auto calibration mode

#define DRV2605_REG_RTPIN					0x02	///< Real-time playback input register
#define DRV2605_REG_LIBRARY				0x03	///< Waveform library selection register
#define DRV2605_REG_WAVESEQ1			0x04	///< Waveform sequence register 1
#define DRV2605_REG_WAVESEQ2			0x05	///< Waveform sequence register 2
#define DRV2605_REG_WAVESEQ3			0x06	///< Waveform sequence register 3
#define DRV2605_REG_WAVESEQ4			0x07	///< Waveform sequence register 4
#define DRV2605_REG_WAVESEQ5			0x08	///< Waveform sequence register 5
#define DRV2605_REG_WAVESEQ6			0x09	///< Waveform sequence register 6
#define DRV2605_REG_WAVESEQ7			0x0A	///< Waveform sequence register 7
#define DRV2605_REG_WAVESEQ8			0x0B	///< Waveform sequence register 8

#define DRV2605_REG_GO						0x0C	///< Go register
#define DRV2605_REG_OVERDRIVE			0x0D	///< Overdrive time offset register
#define DRV2605_REG_SUSTAINPOS		0x0E	///< Sustain time offset, positive register
#define DRV2605_REG_SUSTAINNEG		0x0F	///< Sustain time offset, negative register
#define DRV2605_REG_BREAK					0x10	///< Brake time offset register
#define DRV2605_REG_AUDIOCTRL			0x11	///< Audio-to-vibe control register
#define DRV2605_REG_AUDIOLVL			0x12	///< Audio-to-vibe minimum input level register
#define DRV2605_REG_AUDIOMAX			0x13	///< Audio-to-vibe maximum input level register
#define DRV2605_REG_AUDIOOUTMIN		0x14	///< Audio-to-vibe minimum output drive register
#define DRV2605_REG_AUDIOOUTMAX		0x15	///< Audio-to-vibe maximum output drive register
#define DRV2605_REG_RATEDV				0x16	///< Rated voltage register
#define DRV2605_REG_CLAMPV				0x17	///< Overdrive clamp voltage register
#define DRV2605_REG_AUTOCALCOMP		0x18	///< Auto-calibration compensation result register
#define DRV2605_REG_AUTOCALEMP		0x19	///< Auto-calibration back-EMF result register
#define DRV2605_REG_FEEDBACK			0x1A	///< Feedback control register
#define DRV2605_REG_CONTROL1			0x1B	///< Control1 Register
#define DRV2605_REG_CONTROL2			0x1C	///< Control2 Register
#define DRV2605_REG_CONTROL3			0x1D	///< Control3 Register
#define DRV2605_REG_CONTROL4			0x1E	///< Control4 Register
#define DRV2605_REG_VBAT					0x21	///< Vbat voltage-monitor register
#define DRV2605_REG_LRARESON			0x22	///< LRA resonance-period register  
/**************************************************************************/ 
static int drv_2605_init(void);
static void drv_2605_setWaveform(uint8_t slot, uint8_t w);
static void drv_2605_selectLibrary(uint8_t lib);
static void drv_2605_go(void);
static void drv_2605_stop(void);
static void drv_2605_setMode(uint8_t mode);
static void drv_2605_setRealtimeValue(uint8_t rtp);
static void drv_2605_useERM(void);
static void drv_2605_useLRA(void);
/**************************************************************************/ 	
static i2c_handle_type hi2c2 = { .i2cx = NULL, };
static uint8_t readRegister8(uint8_t reg);
static void writeRegister8(uint8_t reg, uint8_t val);
/**************************************************************************/ 	
const drv_2605_t drv_2605 = 
{
	.init = drv_2605_init,
	.setWaveform = drv_2605_setWaveform,
	.selectLibrary = drv_2605_selectLibrary,
	.go = drv_2605_go,
	.stop = drv_2605_stop,
	.setMode = drv_2605_setMode,
	.setRealtimeValue = drv_2605_setRealtimeValue,
	.useERM = drv_2605_useERM,
	.useLRA = drv_2605_useLRA,
};
/**************************************************************************/
/*!
  @brief  Setup the HW
  @return Always true
*/
/**************************************************************************/
static int drv_2605_init(void) {
	hi2c2.i2cx = DRV2605_I2C;
	gpio_bits_reset(VIBR_IN_GPIO_PORT, VIBR_IN_PIN);
	gpio_bits_set(VIBR_EN_GPIO_PORT, VIBR_EN_PIN);
//  if (!i2c_dev->begin())
//    return false;
  // uint8_t id = readRegister8(DRV2605_REG_STATUS);
  // Serial.print("Status 0x"); Serial.println(id, HEX);

  writeRegister8(DRV2605_REG_MODE, 0x00); // out of standby

  writeRegister8(DRV2605_REG_RTPIN, 0x00); // no real-time-playback

  writeRegister8(DRV2605_REG_WAVESEQ1, 1); // strong click
  writeRegister8(DRV2605_REG_WAVESEQ2, 0); // end sequence

  writeRegister8(DRV2605_REG_OVERDRIVE, 0); // no overdrive

  writeRegister8(DRV2605_REG_SUSTAINPOS, 0);
  writeRegister8(DRV2605_REG_SUSTAINNEG, 0);
  writeRegister8(DRV2605_REG_BREAK, 0);
  writeRegister8(DRV2605_REG_AUDIOMAX, 0x64);

  // ERM open loop

  // turn off N_ERM_LRA
  writeRegister8(DRV2605_REG_FEEDBACK,
                 readRegister8(DRV2605_REG_FEEDBACK) & 0x7F);
  // turn on ERM_OPEN_LOOP
  writeRegister8(DRV2605_REG_CONTROL3,
                 readRegister8(DRV2605_REG_CONTROL3) | 0x20);

  return pdTRUE;
}      
/**************************************************************************/
/*!
  @brief Select the haptic waveform to use.
  @param slot The waveform slot to set, from 0 to 7
  @param w The waveform sequence value, refers to an index in the ROM library.

    Playback starts at slot 0 and continues through to slot 7, stopping if it
  encounters a value of 0. A list of available waveforms can be found in
  section 11.2 of the datasheet: http://www.adafruit.com/datasheets/DRV2605.pdf
*/
/**************************************************************************/
static void drv_2605_setWaveform(uint8_t slot, uint8_t w) {
  writeRegister8(DRV2605_REG_WAVESEQ1 + slot, w);
}      
/**************************************************************************/
/*!
  @brief Select the waveform library to use.
  @param lib Library to use, 0 = Empty, 1-5 are ERM, 6 is LRA.

    See section 7.6.4 in the datasheet for more details:
  http://www.adafruit.com/datasheets/DRV2605.pdf
*/
/**************************************************************************/
static void drv_2605_selectLibrary(uint8_t lib) {
  writeRegister8(DRV2605_REG_LIBRARY, lib);
}

/**************************************************************************/
/*!
  @brief Start playback of the waveforms (start moving!).
*/
/**************************************************************************/
static void drv_2605_go(void) { writeRegister8(DRV2605_REG_GO, 1); }

/**************************************************************************/
/*!
  @brief Stop playback.
*/
/**************************************************************************/
static void drv_2605_stop(void) { writeRegister8(DRV2605_REG_GO, 0); }

/**************************************************************************/
/*!
  @brief Set the device mode.
  @param mode Mode value, see datasheet section 7.6.2:
  http://www.adafruit.com/datasheets/DRV2605.pdf

    0: Internal trigger, call go() to start playback\n
    1: External trigger, rising edge on IN pin starts playback\n
    2: External trigger, playback follows the state of IN pin\n
    3: PWM/analog input\n
    4: Audio\n
    5: Real-time playback\n
    6: Diagnostics\n
    7: Auto calibration
*/
/**************************************************************************/
static void drv_2605_setMode(uint8_t mode) {
  writeRegister8(DRV2605_REG_MODE, mode);
}

/**************************************************************************/
/*!
  @brief Set the realtime value when in RTP mode, used to directly drive the
  haptic motor.
  @param rtp 8-bit drive value.
*/
/**************************************************************************/
static void drv_2605_setRealtimeValue(uint8_t rtp) {
  writeRegister8(DRV2605_REG_RTPIN, rtp);
}  
/**************************************************************************/
/*!
  @brief Use ERM (Eccentric Rotating Mass) mode.
*/
/**************************************************************************/
static void drv_2605_useERM(void) {
  writeRegister8(DRV2605_REG_FEEDBACK,
                 readRegister8(DRV2605_REG_FEEDBACK) & 0x7F);
}

/**************************************************************************/
/*!
  @brief Use LRA (Linear Resonance Actuator) mode.
*/
/**************************************************************************/
static void drv_2605_useLRA(void) {
  writeRegister8(DRV2605_REG_FEEDBACK,
                 readRegister8(DRV2605_REG_FEEDBACK) | 0x80);
} 
/**************************************************************************/
/*!
  @brief Read an 8-bit register.
  @param reg The register to read.
  @return 8-bit value of the register.
*/
/**************************************************************************/
static uint8_t readRegister8(uint8_t reg) {
	uint8_t value = 0;
	if(DRV2605_I2C == hi2c2.i2cx)
	{
		if(I2C_OK != i2c_memory_read(&hi2c2, I2C_MEM_ADDR_WIDIH_8, DRV2605_ADDR, (uint16_t)reg, &value, 1U, 5))
		{
			/* Need to reboot i2c */
		}
	}
  return value;
}

/**************************************************************************/
/*!
  @brief Write an 8-bit register.
  @param reg The register to write.
  @param val The value to write.
*/
/**************************************************************************/
static void writeRegister8(uint8_t reg, uint8_t val) {
	if(DRV2605_I2C == hi2c2.i2cx)
	{
		if(I2C_OK != i2c_memory_write(&hi2c2, I2C_MEM_ADDR_WIDIH_8, DRV2605_ADDR, (uint16_t)reg, &val, 1U, 5))
		{
			/* Need to reboot i2c */
		}
	}
}