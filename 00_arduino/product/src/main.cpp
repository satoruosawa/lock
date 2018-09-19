#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>

#include "./config.h"

#define SERVER_URL "https://api.thingspeak.com/apps/thingtweet/1/statuses/update"

void postTweet(String tweet);

const char SSID[] = WIFI_SSID; // "xxxx"
const char PASSWORD[] = WIFI_PASSWORD; // "xxxx"
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
    if (now_state == 1) {
      postTweet(String("locked!"));
    } else {
      postTweet(String("unlocked!"));
    }
    PREV_STATE = now_state;
  }
  delay(1000);
}

void postTweet(String tweet) {
  HTTPClient http;
  http.begin(SERVER_URL);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  String tsData = String("api_key=" + String(THING_TWEET_API_KEY) + "&status=" + tweet);
  int httpCode = http.POST(tsData);
  if (httpCode == HTTP_CODE_OK) {
    String body = http.getString();
    Serial.print("Response Body: ");
    Serial.println(body);
  }
  delay(15000);
}
