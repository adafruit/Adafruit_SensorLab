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

#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL343.h>
#include <Adafruit_BMP280.h>
#include <Adafruit_BME280.h>
#include <Adafruit_DPS310.h>
#include <Adafruit_LSM6DS33.h>
#include <Adafruit_LIS3MDL.h>

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
			  float originPressure_hPa=1013.25);
  bool detectADXL34X(void);
  bool detectBMP280(void);
  bool detectBME280(void);
  bool detectDPS310(void);
  bool detectLSM6DS33(void);
  bool detectLIS3MDL(void);

  static float mapf(float x, float in_min, float in_max, float out_min, float out_max);
  static constexpr double DEGREES_PER_RADIAN = (180.0/3.141592653589793238463);
  static constexpr double GRAVITY_EARTH = 9.807;

 private:
  bool scanI2C(uint8_t addr);
  TwoWire *_i2c;

  Adafruit_ADXL343 *_adxl34x = NULL;
  Adafruit_BMP280 *_bmp280 = NULL;
  Adafruit_BME280 *_bme280 = NULL;
  Adafruit_DPS310 *_dps310 = NULL;
  Adafruit_LSM6DS33 *_lsm6ds33 = NULL;
  Adafruit_LIS3MDL *_lis3mdl = NULL;

  Adafruit_Sensor *pressure = NULL;
  Adafruit_Sensor *temperature = NULL;
  Adafruit_Sensor *humidity = NULL;
  Adafruit_Sensor *accelerometer = NULL;
  Adafruit_Sensor *gyroscope = NULL;
  Adafruit_Sensor *magnetometer = NULL;
};
#endif
