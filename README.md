DEN農01 UECS NODE デモンストレーションキット01
==============================================

M304とQ917を使ってUECSのデモンストレーションを行うキットセット。

- Version: V001
- Release: 24-Jan-2023
- Develop: HOLLY&Co.,Ltd.

# 1. 必要なH/W

- M304 制御ノード
- Q917 UECSシミュレータ
- LANケーブル2本
- スイッチングハブ


# 2. 配線

M304とQ917は、LANケーブルとスイッチングハブを介してLAN接続する。


# 3. ソフトウェアのインストール

## 3.1. 必要なライブラリ
- [Ethernet2](https://github.com/adafruit/Ethernet2)


## 3.2. EEPROMの初期値

EEPROMが壊れていなければ、再度初期化する必要はない。  
MACアドレスやUECSID,ROOM,REGIONなどの位置情報の初期値を設定する。

このプログラムは、lowcore サブディレクトリの配下にある。この lowcore.ino をArduinoに最初にインストールして初期値を作成する。  
詳細は、 [lowcore/README.md](lowcore/README.md) を参照の事。

## 3.3. 主プログラムのインストール

主プログラムは、M304用とQ917用の２つのプログラムが各々のサブディレクトリの配下に全て含まれている。
それを Arduino IDE などでコンパイルしてCPUにupload(install)する。

### 3.3.1. M304用のプログラム

M304用のプログラムが含まれる。

CLIによるインストール方法は以下のとおりである。

    $ arduino-cli compile -u -p /dev/ttyUSB0 -b arduino:avr:mega m304


### 3.3.2. Q917用のプログラム

CLIによるインストール方法は以下のとおりである。

    $ arduino-cli compile -u -p /dev/ttyUSB0 -b arduino:avr:nano q917


# 4. 使い方

## 4.1. 気温

設定値(既定値：18.0℃)よりも観測値が小さくなるとRLY7がMAKEして、RLY8がBREAKする。
少しややこしいので下に関連式を記す。(T:観測値)

   T ＜ 18.0 ＝ (RLY7:MAKE,RLY8:BREAK)
   18.0 ≦ T ＜ 28.0 ＝ (RLY7:BREAK,RLY8:BREAK)
   28.0 ≦ T ＝ (RLY7:BREAK,RLY8:MAKE)

 
## 4.2. 湿度

設定値(既定値：50%)よりも観測値が小さくなるとRLY1がMAKEする。

## 4.3. 日射量

設定値(既定値：800kW/m2)よりも観測値が小さくなるとRLY3がMAKEする。

## 4.4. CO2濃度

設定値(既定値：350ppm)よりも観測値が小さくなるとRLY4がMAKEする。


## その他

使い方は、取扱説明書を [main/MANUAL.md](main/MANUAL.md) に記すので、そちらを参照する。
まだ書かれていない。

予告なく変更される可能性はある。

# 5. 著作権

This software is released under the MIT License, see [LICENSE.txt](LICENSE.txt).  
(このソフトウェアは、MITライセンスのもとで公開されている。[LICENSE-ja.txt](LICENSE-ja.txt)を見よ。)
