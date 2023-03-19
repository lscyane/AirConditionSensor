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

## License
- [AirConditionSensor - MIT](LICENSE)

QMP6988 のデバイス制御に [M5Unit-ENV](https://github.com/m5stack/M5Unit-ENV) のソースコードをベースにしています
- [M5Unit-ENV - MIT](https://github.com/m5stack/M5Unit-ENV/blob/master/LICENSE)