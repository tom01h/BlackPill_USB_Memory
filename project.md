# Project設定
## Project作成
#### STMF411CEU6を選択
![](image/F411.png)

#### 名前を決める
![](image/PJNAME.png)

## Pinout&Configurationタブ
#### 外部クロックを使う設定
![](image/RCC.png)

#### SPIを使う設定
![](image/SPI.png)

#### SPIのCS
GPIO PA4を出力で使う  
![](image/GPIO.png)

#### USBの設定
![](image/USB_FS.png)

#### USB Masstrage Classを使う
MSC_MEDIA_PACKETは4KB  
![](image/USB_DEVICE.png)

#### FATFSの設定
![](image/FATFS.png)

## Clock Confgurationタブ
USBのクロックは48MHzでないとダメ  
![](image/CLOCK.png)
