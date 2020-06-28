#include <HTTPClient.h>
#include <M5Stack.h>
#include <WiFi.h>

#include "./config.h"
#include "time.h"

#define SERVER_URL \
  "https://api.thingspeak.com/apps/thingtweet/1/statuses/update"
#define LOCKED 0
#define UNLOCKED 1

String getTimeString();
void beep(int frequency, int duration);
void postTweet(String tweet);

RTC_DATA_ATTR int bootCount = 0;
const gpio_num_t PIN = GPIO_NUM_2;

const char* ntpServer = "ntp.nict.jp";
const long gmtOffset_sec = 9 * 3600;  // +9hours
const int daylightOffset_sec = 0;     // summer time offset

void setup() {
  bootCount++;
  M5.begin();
  M5.Power.begin();
  M5.Power.setPowerBoostKeepOn(true);
  M5.Lcd.setBrightness(0);
  M5.Lcd.sleep();
  M5.Speaker.setVolume(7);

  // Get Status.
  int level = M5.Power.getBatteryLevel();
  pinMode(PIN, INPUT_PULLUP);
  int state = digitalRead(PIN);
  beep(3520, 50);

  // WiFi connection.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  beep(3951, 50);

  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

  // Tweet and set wakeup source.
  if (state == LOCKED) {
    postTweet(String("Locked! " + getTimeString() + ", Battery Level " + level +
                     ", bootCount " + bootCount));
    esp_sleep_enable_ext0_wakeup(PIN, UNLOCKED);
  } else {
    postTweet(String("Unlocked. Check the key!! " + getTimeString() +
                     ", Battery Level " + level + ", bootCount " + bootCount));
    esp_sleep_enable_ext0_wakeup(PIN, LOCKED);
  }
  esp_sleep_disable_wakeup_source(ESP_SLEEP_WAKEUP_TIMER);
  beep(4186, 50);

  delay(15000);
  esp_deep_sleep_start();
}

void loop() {}

void beep(int frequency, int duration) {
  M5.Speaker.tone(frequency, duration);
  for (int i = 0; i < duration + 10; i++) {
    M5.update();
    delay(1);
  }
}

void postTweet(String tweet) {
  HTTPClient http;
  http.begin(SERVER_URL);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  String tsData =
      String("api_key=" + String(THING_TWEET_API_KEY) + "&status=" + tweet);
  int httpCode = http.POST(tsData);
  if (httpCode == HTTP_CODE_OK) {
    String body = http.getString();
  }
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
