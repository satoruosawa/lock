#include <Arduino.h>

void setup() {
  Serial.begin(115200);
}

void loop() {
  Serial.println("Hello World from ESPr Developer 32 :)");
  delay(1000);
}
