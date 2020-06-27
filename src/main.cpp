#include <HTTPClient.h>
#include <M5Stack.h>
#include <WiFi.h>

#include "./config.h"
#include "time.h"

#define SERVER_URL \
  "https://api.thingspeak.com/apps/thingtweet/1/statuses/update"
#define LOCKED 0
#define UNLOCKED 1

void postTweet(String tweet);
String getTimeString();

RTC_DATA_ATTR int bootCount = 0;
const gpio_num_t PIN = GPIO_NUM_16;

const char* ntpServer = "ntp.nict.jp";
const long gmtOffset_sec = 9 * 3600;  // +9hours
const int daylightOffset_sec = 0;     // summer time offset

void setup() {
  bootCount++;
  M5.begin();
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  M5.Lcd.printf("Connecting to the WiFi AP: %s ", WIFI_SSID);

  while (WiFi.status() != WL_CONNECTED) {
    M5.Lcd.print(".");
    delay(500);
  }

  M5.Lcd.println(" connected.");
  M5.Lcd.print("IP address: ");
  M5.Lcd.println(WiFi.localIP());

  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

  pinMode(PIN, INPUT_PULLUP);
  int now_state = digitalRead(PIN);

  if (now_state == LOCKED) {
    M5.Lcd.println("Locked.");
    postTweet(String("Locked! at" + getTimeString() + " bootCount " +
                     String(bootCount)));
    esp_sleep_enable_ext0_wakeup(PIN, UNLOCKED);
  } else {
    M5.Lcd.println("Unlocked.");
    postTweet(String("Unlocked. Check the key!! at" + getTimeString() +
                     " bootCount " + String(bootCount)));
    esp_sleep_enable_ext0_wakeup(PIN, LOCKED);
  }

  M5.Lcd.println("Going to sleep.");
  delay(100);
  esp_deep_sleep_start();
}

void loop() {}

void postTweet(String tweet) {
  HTTPClient http;
  http.begin(SERVER_URL);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  String tsData =
      String("api_key=" + String(THING_TWEET_API_KEY) + "&status=" + tweet);
  int httpCode = http.POST(tsData);
  M5.Lcd.printf("Response: %d", httpCode);
  M5.Lcd.println();
  if (httpCode == HTTP_CODE_OK) {
    String body = http.getString();
    M5.Lcd.print("Response Body: ");
    M5.Lcd.println(body);
  }
  delay(15000);
}

String getTimeString() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    return "Failed to obtain time. #rand" + String(random(10000)) + "#";
  }
  return String(timeinfo.tm_year + 1900) + "/" + String(timeinfo.tm_mon + 1) +
         "/" + String(timeinfo.tm_mday) + " " + String(timeinfo.tm_hour) + ":" +
         String(timeinfo.tm_min) + ":" + String(timeinfo.tm_sec);
}
