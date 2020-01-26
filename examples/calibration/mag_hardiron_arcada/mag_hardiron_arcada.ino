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
#include <Adafruit_Arcada.h>
Adafruit_Arcada arcada;
Adafruit_SensorLab lab;

Adafruit_Sensor *accel, *mag;
sensors_event_t accel_event;

uint16_t display_center_x, display_center_y, mag_circle_radius;
float min_x = 1000, max_x = -1000, mid_x = 0, x_hardoffset = 0;
float min_y = 1000, max_y = -1000, mid_y = 0, y_hardoffset = 0;
float min_z = 1000, max_z = -1000, mid_z = 0, z_hardoffset = 0;

void setup() {
  Serial.begin(115200);
  //while (!Serial) delay(10);

  // Start TFT and fill black
  if (!arcada.arcadaBegin()) {
    Serial.print("Failed to begin");
    while (1) yield();
  }

  // If we are using TinyUSB & QSPI we will have the filesystem show up!
  arcada.filesysBeginMSD();
  
  arcada.displayBegin();
  arcada.setBacklight(255);
  arcada.display->fillScreen(ARCADA_BLACK);

  if (arcada.filesysBegin()) {
    Serial.println("Found filesystem!");
    if (! arcada.loadConfigurationFile()) {
      arcada.infoBox("No configuration file found, creating!");
      if (! arcada.saveConfigurationFile()) {
        arcada.haltBox("Unable to create");
      }
    }
  } else {
    arcada.warnBox("No filesystem found! For QSPI flash, load CircuitPython. For SD cards, format with FAT");
  }
  
  arcada.display->fillScreen(ARCADA_BLACK);
  arcada.display->setCursor(0, 0);
  arcada.display->setTextWrap(false);
  arcada.display->setTextSize(2);

  Serial.println("Sensor Lab - Magnetometer Calibration!");
  arcada.display->setTextColor(ARCADA_BLUE);
  arcada.display->println("- Sensor Lab -");
  arcada.display->println("Magnetometer\nCalibration!");
  lab.begin();

  accel = lab.getAccelerometer();

  Serial.println("Looking for an magnetometer");
  mag = lab.getMagnetometer();
  if (! mag) {
    Serial.println("Could not find a magnetometer, check wiring!");
    arcada.haltBox("Could not find a magnetometer, check wiring!");
  }
  
  // Print out some details on what we got
  mag->printSensorDetails();
  sensor_t sensor;
  mag->getSensor(&sensor);
  arcada.display->setTextColor(ARCADA_GREEN);
  arcada.display->printf("Found: %s\n", sensor.name);
  arcada.display->printf("Range: %0.0f~%0.0f uT\n", sensor.min_value, sensor.max_value);
  arcada.display->setTextColor(ARCADA_WHITE);
  arcada.display->println("Press (A) to plot\nmagnetometer data");
  arcada.display->println("After you get three\ncircles, press (A)");
  arcada.display->println("to calibrate and\nsave the\ncalibration on disk");

  do {
    delay(10);
    arcada.readButtons();
  } while (! arcada.justReleasedButtons() & ARCADA_BUTTONMASK_A);
  
  arcada.display->fillScreen(ARCADA_BLACK);  
  display_center_x = arcada.display->width()/2;
  display_center_y = arcada.display->height()/2;
  mag_circle_radius = min(display_center_x, display_center_y);

  x_hardoffset = arcada.configJSON["magXoff"];
  y_hardoffset = arcada.configJSON["magYoff"];
  z_hardoffset = arcada.configJSON["magZoff"];
}

void loop() {
  sensors_event_t event; 
  int16_t dot_xx, dot_xy, dot_yx, dot_yy, dot_zy, dot_zx;

  mag->getEvent(&event);
  float x = event.magnetic.x;
  float y = event.magnetic.y;
  float z = event.magnetic.z;

  min_x = min(min_x, x);
  min_y = min(min_y, y);
  min_z = min(min_z, z);
  max_x = max(max_x, x);
  max_y = max(max_y, y);
  max_z = max(max_z, z);
  mid_x = (max_x + min_x) / 2;
  mid_y = (max_y + min_y) / 2;
  mid_z = (max_z + min_z) / 2;
  Serial.printf("Min (%0.2f, %0.2f, %0.2f)\n", min_x, min_y, min_z);
  Serial.printf("Max (%0.2f, %0.2f, %0.2f)\n", max_x, max_y, max_z);
  Serial.printf("Mid-point (%0.2f, %0.2f, %0.2f)\n", mid_x, mid_y, mid_z);
  
  Serial.printf("Uncal. Mag: (%0.2f, %0.2f, %0.2f)\n", x, y, z);
  x -= x_hardoffset;
  y -= y_hardoffset;
  z -= z_hardoffset;
  Serial.printf("Calibrated Mag: (%0.2f, %0.2f, %0.2f)\n", x, y, z);

  dot_xx = Adafruit_SensorLab::mapf(x, -50, 50, 
                                   display_center_x - mag_circle_radius, 
                                   display_center_x + mag_circle_radius);
  dot_xy = Adafruit_SensorLab::mapf(x, -50, 50, 
                                   display_center_y - mag_circle_radius, 
                                   display_center_y + mag_circle_radius);
  dot_yy = Adafruit_SensorLab::mapf(y, -50, 50, 
                                   display_center_y - mag_circle_radius, 
                                   display_center_y + mag_circle_radius);
  dot_yx = Adafruit_SensorLab::mapf(y, -50, 50, 
                                   display_center_x - mag_circle_radius, 
                                   display_center_x + mag_circle_radius);
  dot_zx = Adafruit_SensorLab::mapf(z, -50, 50, 
                                   display_center_x - mag_circle_radius, 
                                   display_center_x + mag_circle_radius);
  dot_zy = Adafruit_SensorLab::mapf(z, -50, 50, 
                                   display_center_y - mag_circle_radius, 
                                   display_center_y + mag_circle_radius);
  //Serial.printf("Point (%d, %d, %d)\n", dot_xx, dot_yy, dot_zy);
  arcada.display->drawPixel(dot_xx, dot_yy, ARCADA_RED);  
  arcada.display->drawPixel(dot_yx, dot_zy, ARCADA_GREEN);  
  arcada.display->drawPixel(dot_zx, dot_xy, ARCADA_BLUE);

  arcada.readButtons();
  if (arcada.justReleasedButtons() & ARCADA_BUTTONMASK_A) {
    x_hardoffset = mid_x;
    y_hardoffset = mid_y;
    z_hardoffset = mid_z;

    arcada.configJSON["magXoff"] = mid_x;
    arcada.configJSON["magYoff"] = mid_y;
    arcada.configJSON["magZoff"] = mid_z;
    if (! arcada.saveConfigurationFile()) {
      arcada.haltBox("Couldn't save config file!");
    }
    arcada.infoBox("Saved calibration");
    arcada.display->fillScreen(ARCADA_BLACK);
  }
  delay(10);
  Serial.println();
}
