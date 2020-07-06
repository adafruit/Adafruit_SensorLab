/* Simple demo to detect an accelerometer and gyro and display their measurements
 *  to an SSD1306 OLED. Works well with the Adafruit 0.91" Stemma QT OLED:
 *  https://www.adafruit.com/product/4440
 */

#include <Adafruit_SSD1306.h>
#include <Adafruit_SensorLab.h>
Adafruit_SensorLab lab;

Adafruit_Sensor *gyro = NULL, *accel = NULL;
sensors_event_t gyro_event, accel_event;
Adafruit_SSD1306 display = Adafruit_SSD1306(128, 32, &Wire);

void setup() {
  Serial.begin(115200);
  while (!Serial) delay(10);
  Serial.println("6-DoF Accel/Gyro OLED demo");
  lab.begin();

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  display.display();
  delay(500); // Pause for half second

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

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setRotation(0);
}


void loop() {
  if (gyro && ! gyro->getEvent(&gyro_event)) {
    return;
  }
  if (accel && ! accel->getEvent(&accel_event)) {
    return;
  }

  display.clearDisplay();
  display.setCursor(0,0);

  Serial.print("Accelerometer ");
  Serial.print("X: "); Serial.print(accel_event.acceleration.x, 1);
  Serial.print(" m/s^2, ");
  Serial.print("Y: "); Serial.print(accel_event.acceleration.y, 1);
  Serial.print(" m/s^2, ");
  Serial.print("Z: "); Serial.print(accel_event.acceleration.z, 1);
  Serial.println(" m/s^2");

  display.println("Accelerometer - m/s^2");
  display.print(accel_event.acceleration.x, 1); display.print(", ");
  display.print(accel_event.acceleration.y, 1); display.print(", ");
  display.print(accel_event.acceleration.z, 1); display.println("");

  Serial.print("Gyroscope ");
  Serial.print("X: "); Serial.print(gyro_event.gyro.x*Adafruit_SensorLab::DEGREES_PER_RADIAN, 1);
  Serial.print(" dps, ");
  Serial.print("Y: "); Serial.print(gyro_event.gyro.y*Adafruit_SensorLab::DEGREES_PER_RADIAN, 1);
  Serial.print(" dps, ");
  Serial.print("Z: "); Serial.print(gyro_event.gyro.z*Adafruit_SensorLab::DEGREES_PER_RADIAN, 1);
  Serial.println(" dps");

  display.println("Gyroscope - dps");
  display.print(gyro_event.gyro.x*Adafruit_SensorLab::DEGREES_PER_RADIAN, 1);
  display.print(", ");
  display.print(gyro_event.gyro.y*Adafruit_SensorLab::DEGREES_PER_RADIAN, 1);
  display.print(", ");
  display.print(gyro_event.gyro.z*Adafruit_SensorLab::DEGREES_PER_RADIAN, 1);
  display.println("");

  display.display();
  delay(100);
}