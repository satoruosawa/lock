#include <Arduino.h>

#include <WiFi.h>
#include <HTTPClient.h>
#include "time.h"

#include "./config.h"

#define SERVER_URL "https://api.thingspeak.com/apps/thingtweet/1/statuses/update"
#define LOCKED 0
#define UNLOCKED 1

void postTweet(String tweet);
String getTimeString();

RTC_DATA_ATTR int bootCount = 0;
const gpio_num_t PIN = GPIO_NUM_14;

const char* ntpServer = "ntp.nict.jp";
const long gmtOffset_sec = 9 * 3600; // +9hours
const int daylightOffset_sec = 0; // summer time offset

void setup() {
  bootCount++;
  Serial.begin(115200);
  Serial.println("Start");

  while (!Serial);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.printf("Connecting to the WiFi AP: %s ", WIFI_SSID);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println(" connected.");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

  pinMode(PIN, INPUT_PULLUP);
  int now_state = digitalRead(PIN);

  if (now_state == LOCKED) {
    Serial.println("Locked.");
    postTweet(String("Locked! at" + getTimeString() + " bootCount " + String(bootCount)));
    esp_sleep_enable_ext0_wakeup(PIN, UNLOCKED);
  } else {
    Serial.println("Unlocked.");
    postTweet(String("Unlocked. Check the key!! at" + getTimeString() + " bootCount " + String(bootCount)));
    esp_sleep_enable_ext0_wakeup(PIN, LOCKED);
  }

  Serial.println("Going to sleep.");
  delay(100);
  esp_deep_sleep_start();
}

void loop() {
}

void postTweet(String tweet) {
  HTTPClient http;
  http.begin(SERVER_URL);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  String tsData = String("api_key=" + String(THING_TWEET_API_KEY) + "&status=" + tweet);
  int httpCode = http.POST(tsData);
  Serial.printf("Response: %d", httpCode);
  Serial.println();
  if (httpCode == HTTP_CODE_OK) {
    String body = http.getString();
    Serial.print("Response Body: ");
    Serial.println(body);
  }
  delay(15000);
}

String getTimeString() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    return "Failed to obtain time. #rand" + String(random(10000)) + "#";
  }
  return String(timeinfo.tm_year + 1900) + "/" +
    String(timeinfo.tm_mon + 1) + "/" +
    String(timeinfo.tm_mday) + " " +
    String(timeinfo.tm_hour) + ":" +
    String(timeinfo.tm_min) + ":" +
    String(timeinfo.tm_sec);
}
