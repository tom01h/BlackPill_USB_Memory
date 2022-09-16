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

#### RTCを使う
なんか文句ありげに見えるけどこのまま  
Activate Calenderをチェックすると毎回時刻が初期化される  
![](image/RTC.png)

#### USBの設定
![](image/USB_FS.png)

#### USB Masstrage Classを使う
MSC_MEDIA_PACKETは4KB  
![](image/USB_DEVICE.png)

#### FATFSの設定
MAX_SS,MIN_SSを4096にする  
![](image/FATFS.png)

## Clock Confgurationタブ
USBのクロックは48MHzでないとダメ  
RTCのクロックを外部の32.768kHzにする  
![](image/CLOCK.png)

## Project Managerタブ
Linker設定  
スタックとかヒープのサイズを増やす  
![](image/Linker.png)
