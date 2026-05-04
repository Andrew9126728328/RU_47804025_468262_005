#ifndef __DRV_2605_H__
#define __DRV_2605_H__
#include <stdint.h>

#include "at32f415.h" 

/**************************************************************************/
/*!
  @brief The DRV2605 driver object.
*/
/**************************************************************************/
typedef struct drv_2605_s
{
	int (*init)(void);
	void (*setWaveform)(uint8_t slot, uint8_t w);
  void (*selectLibrary)(uint8_t lib);
  void (*go)(void);
  void (*stop)(void);
  void (*setMode)(uint8_t mode);
  void (*setRealtimeValue)(uint8_t rtp);
  // Select ERM (Eccentric Rotating Mass) or LRA (Linear Resonant Actuator)
  // vibration motor The default is ERM, which is more common
  void (*useERM)(void);
  void (*useLRA)(void);
}drv_2605_t;
extern const drv_2605_t drv_2605;

#if 0
class Adafruit_DRV2605 {
public:
  Adafruit_DRV2605(void);
  bool begin(TwoWire *theWire = &Wire);

  bool init();
  void writeRegister8(uint8_t reg, uint8_t val);
  uint8_t readRegister8(uint8_t reg);
  void setWaveform(uint8_t slot, uint8_t w);
  void selectLibrary(uint8_t lib);
  void go(void);
  void stop(void);
  void setMode(uint8_t mode);
  void setRealtimeValue(uint8_t rtp);
  // Select ERM (Eccentric Rotating Mass) or LRA (Linear Resonant Actuator)
  // vibration motor The default is ERM, which is more common
  void useERM();
  void useLRA();

private:
  Adafruit_I2CDevice *i2c_dev = NULL; ///< Pointer to I2C bus interface
};   
#endif
#endif /* __DRV_2605_H__ */