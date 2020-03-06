#include <Adafruit_SSD1306.h>
#include <Adafruit_SensorLab.h>
Adafruit_SensorLab lab;

Adafruit_Sensor *humidity = NULL, *temp = NULL;
sensors_event_t humidity_event, temp_event;
Adafruit_SSD1306 display = Adafruit_SSD1306(128, 32, &Wire);

void setup() {
Serial.begin(115200);
//while (!Serial) delay(10);
Serial.println("Humidity and Temp OLED demo");
lab.begin();

// SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x32
  Serial.println(F("SSD1306 allocation failed"));
  for(;;); // Don't proceed, loop forever
}
display.display();
delay(500); // Pause for half second

Serial.println("Looking for a humidity sensor");
humidity = lab.getHumiditySensor();
if (! humidity) {
  Serial.println(F("Could not find a humidity sensor, skipping!"));
} else {
  humidity->printSensorDetails();
}

Serial.println("Looking for a temperature sensor");
temp = lab.getTemperatureSensor();
if (! temp) {
  Serial.println(F("Could not find a temperature sensor, skipping!"));
} else {
  temp->printSensorDetails();
}

display.setTextSize(1);
display.setTextColor(WHITE);
display.setRotation(0);
}


void loop() {
if (temp && ! temp->getEvent(&temp_event)) {
  return;
}
if (humidity && ! humidity->getEvent(&humidity_event)) {
  return;
}

display.clearDisplay();
display.setCursor(0,0);

Serial.print("Humidity: ");
Serial.print(humidity_event.relative_humidity, 1);
Serial.println(" %rH");

display.print("Humidity:    ");
display.print(humidity_event.relative_humidity, 1);
display.println(" %rH");

Serial.print("Temperature: ");
Serial.print(temp_event.temperature, 1);
Serial.println(" degrees C");
Serial.println("");
//ref.print .("012345678901234567891");
display.println("");
display.print("Temp:      ");
display.print(temp_event.temperature, 1);
display.println(" deg C");

display.display();
delay(100);
}
