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
#include <Adafruit_BMP280.h>
#include <Adafruit_BME280.h>
#include <Adafruit_ADXL343.h>
#include <Adafruit_LSM6DS33.h>

class Adafruit_SensorLab {
 public:
  Adafruit_SensorLab(TwoWire *theWire = &Wire);
  void begin(uint32_t I2C_Frequency = 100000);

  Adafruit_Sensor *getTemperatureSensor(void);
  Adafruit_Sensor *getPressureSensor(void);
  Adafruit_Sensor *getHumiditySensor(void);
  Adafruit_Sensor *getAccelerometer(void);

  float calculateAltitude(float currentPressure_hPa,
			  float originPressure_hPa=1013.25);
  bool detectBMP280(void);
  bool detectBME280(void);
  bool detectADXL34X(void);
  bool detectLSM6DS33(void);

 private:
  bool scanI2C(uint8_t addr);
  TwoWire *_i2c;

  Adafruit_BMP280 *_bmp280 = NULL;
  Adafruit_BME280 *_bme280 = NULL;
  Adafruit_ADXL343 *_adxl34x = NULL;
  Adafruit_LSM6DS33 *_lsm6ds33 = NULL;

  Adafruit_Sensor *pressure = NULL;
  Adafruit_Sensor *temperature = NULL;
  Adafruit_Sensor *humidity = NULL;
  Adafruit_Sensor *accelerometer = NULL;
  Adafruit_Sensor *gyroscope = NULL;
};
#endif
