/***************************************************************************
  This is an example for the Adafruit SensorLab library
  It will look for a supported accelerometer and then
  use that to calculate how level the board/sensor is

  Adafruit invests time and resources providing this open source code,
  please support Adafruit andopen-source hardware by purchasing products
  from Adafruit!

  Written by Limor Fried for Adafruit Industries.
  BSD license, all text above must be included in any redistribution
 ***************************************************************************/

#include <Adafruit_SensorLab.h>
Adafruit_SensorLab lab;

Adafruit_Sensor *accel;
sensors_event_t accel_event;

#define SPIRITGRID_SIZE 17 // make this odd so we can have something in the center!
bool spiritgrid[SPIRITGRID_SIZE][SPIRITGRID_SIZE];

#define Y_LEVEL_TOP    10
#define Y_LEVEL_BOTTOM -10

#define X_LEVEL_LEFT   10
#define X_LEVEL_RIGHT  -10

void setup() {
  Serial.begin(115200);
  while (!Serial) delay(10);
  
  Serial.println(F("Sensor Lab - Accelerometer Inclinometer!"));
  lab.begin();

  Serial.println("Looking for an accelerometer");
  accel = lab.getAccelerometer();
  if (! accel) {
    Serial.println(F("Could not find a accelerometer, check wiring!"));
    while(1) delay(10);
  }

  accel->printSensorDetails();
}

void loop() {
  accel->getEvent(&accel_event);

  /* Display the results (acceleration is measured in m/s^2) */
  Serial.print("Accel X: ");
  Serial.print(accel_event.acceleration.x);
  Serial.print(" \t, \tY: ");
  Serial.print(accel_event.acceleration.y);
  Serial.print(" \t, \tZ: ");
  Serial.print(accel_event.acceleration.z);
  Serial.println(" m/s^2 ");

  // Read the X component of the accel and convert to 'g's
  double accel_xg = accel_event.acceleration.x / Adafruit_SensorLab::GRAVITY_EARTH;
  // don't let the accelerometer go beyond just below 1 g
  accel_xg = min(accel_xg, 0.9999999);
  accel_xg = max(accel_xg, -0.9999999);

  // Read the Y component of the accel and convert to 'g's
  double accel_yg = accel_event.acceleration.y / Adafruit_SensorLab::GRAVITY_EARTH;
  accel_yg = min(accel_yg, 0.9999999);
  accel_yg = max(accel_yg, -0.9999999);

  // Take inverse sine and cosine & convert both to degrees
  double inclination_x = asin(accel_xg) * Adafruit_SensorLab::DEGREES_PER_RADIAN;
  double inclination_y = acos(accel_yg) * Adafruit_SensorLab::DEGREES_PER_RADIAN;

  Serial.print("X & Y inclinations (degrees): "); 
  Serial.print(inclination_x); Serial.print(", ");
  Serial.println(inclination_y);
  delay(100);
}
