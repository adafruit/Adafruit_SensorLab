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

Adafruit_Sensor *accel;
sensors_event_t accel_event;

#define Y_LEVEL_TOP    10.0
#define Y_LEVEL_BOTTOM -10.0
#define X_LEVEL_LEFT   10.0
#define X_LEVEL_RIGHT  -10.0
#define BUBBLE_R       20

float bubble_x = 0, bubble_y = 0;
int screen_mid_x, screen_mid_y, largecircle_r;

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
  
  Serial.println("Sensor Lab - Accelerometer Spirit Level!");
  arcada.display->setTextColor(ARCADA_BLUE);
  arcada.display->println("- Sensor Lab -");
  arcada.display->println("Accel. Spirit Level!\n");
  lab.begin();

  Serial.println("Looking for an accelerometer");
  accel = lab.getAccelerometer();
  if (! accel) {
    Serial.println(F("Could not find a accelerometer, check wiring!"));
    arcada.haltBox("Could not find a accelerometer, check wiring!");
  }

  accel->printSensorDetails();
  sensor_t sensor;
  accel->getSensor(&sensor);
  arcada.display->setTextColor(ARCADA_GREEN);
  arcada.display->printf("Found: %s\n", sensor.name);
  arcada.display->printf("Range: %0.0fg ~ %0.0fg\n", sensor.min_value / 9.8, sensor.max_value / 9.8);
  arcada.display->setTextColor(ARCADA_WHITE);
  arcada.display->println("\nPress (A) to start\nthe spirit level\ndemo");

  while (! (arcada.justPressedButtons() & ARCADA_BUTTONMASK_A)) {
    arcada.readButtons();
    delay(10);
  }

  screen_mid_x = arcada.display->width()/2;
  screen_mid_y = arcada.display->height()/2;
  largecircle_r = min(screen_mid_x, screen_mid_y)-1;
  arcada.display->fillScreen(ARCADA_BLACK);
  bubble_x = screen_mid_x;
  bubble_y = screen_mid_y;
}


void loop() {
 
  accel->getEvent(&accel_event);

  /* Display the results (acceleration is measured in m/s^2) 
  Serial.print("Accel X: ");
  Serial.print(accel_event.acceleration.x);
  Serial.print(" \t, \tY: ");
  Serial.print(accel_event.acceleration.y);
  Serial.print(" \t, \tZ: ");
  Serial.print(accel_event.acceleration.z);
  Serial.println(" m/s^2 ");
*/

  // erase old bubble
  arcada.display->fillCircle(bubble_x, bubble_y, BUBBLE_R, ARCADA_BLACK);

  // draw rings
  arcada.display->drawCircle(screen_mid_x, screen_mid_x, largecircle_r, ARCADA_WHITE);
  arcada.display->drawCircle(screen_mid_x, screen_mid_x, largecircle_r / 2, ARCADA_YELLOW);
  arcada.display->drawCircle(screen_mid_x, screen_mid_x, largecircle_r / 4, ARCADA_GREEN);
    
  // calculate new bubble location & draw it
  bubble_x = Adafruit_SensorLab::mapf(accel_event.acceleration.x, 
                                      X_LEVEL_LEFT, X_LEVEL_RIGHT, 
                                      screen_mid_x - largecircle_r + BUBBLE_R + 2,
                                      screen_mid_x + largecircle_r - BUBBLE_R - 2);
  bubble_y = Adafruit_SensorLab::mapf(accel_event.acceleration.y,
                                      Y_LEVEL_TOP, Y_LEVEL_BOTTOM,
                                      screen_mid_x - largecircle_r + BUBBLE_R + 2,
                                      screen_mid_x + largecircle_r - BUBBLE_R - 2);

  if ((abs(bubble_x - screen_mid_x) < (largecircle_r / 4 - BUBBLE_R)) &&
      (abs(bubble_y - screen_mid_y) < (largecircle_r / 4 - BUBBLE_R))) {
   arcada.display->fillCircle(bubble_x, bubble_y, BUBBLE_R, ARCADA_GREEN);
  } else {
   arcada.display->fillCircle(bubble_x, bubble_y, BUBBLE_R, ARCADA_RED);    
  }
  delay(100);
}
