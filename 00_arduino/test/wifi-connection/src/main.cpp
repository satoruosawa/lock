#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>

#include "./wifi-info.h"

const char SSID[] = WIFI_SSID; // "xxxx"
const char PASSWORD[] = WIFI_PASSWORD; // "xxxx"
const char URL[] = SERVER_URL; // "http://xxx.xxx.xxx.xxx:8000/"

void setup() {
  Serial.begin(115200);
  while(!Serial);

  WiFi.begin(SSID, PASSWORD);
  Serial.printf("Connecting to the WiFi AP: %s ", SSID);

  while(WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println(" connected.");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  HTTPClient client;
  client.begin(URL);

  Serial.printf("Response: %d\n", client.GET());
  String body = client.getString();
  Serial.print("Response Body: ");
  Serial.println(body);

  delay(1000);
}
