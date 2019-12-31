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
  
  Serial.println(F("Sensor Lab - Accelerometer Spirit Level!"));
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
  
  // calculate X/y offset
  int x_level = map(accel_event.acceleration.x, X_LEVEL_LEFT, X_LEVEL_RIGHT, 0, SPIRITGRID_SIZE);
  int y_level = map(accel_event.acceleration.y, Y_LEVEL_TOP, Y_LEVEL_BOTTOM, 0, SPIRITGRID_SIZE);
    
  // Top 'bar'
  for (int i=0; i<SPIRITGRID_SIZE+1; i++)
     Serial.print("--");
  Serial.println();
  
  for (int y=0; y<SPIRITGRID_SIZE; y++) {
    Serial.print('|');  // Left side
    for (int x=0; x<SPIRITGRID_SIZE; x++) {
      if ((x == x_level) && (y == y_level)) {
       Serial.print("<>");        
      } else {
        Serial.print("  ");
      }
    }
    Serial.println('|');  // Right side
  }

  // Bottom 'bar'
  for (int i=0; i<SPIRITGRID_SIZE+1; i++)
     Serial.print("--");
  Serial.println();
  
  delay(100);
}
