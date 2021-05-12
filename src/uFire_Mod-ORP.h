#ifndef Mod_ORP_H
#define Mod_ORP_H

#include <Arduino.h>
#include <Wire.h>

#define ORP_MEASUREMENT_TIME 750
#define ORP_TEMP_MEASURE_TIME 750

#define UFIRE_MOD_ORP 0x0e

#define MEASURE_ORP_TASK 80      /*!< Command to start a pH measure */
#define MEASURE_TEMP_TASK 40    /*!< Command to measure temperature */
#define CALIBRATE_SINGLE_TASK 4 /*!< Command to calibrate the high point of the probe */
#define I2C_TASK 2              /*!< Command to change the i2c address */

#define HW_VERSION_REGISTER 0               /*!< hardware version register */
#define FW_VERSION_REGISTER 1               /*!< firmware version  register */
#define TASK_REGISTER 2                     /*!< task register */
#define STATUS_REGISTER 3                   /*!< status register */
#define MV_REGISTER 4                       /*!< mV register */
#define TEMP_C_REGISTER 8                   /*!< temperature in C register */
#define CALIBRATE_SINGLE_OFFSET_REGISTER 12 /*!< single-point calibration register */

namespace uFire
{
  namespace Mod_ORP
  {
    class i2c
    {
    public:
      const float &tempC = _tempC;
      const float &tempF = _tempF;
      const float &mV = _mV;
      const float &calibrationSingleOffset = _calibrationSingleOffset;
      const int &hwVersion = _hwVersion;
      const int &fwVersion = _fwVersion;
      const int &status = _status;

      typedef enum ORP_Error_e
      {
          STATUS_NO_ERROR,
          STATUS_SYSTEM_ERROR
      } ORP_Error_t;

      bool begin(TwoWire &wirePort = Wire, uint8_t address = UFIRE_MOD_ORP);
      bool connected();
      float calibrateSingle(float solution_mV, bool blocking = true);
      void getDeviceInfo();
      float measureORP(bool blocking = true);
      float measureTemp(bool blocking = true);
      void reset();
      void setDeviceInfo(float calibrationSingleOffset);
      void setI2CAddress(uint8_t i2cAddress);
      float update();

    private:
      uint8_t _address;
      TwoWire *_i2cPort;
      static float _mV;
      static float _tempC;
      static float _tempF;
      static float _calibrationSingleOffset;
      static int _hwVersion;
      static int _fwVersion;
      static int _status;

      void _updateRegisters();
      void _send_command(uint8_t command);
      void _write_4_bytes(uint8_t reg, float f);
      void _write_byte(uint8_t reg, uint8_t val);
      float _read_4_bytes(uint8_t reg);
      uint8_t _read_byte(uint8_t reg);
    };
  } // namespace Mod_ORP
} // namespace uFire
#endif // ifndef Mod_ORP_H