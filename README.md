# AirConditionSensor
for STM32F401CCUX

## 開発環境
STM32CubeIDE 1.10.1

## 使用デバイス
| 項目           | 型番等         |
| -------------- | ------------- |
| マイコン        | STM32F401CCU6 |
| LCD            | 0.91インチolediicシリアルディスプレイモジュール (ssd1306) |
| 温度・湿度・気圧センサ | M5Stack用温湿度気圧センサユニット Ver3 |
| CO2センサ       | MH-19C        |

## ディレクトリ構成の説明
<pre>
.
├── Application         .. ユーザーコード
|    ├── deviceXXX.c/.h .. デバイス毎に制御を管理する層
|    ├── lmacXXX.c/.h   .. Lower MAC 低レイヤアプリケーション管理層、デバイス層のとりまとめなど
|    └── umacXXX.c/.h   .. Upper MAC 高レイヤアプリケーション管理層
├── Core                .. CubeIDEが自動生成する
|    ├── Src
|    |    ├── main.c    .. CubeIDEが自動生成する。ユーザーコードへの接続は追記
├── Drivers             .. CubeIDEが自動生成する
├── Middlewares         .. CubeIDEが自動生成する
├── USB_DEVICE          .. CubeIDEが自動生成する

</pre>

## ソフトウェアの動きをざっくり説明
- 各モジュール(lmac層等)の初期化を呼び出します。
- タイマー割り込みを使用し、1秒毎にメインループを回します
- メインループでは、メインループを必要とするモジュールの関数を呼び出します。
    - まずセンサ値の取得を行います。<br>
      処理を並列にする程のものでもないので割り込みは使わずにブロッキングで通信処理を行っています。
    - センサ値の取得が終わったら、表示の更新を行います。
        - 表示処理は表示バッファをお用意しておいて、メインループ毎にLCD4つ分順番に全領域を転送します。

## License
- [AirConditionSensor - MIT](LICENSE)

QMP6988 のデバイス制御に [M5Unit-ENV](https://github.com/m5stack/M5Unit-ENV) のソースコードをベースにしています
- [M5Unit-ENV - MIT](https://github.com/m5stack/M5Unit-ENV/blob/master/LICENSE)