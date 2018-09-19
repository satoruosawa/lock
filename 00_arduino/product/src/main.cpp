#include <Arduino.h>
#include <WiFi.h>
// #include <string>
#include <HTTPClient.h>

#include "./wifi-info.h"

void sendValue(int value);

const char SSID[] = WIFI_SSID; // "xxxx"
const char PASSWORD[] = WIFI_PASSWORD; // "xxxx"
const char URL[] = SERVER_URL; // "http://xxx.xxx.xxx.xxx:8000/"
const int PIN = 14;
int PREV_STATE = 0;

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

  pinMode(PIN, INPUT_PULLUP);
}

void loop() {
  int now_state = digitalRead(PIN);
  Serial.println(now_state);
  if (PREV_STATE != now_state) {
    sendValue(now_state);
    PREV_STATE = now_state;
  }
  delay(1000);
}

void sendValue(int value) {
  HTTPClient http;
  String url = URL;
  String sValue = String(value, DEC);
  String request = String(url + "&field1=" + sValue);
  Serial.println(request);
  http.begin(request.c_str());
  String requestBody = "Hello World!";
  int httpCode = http.POST(requestBody);

  Serial.printf("Response: %d", httpCode);
  Serial.println();
  if (httpCode == HTTP_CODE_OK) {
    String body = http.getString();
    Serial.print("Response Body: ");
    Serial.println(body);
  }
  delay(15000);
}
