/**************************************************************************/
/*!
    @file     Adafruit_SensorLab.cpp
    @author   Limor Fried (Adafruit Industries)

    Arduino library for scientific sensor readings/fusions/manipulations

    Adafruit invests time and resources providing this open source code,
    please support Adafruit and open-source hardware by purchasing
    products from Adafruit!

*/
/**************************************************************************/

#include <Adafruit_SensorLab.h>

float Adafruit_SensorLab::mapf(float x, float in_min, float in_max,
                               float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

Adafruit_SensorLab::Adafruit_SensorLab(TwoWire *i2c) { _i2c = i2c; }

void Adafruit_SensorLab::begin(uint32_t I2C_Frequency) {
  _i2c->begin();
  _i2c->setClock(I2C_Frequency);
  yield();
}

bool Adafruit_SensorLab::detectADXL34X(void) {
  bool addr1D = scanI2C(0x1D);
  bool addr53 = scanI2C(0x53);

  if (!addr1D && !addr53) {
    return false; // no I2C device that could possibly work found!
  }

  _adxl34x = new Adafruit_ADXL343(343);

  if ((addr1D && _adxl34x->begin(0x1D)) || (addr53 && _adxl34x->begin(0x53))) {
    // yay found a ADXL34x
    Serial.println(F("Found a ADXL34x accelerometer"));
    accelerometer = _adxl34x;
    return true;
  }

  delete _adxl34x;
  return false;
}

bool Adafruit_SensorLab::detectLSM6DS33(void) {
  bool addr6A = scanI2C(0x6A);
  bool addr6B = scanI2C(0x6B);

  if (!addr6A && !addr6B) {
    return false; // no I2C device that could possibly work found!
  }

  _lsm6ds33 = new Adafruit_LSM6DS33();

  if ((addr6A && _lsm6ds33->begin_I2C(0x6A)) ||
      (addr6B && _lsm6ds33->begin_I2C(0x6B))) {
    // yay found a LSM6DS33
    Serial.println(F("Found a LSM6DS33 IMU"));

    if (!accelerometer)
      accelerometer = _lsm6ds33->getAccelerometerSensor();
    if (!gyroscope)
      gyroscope = _lsm6ds33->getGyroSensor();
    if (!temperature) {
      temperature = _lsm6ds33->getTemperatureSensor();
    }
    return true;
  }

  delete _lsm6ds33;
  return false;
}

bool Adafruit_SensorLab::detectLSM6DSOX(void) {
  bool addr6A = scanI2C(0x6A);
  bool addr6B = scanI2C(0x6B);

  if (!addr6A && !addr6B) {
    return false; // no I2C device that could possibly work found!
  }

  _lsm6dsox = new Adafruit_LSM6DSOX();

  if ((addr6A && _lsm6dsox->begin_I2C(0x6A)) ||
      (addr6B && _lsm6dsox->begin_I2C(0x6B))) {
    // yay found a LSM6DSOX
    Serial.println(F("Found a LSM6DSOX IMU"));

    if (!accelerometer)
      accelerometer = _lsm6dsox->getAccelerometerSensor();
    if (!gyroscope)
      gyroscope = _lsm6dsox->getGyroSensor();
    if (!temperature) {
      temperature = _lsm6dsox->getTemperatureSensor();
    }
    return true;
  }

  delete _lsm6dsox;
  return false;
}

bool Adafruit_SensorLab::detectISM330DHCT(void) {
  bool addr6A = scanI2C(0x6A);
  bool addr6B = scanI2C(0x6B);

  if (!addr6A && !addr6B) {
    return false; // no I2C device that could possibly work found!
  }

  _ism330dhct = new Adafruit_ISM330DHCT();

  if ((addr6A && _ism330dhct->begin_I2C(0x6A)) ||
      (addr6B && _ism330dhct->begin_I2C(0x6B))) {
    // yay found a ISM330
    Serial.println(F("Found a ISM330DHCT IMU"));

    if (!accelerometer)
      accelerometer = _ism330dhct->getAccelerometerSensor();
    if (!gyroscope)
      gyroscope = _ism330dhct->getGyroSensor();
    if (!temperature) {
      temperature = _ism330dhct->getTemperatureSensor();
    }
    return true;
  }

  delete _ism330dhct;
  return false;
}

bool Adafruit_SensorLab::detectFXOS8700(void) {
  bool addr1C = scanI2C(0x1C);
  bool addr1D = scanI2C(0x1D);
  bool addr1E = scanI2C(0x1E);
  bool addr1F = scanI2C(0x1F);

  if (!addr1C && !addr1D && !addr1E && !addr1F) {
    return false; // no I2C device that could possibly work found!
  }

  _fxos8700 = new Adafruit_FXOS8700(0x8700A, 0x8700B);

  if ((addr1C && _fxos8700->begin()) || (addr1D && _fxos8700->begin()) ||
      (addr1E && _fxos8700->begin()) || (addr1F && _fxos8700->begin())) {
    // yay found a FXOS8700
    Serial.println(F("Found a FXOS8700 eCompass"));

    if (!accelerometer)
      accelerometer = _fxos8700->getAccelerometerSensor();
    if (!magnetometer)
      magnetometer = _fxos8700->getMagnetometerSensor();
    return true;
  }

  delete _fxos8700;
  return false;
}

bool Adafruit_SensorLab::detectFXAS21002(void) {
  bool addr20 = scanI2C(0x20);
  bool addr21 = scanI2C(0x21);

  if (!addr20 && !addr21) {
    return false; // no I2C device that could possibly work found!
  }

  _fxas21002 = new Adafruit_FXAS21002C(21002);

  if ((addr20 && _fxas21002->begin()) || (addr21 && _fxas21002->begin())) {
    // yay found a FXAS21002C
    Serial.println(F("Found a FXAS21002C gyro"));

    if (!gyroscope)
      gyroscope = _fxas21002;
    return true;
  }

  delete _fxas21002;
  return false;
}

bool Adafruit_SensorLab::detectLIS3MDL(void) {
  bool addr1E = scanI2C(0x1E);
  bool addr1C = scanI2C(0x1C);

  if (!addr1C && !addr1E) {
    return false; // no I2C device that could possibly work found!
  }

  _lis3mdl = new Adafruit_LIS3MDL();

  if ((addr1E && _lis3mdl->begin_I2C(0x1E)) ||
      (addr1C && _lis3mdl->begin_I2C(0x1C))) {
    // yay found a LIS3MDL
    Serial.println(F("Found a LIS3MDL IMU"));

    if (!magnetometer)
      magnetometer = _lis3mdl;
    return true;
  }

  delete _lis3mdl;
  return false;
}

bool Adafruit_SensorLab::detectBMP280(void) {
  bool addr77 = scanI2C(0x77);
  bool addr76 = scanI2C(0x76);

  if (!addr77 && !addr76) {
    return false; // no I2C device that could possibly work found!
  }

  _bmp280 = new Adafruit_BMP280();

  if ((addr77 && _bmp280->begin(0x77)) || (addr76 && _bmp280->begin(0x76))) {
    // yay found a BMP280
    Serial.println(F("Found a BMP280 Pressure sensor"));
    if (!pressure)
      pressure = _bmp280->getPressureSensor();
    if (!temperature)
      temperature = _bmp280->getTemperatureSensor();
    return true;
  }

  delete _bmp280;
  return false;
}

bool Adafruit_SensorLab::detectDPS310(void) {
  bool addr77 = scanI2C(0x77);
  bool addr76 = scanI2C(0x76);

  Serial.println("Looking for DPS310");

  if (!addr77 && !addr76) {
    return false; // no I2C device that could possibly work found!
  }

  Adafruit_DPS310 *_dps310 = new Adafruit_DPS310();
  if ((addr77 && _dps310->begin_I2C(0x77)) ||
      (addr76 && _dps310->begin_I2C(0x76))) {
    // yay found a DPS310
    Serial.println(F("Found a DPS310 Pressure sensor"));
    if (!pressure)
      pressure = _dps310->getPressureSensor();
    if (!temperature)
      temperature = _dps310->getTemperatureSensor();
    return true;
  }

  delete _dps310;
  return false;
}

bool Adafruit_SensorLab::detectBME280(void) {
  bool addr77 = scanI2C(0x77);
  bool addr76 = scanI2C(0x76);
  Serial.println("Looking for BME280");

  if (!addr77 && !addr76) {
    return false; // no I2C device that could possibly work found!
  }

  _bme280 = new Adafruit_BME280();

  if ((addr77 && _bme280->begin()) || (addr76 && _bme280->begin(0x76))) {
    // yay found a BME280
    Serial.println(F("Found a BME280 Pressure+Humidity sensor"));
    if (!pressure)
      pressure = _bme280->getPressureSensor();
    if (!humidity)
      humidity = _bme280->getHumiditySensor();
    if (!temperature)
      temperature = _bme280->getTemperatureSensor();
    return true;
  }

  delete _bme280;
  return false;
}

Adafruit_Sensor *Adafruit_SensorLab::getAccelerometer(void) {
  if (accelerometer) {
    return accelerometer; // we already did this process
  }
  if (detectADXL34X() || detectLSM6DS33() || detectLSM6DSOX() ||
      detectFXOS8700() || detectISM330DHCT()) {
    return accelerometer;
  }
  // Nothing detected
  return NULL;
}

Adafruit_Sensor *Adafruit_SensorLab::getMagnetometer(void) {
  if (magnetometer) {
    return magnetometer; // we already did this process
  }
  if (detectLIS3MDL() || detectFXOS8700()) {
    return magnetometer;
  }
  // Nothing detected
  return NULL;
}

Adafruit_Sensor *Adafruit_SensorLab::getGyroscope(void) {
  if (gyroscope) {
    return gyroscope; // we already did this process
  }
  if (detectLSM6DS33() || detectLSM6DSOX() || detectISM330DHCT() ||
      detectFXAS21002()) {
    return gyroscope;
  }
  // Nothing detected
  return NULL;
}

Adafruit_Sensor *Adafruit_SensorLab::getPressureSensor(void) {
  if (pressure) {
    return pressure; // we already did this process
  }
  if (detectBMP280() || detectBME280() || detectDPS310()) {
    return pressure;
  }
  // Nothing detected
  return NULL;
}

Adafruit_Sensor *Adafruit_SensorLab::getTemperatureSensor(void) {
  if (temperature) {
    return temperature; // we already did this process
  }
  if (detectBMP280() || detectBME280() || detectDPS310()) {
    return temperature;
  }
  // Nothing detected
  return NULL;
}

Adafruit_Sensor *Adafruit_SensorLab::getHumiditySensor(void) {
  if (humidity) {
    return humidity; // we already did this process
  }
  if (detectBME280()) {
    return humidity;
  }
  // Nothing detected
  return NULL;
}

/*!
 * @brief Calculates the approximate altitude using barometric pressure and the
 * supplied 'zero' point (e.g. sea level) hPa as a reference.
 * @param currentPressure_hPa The pressure at your current location
 * @param originPressure_hPa The pressure at your 'zero' point
 * @return The approximate altitude in meters.
 */
float Adafruit_SensorLab::calculateAltitude(float currentPressure_hPa,
                                            float originPressure_hPa) {
  return 44330 * (1.0 - pow(currentPressure_hPa / originPressure_hPa, 0.1903));
}

bool Adafruit_SensorLab::scanI2C(uint8_t addr) {
  yield();

  // A basic scanner, see if it ACK's
  _i2c->beginTransmission(addr);
  bool f = (_i2c->endTransmission() == 0);
  if (f) {
    Serial.print("Found addr 0x");
    Serial.println(addr, HEX);
  }
  return f;
}
