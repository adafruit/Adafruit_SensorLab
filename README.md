# Adafruit_SensorLab
Arduino library for scientific sensor readings/fusions/manipulations

**Not for use with small Arduino's like Atmega328/UNO** - this library
includes *all* sensors, requires a SAMD21/Zero/M4/ESP-size chip with
plenty of flash storage :)


## Supported environmental sensors:
  * AHT10 - Temperature & Humidity
  * AHT20 - Temperature & Humidity
  * BMP280 - Temperature & Pressure
  * BME280 - Temperature, Humidity & Pressure
  * DSP310 - Temperature & Pressure

## Supported motion sensors:
  * LSM9DS0 - 9-DoF Accelerometer, Gyro, and Magnetometer + Temperature
  * LSM6DS33 - 6-DoF Accelerometer & Gyro + Temperature
  * LSM6DSOX - 6-DoF Accelerometer & Gyro + Temperature
  * ICM20649 - 6-DoF Accelerometer & Gyro + Temperature
  * ISM330DHC - 6-DoF Accelerometer & Gyro + Temperature
  * FXOS8700 - 6-DoF Accelerometer & Magnetometer
  * MPU6050 - 6-DoF Accelerometer & Gyro
  * FXAS21001C - 3-Axis Gyroscope

## Supported magnetometers
  * LIS2MDL - 3-Axis Magnetometer (also in LSM303AGR)
  * LIS3MDL - 3-Axis Magnetometer (also in LSM9DS1)
  * MLX90393 - 3-Axis Magnetometer
  * HMC5883L - 3-Axis Magnetometer

## Supported accelerometers
  * ADXL343 & ADXL345 - 3-Axis Accelerometers
  * MSA301 - 3-Axis Accelerometer
  * LSM303 - 3-Axis Acecemeroteter in LSM303AGR and LSM3030DLHC
