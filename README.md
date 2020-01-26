# Adafruit_SensorLab
Arduino library for scientific sensor readings/fusions/manipulations

**Not for use with small Arduino's like Atmega328/UNO** - this library
includes *all* sensors, requires a SAMD21/Zero/M4/ESP-size chip with
plenty of flash storage :)


## Supported environmental sensors:
  * BMP280 - Temperature & Pressure
  * BME280 - Temperature, Humidity & Pressure
  * DSP310 - Temperature & Pressure

## Supported motion sensors:
  * LSM6DS33 - 6-DoF Accelerometer & Gyro + Temperature
  * LSM6DSOX - 6-DoF Accelerometer & Gyro + Temperature
  * ICM20649 - 6-DoF Accelerometer & Gyro + Temperature
  * ISM330DHC - 6-DoF Accelerometer & Gyro + Temperature
  * FXOS8700 - 6-DoF Accelerometer & Magnetometer
  * MPU6050 - 6-DoF Accelerometer & Gyro
  * FXAS21001C - 3-Axis Gyroscope
  * LIS2MDL - 3-Axis Magnetometer (also in LSM303AGR)
  * LIS3MDL - 3-Axis Magnetometer (also in LSM9DS1)
  * ADXL343 & ADXL345 - 3-Axis Accelerometers
  * MSA301 - 3-Axis Accelerometer
