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

/**************************************************************************/
/*!
  @brief Mapper helper function for floating point values
  @param x Value to map
  @param in_min Input range minimum for x
  @param in_max Input range maximum for x
  @param out_min Ouput range maximum for return
  @param out_max Ouput range maximum for return
  @returns Mapped value
*/

/**************************************************************************/
float Adafruit_SensorLab::mapf(float x, float in_min, float in_max,
                               float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

/**************************************************************************/
/*!
  @brief Instantiate the SensorLab object
  @param i2c The I2C hardware interface, default is Wire
*/
/**************************************************************************/
Adafruit_SensorLab::Adafruit_SensorLab(TwoWire *i2c) { _i2c = i2c; }

/**************************************************************************/
/*!
  @brief Initialize the sensorlab manager and begin I2C
  @param I2C_Frequency desired I2C clock rate - default is 100KHz
*/
/**************************************************************************/
void Adafruit_SensorLab::begin(uint32_t I2C_Frequency) {
  _i2c->begin();
  _i2c->setClock(I2C_Frequency);
  yield();
}

/**************************************************************************/
/*!
    @brief  Detect if we have a valid ADXL34x sensor attached and sets
    the default accelerometer sensor if so
    @return True if found
*/
/**************************************************************************/
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

/**************************************************************************/
/*!
    @brief  Detect if we have a valid ADXL34x sensor attached and sets
    the default accelerometer sensor if so
    @return True if found
*/
/**************************************************************************/
bool Adafruit_SensorLab::detectLSM303A(void) {
  bool addr19 = scanI2C(0x19);

  if (!addr19) {
    return false; // no I2C device that could possibly work found!
  }

  _lsm303a = new Adafruit_LSM303_Accel_Unified(303);

  if (_lsm303a->begin()) {
    // yay found a LSM303 Accel
    Serial.println(F("Found a LSM303 accelerometer"));
    accelerometer = _lsm303a;
    return true;
  }

  delete _lsm303a;
  return false;
}

/**************************************************************************/
/*!
    @brief  Detect if we have a valid LSM6DS33 sensor attached and sets
    the default temperature, accelerometer and gyroscope sensors if so
    @return True if found
*/
/**************************************************************************/
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

/**************************************************************************/
/*!
    @brief  Detect if we have a valid LSM6DSOX sensor attached and sets
    the default temperature, accelerometer and gyroscope sensors if so
    @return True if found
*/
/**************************************************************************/
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

/**************************************************************************/
/*!
    @brief  Detect if we have a valid LSM9DS1 sensor attached and sets
    the default temperature, accelerometer and gyroscope sensors if so
    @return True if found
*/
/**************************************************************************/
bool Adafruit_SensorLab::detectLSM9DS1(void) {
  bool addr6B = scanI2C(0x6B);

  if (!addr6B) {
    return false; // no I2C device that could possibly work found!
  }

  _lsm9ds1 = new Adafruit_LSM9DS1(9051);

  if (_lsm9ds1->begin()) {
    // yay found a LSM9DS1
    Serial.println(F("Found a LSM9DS1 IMU"));

    if (!accelerometer)
      accelerometer = &_lsm9ds1->getAccel();
    if (!gyroscope)
      gyroscope = &_lsm9ds1->getGyro();
    if (!temperature) {
      temperature = &_lsm9ds1->getTemp();
    }
    return true;
  }

  delete _lsm9ds1;
  return false;
}

/**************************************************************************/
/*!
    @brief  Detect if we have a valid LSM9DS0 sensor attached and sets
    the default temperature, accelerometer, mag and gyroscope sensors if so
    @return True if found
*/
/**************************************************************************/
bool Adafruit_SensorLab::detectLSM9DS0(void) {
  bool addr6B = scanI2C(0x6B);
  bool addr1D = scanI2C(0x1D);

  if (!addr6B || !addr1D) {
    return false; // no I2C device that could possibly work found!
  }

  _lsm9ds0 = new Adafruit_LSM9DS0(9050);

  if (_lsm9ds0->begin()) {
    // yay found a LSM9DS0
    Serial.println(F("Found a LSM9DS0 IMU"));

    if (!accelerometer)
      accelerometer = &_lsm9ds0->getAccel();
    if (!gyroscope)
      gyroscope = &_lsm9ds0->getGyro();
    if (!magnetometer)
      magnetometer = &_lsm9ds0->getMag();
    if (!temperature) {
      temperature = &_lsm9ds0->getTemp();
    }
    return true;
  }

  delete _lsm9ds0;
  return false;
}

/**************************************************************************/
/*!
    @brief  Detect if we have a valid ICM20649 sensor attached and sets
    the default temperature, accelerometer and gyroscope sensors if so
    @return True if found
*/
/**************************************************************************/
bool Adafruit_SensorLab::detectICM20649(void) {
  bool addr68 = scanI2C(0x68);
  bool addr69 = scanI2C(0x69);

  if (!addr68 && !addr69) {
    return false; // no I2C device that could possibly work found!
  }

  _icm20649 = new Adafruit_ICM20649();

  if ((addr68 && _icm20649->begin_I2C(0x68)) ||
      (addr69 && _icm20649->begin_I2C(0x69))) {
    // yay found an ICM20649
    Serial.println(F("Found an ICM20649 IMU"));
    if (!accelerometer)
      accelerometer = _icm20649->getAccelerometerSensor();
    if (!gyroscope)
      gyroscope = _icm20649->getGyroSensor();
    if (!temperature) {
      temperature = _icm20649->getTemperatureSensor();
    }
    return true;
  }

  delete _icm20649;
  return false;
}

/**************************************************************************/
/*!
    @brief  Detect if we have a valid ISM330DHC sensor attached and sets
    the default temperature, accelerometer and gyroscope sensors if so
    @return True if found
*/
/**************************************************************************/
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

/**************************************************************************/
/*!
    @brief  Detect if we have a valid FXOS8700 sensor attached and sets
    the default accelerometer and magnetometer sensors if so
    @return True if found
*/
/**************************************************************************/
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

/**************************************************************************/
/*!
    @brief  Detect if we have a valid FXAS21002C sensor attached and sets
    the default gyroscope sensor if so
    @return True if found
*/
/**************************************************************************/
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

/**************************************************************************/
/*!
    @brief  Detect if we have a valid LIS3MDL sensor attached and sets
    the default magnetometer sensor if so
    @return True if found
*/
/**************************************************************************/
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

/**************************************************************************/
/*!
    @brief  Detect if we have a valid LIS2MDL sensor attached and sets
    the default magnetometer sensor if so
    @return True if found
*/
/**************************************************************************/
bool Adafruit_SensorLab::detectLIS2MDL(void) {
  bool addr1E = scanI2C(0x1E);

  if (!addr1E) {
    return false; // no I2C device that could possibly work found!
  }

  _lis2mdl = new Adafruit_LIS2MDL();

  if (_lis2mdl->begin()) {
    // yay found a LIS2MDL
    Serial.println(F("Found a LIS2MDL IMU"));

    if (!magnetometer)
      magnetometer = _lis2mdl;
    return true;
  }

  delete _lis2mdl;
  return false;
}

/**************************************************************************/
/*!
    @brief  Detect if we have a valid MPU6050 sensor attached and sets
    the default temperature, accelerometer and gyroscope sensors if so
    @return True if found
*/
/**************************************************************************/
bool Adafruit_SensorLab::detectMPU6050(void) {
  bool addr68 = scanI2C(0x68);
  bool addr69 = scanI2C(0x69);

  if (!addr68 && !addr69) {
    return false; // no I2C device that could possibly work found!
  }

  _mpu6050 = new Adafruit_MPU6050();

  if ((addr68 && _mpu6050->begin(0x68)) || (addr69 && _mpu6050->begin(0x69))) {
    // yay found an MPU6050
    Serial.println(F("Found an MPU6050 IMU"));
    if (!accelerometer)
      accelerometer = _mpu6050->getAccelerometerSensor();
    if (!gyroscope)
      gyroscope = _mpu6050->getGyroSensor();
    if (!temperature) {
      temperature = _mpu6050->getTemperatureSensor();
    }
    return true;
  }

  delete _mpu6050;
  return false;
}

/**************************************************************************/
/*!
    @brief  Detect if we have a valid MSA301 sensor attached and sets
    the default accelerometer sensor if so
    @return True if found
*/
/**************************************************************************/
bool Adafruit_SensorLab::detectMSA301(void) {
  bool addr26 = scanI2C(0x26);

  if (!addr26) {
    return false; // no I2C device that could possibly work found!
  }

  _msa301 = new Adafruit_MSA301();

  if ((addr26 && _msa301->begin(0x26))) {
    // yay found a MSA301
    Serial.println(F("Found a MSA301 accelerometer"));
    accelerometer = _msa301;
    return true;
  }

  delete _msa301;
  return false;
}

/**************************************************************************/
/*!
    @brief  Detect if we have a valid BMP280 sensor attached and sets
    the default pressure and temperature sensor if so
    @return True if found
*/
/**************************************************************************/
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

/**************************************************************************/
/*!
    @brief  Detect if we have a valid LPS2X sensor attached and sets
    the default pressure and temperature sensor if so
    @return True if found
*/
/**************************************************************************/
bool Adafruit_SensorLab::detectLPS2X(void) {
  bool addr5d = scanI2C(0x5D);
  bool addr5c = scanI2C(0x5C);

  if (!addr5d && !addr5c) {
    return false; // no I2C device that could possibly work found!
  }

  _lps2x = new Adafruit_LPS2X();

  if ((addr5d && _lps2x->begin_I2C(0x5D)) || (addr5c && _lps2x->begin_I2C(0x5C))) {
    // yay found a LPS2X
    Serial.println(F("Found a LPS2X Pressure sensor"));
    if (!pressure)
      pressure = _lps2x->getPressureSensor();
    if (!temperature)
      temperature = _lps2x->getTemperatureSensor();
    return true;
  }

  delete _lps2x;
  return false;
}

/**************************************************************************/
/*!
    @brief  Detect if we have a valid DPS310 sensor attached and sets
    the default pressure and temperature sensor if so
    @return True if found
*/
/**************************************************************************/
bool Adafruit_SensorLab::detectDPS310(void) {
  bool addr77 = scanI2C(0x77);
  bool addr76 = scanI2C(0x76);

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

/**************************************************************************/
/*!
    @brief  Detect if we have a valid BME280 sensor attached and sets
    the default humidity, pressure and temperature sensor if so
    @return True if found
*/
/**************************************************************************/
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

/**************************************************************************/
/*!
    @brief  Look for any known accelerometer-providing sensor on I2C
    @return A pointer to the Adafruit_Sensor device that can be queried
    for sensor events. NULL on failure to find any matching sensor.
*/
/**************************************************************************/
Adafruit_Sensor *Adafruit_SensorLab::getAccelerometer(void) {
  if (accelerometer) {
    return accelerometer; // we already did this process
  }
  if (detectADXL34X() || detectLSM6DS33() || detectLSM6DSOX() ||
      detectFXOS8700() || detectICM20649() || detectISM330DHCT() ||
      detectMPU6050() || detectMSA301() || detectLSM303A() || detectLSM9DS1() ||
      detectLSM9DS0()) {
    return accelerometer;
  }
  // Nothing detected
  return NULL;
}

/**************************************************************************/
/*!
    @brief  Look for any known magnetometer-providing sensor on I2C
    @return A pointer to the Adafruit_Sensor device that can be queried
    for sensor events. NULL on failure to find any matching sensor.
*/
/**************************************************************************/
Adafruit_Sensor *Adafruit_SensorLab::getMagnetometer(void) {
  if (magnetometer) {
    return magnetometer; // we already did this process
  }
  if (detectLIS3MDL() || detectLIS2MDL() || detectFXOS8700() ||
      detectLSM9DS0()) {
    return magnetometer;
  }
  // Nothing detected
  return NULL;
}

/**************************************************************************/
/*!
    @brief  Look for any known gyroscope-providing sensor on I2C
    @return A pointer to the Adafruit_Sensor device that can be queried
    for sensor events. NULL on failure to find any matching sensor.
*/
/**************************************************************************/
Adafruit_Sensor *Adafruit_SensorLab::getGyroscope(void) {
  if (gyroscope) {
    return gyroscope; // we already did this process
  }
  if (detectLSM6DS33() || detectLSM6DSOX() || detectICM20649() ||
      detectISM330DHCT() || detectFXAS21002() || detectMPU6050() ||
      detectLSM9DS1() || detectLSM9DS0()) {
    return gyroscope;
  }
  // Nothing detected
  return NULL;
}

/**************************************************************************/
/*!
    @brief  Look for any known barometric pressure-providing sensor on I2C
    @return A pointer to the Adafruit_Sensor device that can be queried
    for sensor events. NULL on failure to find any matching sensor.
*/
/**************************************************************************/
Adafruit_Sensor *Adafruit_SensorLab::getPressureSensor(void) {
  if (pressure) {
    return pressure; // we already did this process
  }
  if (detectBMP280() || detectBME280() || detectDPS310() || detectLPS2X()) {
    return pressure;
  }
  // Nothing detected
  return NULL;
}

/**************************************************************************/
/*!
    @brief  Look for any known temperature-providing sensor on I2C
    @return A pointer to the Adafruit_Sensor device that can be queried
    for sensor events. NULL on failure to find any matching sensor.
*/
/**************************************************************************/
Adafruit_Sensor *Adafruit_SensorLab::getTemperatureSensor(void) {
  if (temperature) {
    return temperature; // we already did this process
  }
  if (detectBMP280() || detectBME280() || detectDPS310() || detectLPS2X()) {
    return temperature;
  }
  // Nothing detected
  return NULL;
}

/**************************************************************************/
/*!
    @brief  Look for any known humidity-providing sensor on I2C
    @return A pointer to the Adafruit_Sensor device that can be queried
    for sensor events. NULL on failure to find any matching sensor.
*/
/**************************************************************************/
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
    // Serial.print("Found addr 0x");
    // Serial.println(addr, HEX);
  }
  return f;
}
