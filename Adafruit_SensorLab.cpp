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
#include <Adafruit_BMP280.h>


Adafruit_SensorLab::Adafruit_SensorLab(TwoWire *i2c) {
  _i2c = i2c;
}

void Adafruit_SensorLab::begin(uint32_t I2C_Frequency) {
  _i2c->begin();
  _i2c->setClock(I2C_Frequency);
}

bool Adafruit_SensorLab::detectBMP280(void) {
  bool addr77 = scanI2C(0x77);
  bool addr76 = scanI2C(0x76);
  
  if (!addr77 && !addr76) {
    return false; // no I2C device that could possibly work found!
  }

  _bmp280 = new Adafruit_BMP280();
    
  if ((addr77 && _bmp280->begin(0x77)) || 
      (addr76 && _bmp280->begin(0x76))) {
    // yay found a BMP280
    Serial.println(F("Found a BMP280 Pressure sensor"));
    pressure = _bmp280->getPressureSensor();
    temperature = _bmp280->getTemperatureSensor();
    return true;
  }
  
  delete _bmp280;
  return false;
}

bool Adafruit_SensorLab::detectBME280(void) {
  bool addr77 = scanI2C(0x77);
  bool addr76 = scanI2C(0x76);
  
  if (!addr77 && !addr76) {
    return false; // no I2C device that could possibly work found!
  }

  _bme280 = new Adafruit_BME280();
    
  if ((addr77 && _bme280->begin(0x77)) || 
      (addr76 && _bme280->begin(0x76))) {
    // yay found a BME280
    Serial.println(F("Found a BME280 Pressure+Humidity sensor"));
    pressure = _bme280->getPressureSensor();
    humidity = _bme280->getHumiditySensor();
    temperature = _bme280->getTemperatureSensor();
    return true;
  }
  
  delete _bme280;
  return false;
}

Adafruit_Sensor *Adafruit_SensorLab::getPressureSensor(void) {
  if (pressure) {
    return pressure; // we already did this process
  }
  if (detectBMP280() || detectBME280()) {
    return pressure;
  }
  // Nothing detected
  return NULL;
}

Adafruit_Sensor *Adafruit_SensorLab::getTemperatureSensor(void) {
  if (temperature) {
    return temperature; // we already did this process
  }
  if (detectBMP280() || detectBME280()) {
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
					    float originPressure_hPa ) {
  return 44330 * (1.0 - pow(currentPressure_hPa/originPressure_hPa, 0.1903));
}

bool Adafruit_SensorLab::scanI2C(uint8_t addr) {
  // A basic scanner, see if it ACK's
  _i2c->beginTransmission(addr);
  return (_i2c->endTransmission() == 0);
}
