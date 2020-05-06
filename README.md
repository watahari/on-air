# ON-AIR light controller

![実行例](https://user-images.githubusercontent.com/4580778/81174157-4582d280-8fdc-11ea-853d-48524967a8d1.jpeg)
<blockquote class="twitter-tweet"><p lang="ja" dir="ltr">GWの自由工作が完了しました。<br>M5StackをWi-Fiに接続、ON-AIRサインライトをワイヤレスでオン/オフできます。 <a href="https://t.co/vy8erqciam">pic.twitter.com/vy8erqciam</a></p>&mdash; わたはり男爵 (@wata_hari) <a href="https://twitter.com/wata_hari/status/1257974356145131520?ref_src=twsrc%5Etfw">May 6, 2020</a></blockquote>

そのへんのスマートプラグを使って, ON-AIRサインライトを, M5StackからON/OFFさせる.<br>
例えば, 在宅勤務でリモートミーティングするときに "ON-AIR" を点灯させることで, 家族が誤って乱入してしまうインシデントを防ぐことに使えます.

## 利用要素

* M5Stack
* ON-AIRサインライト http://amzn.to/3fg1hyX
* スマートプラグ(Smartlifeアプリ連携) e.g. https://amzn.to/2L4bbFT

```
Power supply outlet
      |
+------------+                                                               +---------+
| Smart Plug |  <======= (smartlife) <====== (IFTTT webhooks) <===(Wi-Fi)=== | M5Stack |
+------------+                                                               +---------+
      |
+--------------+
| ON-AIR light | < flash
+--------------+
```

## setup

1. Connect your smart plug to smartlife.

2. Create Applets "webhooks to turn-on light" (event name:`on_air`) and "webhooks to turn-off light" (event name: `off_air`).<br>
Set your webhook key to `const char* webhook_key` (Line 12)

3. Set your home Wi-Fi SSID and password to `const char* ssid` `password` (Line 10,11).

4. Compilie and write to M5Stack.
