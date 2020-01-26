/***************************************************************************
  This is an example for the Adafruit SensorLab library
  It will look for a supported magnetometer and output
  PJRC Motion Sensor Calibration Tool-compatible serial data

  PJRC & Adafruit invest time and resources providing this open source code,
  please support PJRC and open-source hardware by purchasing products
  from PJRC!

  Written by PJRC, adapted by Limor Fried for Adafruit Industries.
 ***************************************************************************/

#include <Adafruit_SensorLab.h>
Adafruit_SensorLab lab;

Adafruit_Sensor *mag = NULL, *gyro = NULL, *accel = NULL;
sensors_event_t mag_event, gyro_event, accel_event;

void setup(void) {
  Serial.begin(115200);
  while (!Serial) delay(10);     // will pause Zero, Leonardo, etc until serial console opens
  
  Serial.println(F("Sensor Lab - IMU Calibration!"));
  lab.begin();
  
  Serial.println("Looking for a magnetometer");
  mag = lab.getMagnetometer();
  if (! mag) {
    Serial.println(F("Could not find a magnetometer, skipping!"));
  } else {
    mag->printSensorDetails();
  }
  
  Serial.println("Looking for a gyroscope");
  gyro = lab.getGyroscope();
  if (! gyro) {
    Serial.println(F("Could not find a gyroscope, skipping!"));
  } else {
    gyro->printSensorDetails();
  }
  
  Serial.println("Looking for a accelerometer");
  accel = lab.getAccelerometer();
  if (! accel) {
    Serial.println(F("Could not find a accelerometer, skipping!"));
  } else {
    accel->printSensorDetails();
  }
}

void loop() {
  if (mag && ! mag->getEvent(&mag_event)) {
    return;
  }
  if (gyro && ! gyro->getEvent(&gyro_event)) {
    return;
  }
  if (accel && ! accel->getEvent(&accel_event)) {
    return;
  }
  // 'Raw' values to match expectation of MOtionCal
  Serial.print("Raw:");
  Serial.print(int(accel_event.acceleration.x*8192/9.8)); Serial.print(",");
  Serial.print(int(accel_event.acceleration.y*8192/9.8)); Serial.print(",");
  Serial.print(int(accel_event.acceleration.z*8192/9.8)); Serial.print(",");
  Serial.print(int(gyro_event.gyro.x*Adafruit_SensorLab::DEGREES_PER_RADIAN*16)); Serial.print(",");
  Serial.print(int(gyro_event.gyro.y*Adafruit_SensorLab::DEGREES_PER_RADIAN*16)); Serial.print(",");
  Serial.print(int(gyro_event.gyro.z*Adafruit_SensorLab::DEGREES_PER_RADIAN*16)); Serial.print(",");
  Serial.print(int(mag_event.magnetic.x*10)); Serial.print(",");
  Serial.print(int(mag_event.magnetic.y*10)); Serial.print(",");
  Serial.print(int(mag_event.magnetic.z*10)); Serial.println("");

  // unified data
  Serial.print("Uni:");
  Serial.print(accel_event.acceleration.x); Serial.print(",");
  Serial.print(accel_event.acceleration.y); Serial.print(",");
  Serial.print(accel_event.acceleration.z); Serial.print(",");
  Serial.print(gyro_event.gyro.x, 4); Serial.print(",");
  Serial.print(gyro_event.gyro.y, 4); Serial.print(",");
  Serial.print(gyro_event.gyro.z, 4); Serial.print(",");
  Serial.print(mag_event.magnetic.x); Serial.print(",");
  Serial.print(mag_event.magnetic.y); Serial.print(",");
  Serial.print(mag_event.magnetic.z); Serial.println("");
  delay(10); 
}

/********************************************************/
