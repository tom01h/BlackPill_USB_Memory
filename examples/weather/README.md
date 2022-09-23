# BME280とMH-Z19で空気を測る
BME280とMH-Z19を接続して10秒ごとに空気を計測してLCDに表示  
LCDはI2C接続のSSD1306コントローラーの載った128x32ドットのよくわからないやつ  
5分ごとに計測結果をファイルに保存する

以下のライブラリを改変したものを使っています
- [BMP280_STM32](https://github.com/ciastkolog/BMP280_STM32)
    - BME280にも対応します
    - ライセンスは各ファイルを参照
- [stm32-ssd1306](https://github.com/afiskon/stm32-ssd1306)
    - SSD1306コントローラーならいろいろ対応するみたいです
    - ライセンスは`LICENSE_SSD1360`を参照
- [MH-Z14A, MH-Z19B CO2 ... Module](https://github.com/tobiasschuerg/MH-Z-CO2-Sensors)
    - 元がSTM32専用でないので改変量が多いかも
    - ライセンスは`LICENSE_MHZ`を参照
    
## 準備
センサの端子割り当て

```
SDA=B9, SCL=B8 ← BME280, LCD
TX=B6,  RX=B7  ← MH-Z19
```

## Project作成
USBメモリを作るprojectに加えて以下の設定をします

#### 名前を決める
この時にTargeted LanguageをC++にすること

#### I2Cを使う設定
- I2C1    SDA=B9, SCL=B8 

#### UARTを使う設定
- UART1 TX=B6, RX=B7, 9600bpsにする

## ソースコード準備
- STM32CubeIDEで生成したファイルの一部を、USBメモリを作るprojectのファイルで上書きする
- さらに、このディレクトリのファイルで上書きする
- [Winbond W25Q series tiny driver](https://github.com/tom01h/winbond-w25-flash-drv)をダウンロードする。
    - `spi.c, w25_flash.c` を `Core/Src/` にコピーする
    - `spi.h, w25_flash.h` を `Core/Inc/` にコピーする

## 実行
- STM32CubeIDEで書き込む
- PCにつなぐとUSBメモリとして認識する
    - 最初はフォーマットが必要
- 電源だけつなぐとLCDに表示され、ログファイル `WEATHER.CSV` ができる
- 電源だけつないだ時に `SETTIME.TXT` があると時刻を設定する
    ```
    22-09-23 20:11
    ```