#include <Arduino.h>
#include <WiFi.h>
#include "time.h"
#include "./config.h"

const char* ntpServer = "ntp.nict.jp";
const long gmtOffset_sec = 9 * 3600; // +9hours
const int daylightOffset_sec = 0; // summer time offset

void printLocalTime() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
    return;
  }
  Serial.println(&timeinfo, "%Y %m %d %a %H:%M:%S");
  Serial.println(
    String(timeinfo.tm_year + 1900) + "/" +
    String(timeinfo.tm_mon + 1) + "/" +
    String(timeinfo.tm_mday) + " " +
    String(timeinfo.tm_hour) + ":" +
    String(timeinfo.tm_min) + ":" +
    String(timeinfo.tm_sec)
  );
}

void setup() {
  Serial.begin(115200);

  Serial.printf("Connecting to %s ", WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println(" connected.");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  //init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  printLocalTime();

  //disconnect WiFi as it's no longer needed
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
}

void loop() {
  printLocalTime();
  delay(1000);
}
