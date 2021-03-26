# WeatherStationForM5PAPER
M5PAPER用のWeatherStation

# はじめに

これは、M5PAPER用に作成されたお天気ステーションです。
時計、温湿度計、天気予報を表示します。
また、SDファイルのルートに配置された写真をランダムに表示します。
 
PlatformIOのプロジェクト形式となっています。

# 必要な追加ファイル

このコードからは、必要なファイルが2つ、削除されています。
wifiid.hとapikey.hです。
ファイルは、"src/"に配置します。

## wifiid.h
以下の形式で、作成してください。

```src/wifiid.h
#define ssid "ssidname"
#define password "password"
```

## apikey.h
以下の形式です。

```src/apikey.h
#define apikey "openweatherのapikey"
```

天気情報を、[openweathermap](https://openweathermap.org/)より、取得しています。
apiキーが必要ですので、取得の上、apikeyをapikey.hに設定してください。

# 表示する写真
写真は、SDカードのルート直下に配置するように作成しています。また、480×320のサイズでの表示を前提としています。
表示装置の特性上、グレースケールです。
サイズとカラーを編集してSDカードにコピーします。グレースケールの階調度は16(4ビット)です。
例えば、ImageMagicで編集するなら、

```
convert -colorspace Gray -depth 4 -resize 480x320 -strip src/*.jpg dest/photo%03d.jpg
```

のような感じになります。
もし、GIMPで編集するなら、保存時に、プログレッシブと最適化の設定を無効にしてください。これを忘れると何も表示しません。
