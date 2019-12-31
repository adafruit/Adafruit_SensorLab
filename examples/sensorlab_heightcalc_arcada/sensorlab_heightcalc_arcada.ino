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
#include <Adafruit_Arcada.h>
Adafruit_Arcada arcada;
Adafruit_SensorLab lab;

Adafruit_Sensor *pressure;
sensors_event_t pressure_event;

// We'll take a reading to start
float initial_pressure;
// We can also calculate the altitude
float seaLevel_pressure = 1013.25;

void setup() {
  Serial.begin(115200);
  //while (!Serial) delay(10);

  // Start TFT and fill black
  if (!arcada.arcadaBegin()) {
    Serial.print("Failed to begin");
    while (1);
  }
  arcada.displayBegin();
  arcada.setBacklight(255);
  arcada.display->fillScreen(ARCADA_BLACK);
  arcada.display->setTextWrap(false);
  arcada.display->setTextSize(2);
  
  Serial.println("Sensor Lab - Barometric Pressure to Height!");
  arcada.display->setTextColor(ARCADA_BLUE);
  arcada.display->println("- Sensor Lab -");
  arcada.display->println("Pressure to Height!");
  lab.begin();

  Serial.println("Looking for a pressure sensor");
  pressure = lab.getPressureSensor();
  if (! pressure) {
    Serial.println("Could not find a pressure sensor, check wiring!");
    arcada.haltBox("Could not find a pressure sensor, check wiring!");
  }

  // Print out some details on what we got
  pressure->printSensorDetails();
  sensor_t sensor;
  pressure->getSensor(&sensor);
  arcada.display->setTextColor(ARCADA_GREEN);
  arcada.display->printf("Found: %s\n", sensor.name);
  arcada.display->printf("Range: %0.0f~%0.0f hPa\n", sensor.min_value, sensor.max_value);
  arcada.display->setTextColor(ARCADA_WHITE);
  arcada.display->println("Press (A) to reset\ninitial reading");
  
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

  int halfway_down = arcada.display->height()/2;
  // clear screen
  arcada.display->fillRect(0, halfway_down, arcada.display->width(), halfway_down, ARCADA_BLACK);
  // place cursor
  arcada.display->setCursor(0, halfway_down);
  arcada.display->setTextColor(ARCADA_WHITE);
  // Display pressure and height calculation
  arcada.display->printf("Initial: %0.2f hPa\n", initial_pressure);
  arcada.display->printf("Current: %0.2f hPa\n\n", pressure_event.pressure);
  arcada.display->printf("Height: %0.1f m\n\n", height);
  arcada.display->printf("Est. Alt.: %0.1f m\n", altitude);

  // If the A button is pressed, reset initial pressure reading
  uint8_t buttons = arcada.readButtons();
  if (buttons & ARCADA_BUTTONMASK_A) {
    initial_pressure = pressure_event.pressure;
  }
  delay(1000);
}
