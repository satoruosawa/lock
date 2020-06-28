### Overview
- ESPr Developer32から、ThingSpeakにデータを送信して、ThingTweetでTwitterに投稿する。
### setting
- ThingTweetを登録して、APIを生成
- src directoryにconfig.hを追加
```cpp:config.h
#define WIFI_SSID "xxxxxx"
#define WIFI_PASSWORD "xxxxxx"
#define THING_TWEET_API_KEY "xxxxxx"
```

### Reference
- [Sleep Modes](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/system/sleep_modes.html)

### TODO
- WiFiに接続できなかったときの処理
  - 少しずつ間隔を開けながら再接続
- バッテリーが少なくなってきたらツイート
- ボタンインタラクション
- リセット時にはツイートしない