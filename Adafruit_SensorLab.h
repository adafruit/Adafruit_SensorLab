/**************************************************************************/
/*!
    @file     Adafruit_SensorLab.h
    @author   Limor Fried (Adafruit Industries)

    Arduino library for scientific sensor readings/fusions/manipulations

    Adafruit invests time and resources providing this open source code,
    please support Adafruit and open-source hardware by purchasing
    products from Adafruit!

*/
/**************************************************************************/

#ifndef ADAFRUIT_SENSORLAB_H
#define ADAFRUIT_SENSORLAB_H

#include <Adafruit_ADXL343.h>
#include <Adafruit_BME280.h>
#include <Adafruit_BMP280.h>
#include <Adafruit_DPS310.h>
#include <Adafruit_FXAS21002C.h>
#include <Adafruit_FXOS8700.h>
#include <Adafruit_ICM20649.h>
#include <Adafruit_ISM330DHCT.h>
#include <Adafruit_LIS2MDL.h>
#include <Adafruit_LIS3MDL.h>
#include <Adafruit_LSM303_Accel.h>
#include <Adafruit_LSM6DS33.h>
#include <Adafruit_LSM6DSOX.h>
#include <Adafruit_LSM9DS0.h>
#include <Adafruit_LSM9DS1.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_MSA301.h>
#include <Adafruit_Sensor.h>
#include <Arduino.h>

/**************************************************************************/
/*!
  @brief A handy object that will automatically default a wide range of
  common I2C sensors, makes it easy to perform generalized sensor measurements
  that compile on any platform and run with any hardware attached
*/
/**************************************************************************/
class Adafruit_SensorLab {
public:
  Adafruit_SensorLab(TwoWire *theWire = &Wire);
  void begin(uint32_t I2C_Frequency = 100000);

  Adafruit_Sensor *getTemperatureSensor(void);
  Adafruit_Sensor *getPressureSensor(void);
  Adafruit_Sensor *getHumiditySensor(void);
  Adafruit_Sensor *getAccelerometer(void);
  Adafruit_Sensor *getMagnetometer(void);
  Adafruit_Sensor *getGyroscope(void);

  float calculateAltitude(float currentPressure_hPa,
                          float originPressure_hPa = 1013.25);
  bool detectADXL34X(void);
  bool detectBMP280(void);
  bool detectBME280(void);
  bool detectDPS310(void);
  bool detectFXOS8700(void);
  bool detectFXAS21002(void);
  bool detectICM20649(void);
  bool detectISM330DHCT(void);
  bool detectLSM303A(void);
  bool detectLSM6DS33(void);
  bool detectLSM6DSOX(void);
  bool detectLSM9DS1(void);
  bool detectLSM9DS0(void);
  bool detectLIS2MDL(void);
  bool detectLIS3MDL(void);
  bool detectMPU6050(void);
  bool detectMSA301(void);

  static float mapf(float x, float in_min, float in_max, float out_min,
                    float out_max);
  static constexpr double DEGREES_PER_RADIAN =
      (180.0 / 3.141592653589793238463); ///< Degrees per radian for conversion
  static constexpr double GRAVITY_EARTH = 9.807; ///< Standard Earth Gravity

private:
  bool scanI2C(uint8_t addr);
  TwoWire *_i2c;

  Adafruit_ADXL343 *_adxl34x = NULL;
  Adafruit_BMP280 *_bmp280 = NULL;
  Adafruit_BME280 *_bme280 = NULL;
  Adafruit_DPS310 *_dps310 = NULL;
  Adafruit_LSM303_Accel_Unified *_lsm303a = NULL;
  Adafruit_LSM6DS33 *_lsm6ds33 = NULL;
  Adafruit_LSM6DSOX *_lsm6dsox = NULL;
  Adafruit_LSM9DS1 *_lsm9ds1 = NULL;
  Adafruit_LSM9DS0 *_lsm9ds0 = NULL;
  Adafruit_ICM20649 *_icm20649 = NULL;
  Adafruit_ISM330DHCT *_ism330dhct = NULL;
  Adafruit_LIS2MDL *_lis2mdl = NULL;
  Adafruit_LIS3MDL *_lis3mdl = NULL;
  Adafruit_FXOS8700 *_fxos8700 = NULL;
  Adafruit_FXAS21002C *_fxas21002 = NULL;
  Adafruit_MPU6050 *_mpu6050 = NULL;
  Adafruit_MSA301 *_msa301 = NULL;

  Adafruit_Sensor *pressure = NULL;
  Adafruit_Sensor *temperature = NULL;
  Adafruit_Sensor *humidity = NULL;
  Adafruit_Sensor *accelerometer = NULL;
  Adafruit_Sensor *gyroscope = NULL;
  Adafruit_Sensor *magnetometer = NULL;
};
#endif
