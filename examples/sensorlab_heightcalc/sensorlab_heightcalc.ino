/***************************************************************************
  This is an example for the Adafruit SensorLab library
  It will look for a supported barometric pressure sensor and then
  use that to calculate altitude and delta-height from start

  Adafruit invests time and resources providing this open source code,
  please support Adafruit andopen-source hardware by purchasing products
  from Adafruit!

  Written by Limor Fried & Kevin Townsend for Adafruit Industries.
  BSD license, all text above must be included in any redistribution
 ***************************************************************************/

#include <Adafruit_SensorLab.h>
Adafruit_SensorLab lab;

Adafruit_Sensor *pressure;
sensors_event_t pressure_event;

// We'll take a reading to start
float initial_pressure;
// We can also calculate the altitude
float seaLevel_pressure = 1013.25;

void setup() {
  Serial.begin(115200);
  while (!Serial) delay(10);
  
  Serial.println(F("Sensor Lab - Barometric Pressure to Height!"));
  lab.begin();

  pressure = lab.getPressureSensor();
  if (! pressure) {
    Serial.println(F("Could not find a pressure sensor, check wiring!"));
    while(1) delay(10);
  }

  pressure->printSensorDetails();

  // Take an initial reading!
  pressure->getEvent(&pressure_event);
  initial_pressure = pressure_event.pressure;
}

void loop() {
  pressure->getEvent(&pressure_event);

  Serial.print(F("Pressure = "));
  Serial.print(pressure_event.pressure);
  Serial.println(" hPa");

  Serial.print(F("Change in pressure = "));
  Serial.print(pressure_event.pressure - initial_pressure);
  Serial.println(" hPa");

  float altitude = lab.calculateAltitude(pressure_event.pressure, seaLevel_pressure);
  float height = lab.calculateAltitude(pressure_event.pressure, initial_pressure);
  Serial.print(F("Altitude = "));
  Serial.print(altitude);
  Serial.print(F(" meters, Height = "));
  Serial.print(height);
  Serial.println(" meters");

  Serial.println();
  delay(1000);
}
