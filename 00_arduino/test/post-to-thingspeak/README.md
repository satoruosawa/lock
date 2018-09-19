# setup
- thingSpeak channel作成

- to check address
  - $ `ifconfig`
- add wifi-info.h
  ```
  #define WIFI_SSID "xxxx"
  #define WIFI_PASSWORD "xxxx"
  #define SERVER_URL "https://api.thingspeak.com/update?api_key=XXXXXXXXXXXXXX"
  ```

### ※
- thingSpeakの無料アカウントでの制限
  - メッセージインターバル15秒
  - 300万メッセージ／年
  - 商用利用は禁止

### 参照:
  - [ESPr Developer 32 で WROOM-32 を Wi-Fi に繋いでみる](http://blog.akanumahiroaki.com/entry/2018/03/19/080000)
  - [データを簡単に保存&グラフ化できるThingSpeakが便利！](http://iwathi3.hatenablog.com/entry/Data-to-Graph-ThingSpeak)
  - [センサーの計測値をクラウドにためて見てみる](https://www.sglabs.jp/thingspeak/)
