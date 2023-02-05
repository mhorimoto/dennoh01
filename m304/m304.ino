//================================================================
//  DEN農01　制御部プログラム
//  Modifier: Masafumi Horimoto
//  Based on UARDECS Sample Program
//  Ver1.00
//================================================================

#include <avr/pgmspace.h>
#include <LiquidCrystal.h>
#include <SPI.h>
#include <Ethernet2.h>
#include <EEPROM.h>
#include <AT24CX.h>
#include <Wire.h>
#include <DS1307RTC.h>
#include <Uardecs_mega.h>
#include "m304def.h"
#include "m304const.h"

LiquidCrystal lcd(RS,RW,ENA,DB0,DB1,DB2,DB3,DB4,DB5,DB6,DB7);
AT24C256      atmem(0);
EthernetClient client;

char pgname[21],lcdtxt[21];
char U_nodename[20] = "M304jp";//MAX 19chars (This value enabled in safemode)
UECSOriginalAttribute U_orgAttribute;//この定義は弄らないで下さい

//////////////////////////////////
// UserCCM setting
// CCM用の素材
//////////////////////////////////

//define CCMID for identify
//CCMID_dummy must put on last
//可読性を高めるためCCMIDという記号定数を定義しています
enum {
  CCMID_InAirTemp,
  CCMID_CndInAirTemp,
  CCMID_InAirHumid,
  CCMID_CndInAirHumid,
  CCMID_Illuminance,
  CCMID_CndIlluminance,
  CCMID_REncoder,
  CCMID_CndREncoder,
  CCMID_dummy, //CCMID_dummyは必ず最後に置くこと
};

//CCM格納変数の宣言
//ここはこのままにして下さい
const int U_MAX_CCM = CCMID_dummy;
UECSCCM U_ccmList[U_MAX_CCM];

//CCM定義用の素材
const char ccmNameTemp[] PROGMEM= "Temperature";
const char ccmTypeTemp[] PROGMEM= "InAirTemp.mIC";
const char ccmUnitTemp[] PROGMEM= "C";

const char ccmNameCndTemp[] PROGMEM= "NodeCond_Temp";
const char ccmTypeCndTemp[] PROGMEM= "CndTemp.aXX";
const char ccmUnitCndTemp[] PROGMEM= "";

const char ccmNameHumid[] PROGMEM= "AirHumid";
const char ccmTypeHumid[] PROGMEM= "InAirHumid.mIC";
const char ccmUnitHumid[] PROGMEM= "C";

const char ccmNameCndHumid[] PROGMEM= "NodeCond_Humid";
const char ccmTypeCndHumid[] PROGMEM= "CndHumid.aXX";
const char ccmUnitCndHumid[] PROGMEM= "";

const char ccmNameIlluminance[] PROGMEM= "Illuminance";
const char ccmTypeIlluminance[] PROGMEM= "Illuminance.mIC";
const char ccmUnitIlluminance[] PROGMEM= "";

const char ccmNameCndIlluminance[] PROGMEM= "NodeCond_Illuminance";
const char ccmTypeCndIlluminance[] PROGMEM= "CndIlluminance.aXX";
const char ccmUnitCndIlluminance[] PROGMEM= "";

const char ccmNameREncoder[] PROGMEM= "REncoder";
const char ccmTypeREncoder[] PROGMEM= "REncoder.mIC";
const char ccmUnitREncoder[] PROGMEM= "";

const char ccmNameCndREncoder[] PROGMEM= "NodeCond_REncoder";
const char ccmTypeCndREncoder[] PROGMEM= "CndREncoder.aXX";
const char ccmUnitCndREncoder[] PROGMEM= "";

//------------------------------------------------------
//UARDECS初期化用関数
//主にCCMの作成とMACアドレスの設定を行う
//------------------------------------------------------
void UserInit() {
//MAC address is printed on sticker of Ethernet Shield.
//You must assign unique MAC address to each nodes.
//MACアドレス設定、必ずEthernet Shieldに書かれた値を入力して下さい。
//全てのノードに異なるMACアドレスを設定する必要があります。
  U_orgAttribute.mac[0] = 0x02;
  U_orgAttribute.mac[1] = 0xA2;
  U_orgAttribute.mac[2] = 0x73;
  U_orgAttribute.mac[3] = 0x0B;
  U_orgAttribute.mac[4] = 0x00;
  U_orgAttribute.mac[5] = 0x03;
  
//Set ccm list
//CCMの作成
//UECSsetCCM(送/受の別, CCMID(固有の整数), CCM名(表示用), type, 単位,優先度(通常29), 小数桁数, 送受信頻度);
//true:送信 false:受信になります
  UECSsetCCM(false, CCMID_InAirTemp, ccmNameTemp, ccmTypeTemp, ccmUnitTemp, 29, DECIMAL_DIGIT, A_10S_0);
  UECSsetCCM(true,  CCMID_CndInAirTemp, ccmNameCndTemp, ccmTypeCndTemp, ccmUnitCndTemp, 29, DECIMAL_DIGIT_0, A_1S_0);
  UECSsetCCM(false, CCMID_InAirHumid, ccmNameHumid, ccmTypeHumid, ccmUnitHumid, 29, DECIMAL_DIGIT_0, A_10S_0);
  UECSsetCCM(true,  CCMID_CndInAirHumid, ccmNameCndHumid, ccmTypeCndHumid, ccmUnitCndHumid, 29,  DECIMAL_DIGIT_0, A_1S_0);
  UECSsetCCM(false, CCMID_Illuminance, ccmNameIlluminance, ccmTypeIlluminance, ccmUnitIlluminance, 29, DECIMAL_DIGIT_0, A_10S_0);
  UECSsetCCM(true,  CCMID_CndIlluminance, ccmNameCndIlluminance, ccmTypeCndIlluminance, ccmUnitCndIlluminance, 29,  DECIMAL_DIGIT_0, A_1S_0);
  UECSsetCCM(false, CCMID_REncoder, ccmNameREncoder, ccmTypeREncoder, ccmUnitREncoder, 29, DECIMAL_DIGIT_0, A_10S_0);
  UECSsetCCM(true,  CCMID_CndREncoder, ccmNameCndREncoder, ccmTypeCndREncoder, ccmUnitCndREncoder, 29,  DECIMAL_DIGIT_0, A_1S_0);
}

//---------------------------------------------------------
//Webページから入力が行われ各種値を取得後以下の関数が呼び出される。
//この関数呼び出し後にEEPROMへの値の保存とWebページの再描画が行われる
//---------------------------------------------------------
void OnWebFormRecieved(){
  ChangeThermostat();
}


//---------------------------------------------------------
//毎秒１回呼び出される関数
//関数の終了後に自動的にCCMが送信される
//---------------------------------------------------------
void UserEverySecond(){
  ChangeThermostat();
}

//---------------------------------------------------------
//１分に１回呼び出される関数
//---------------------------------------------------------
void UserEveryMinute(){
}

//---------------------------------------------------------
//メインループ
//システムのタイマカウント，httpサーバーの処理，
//UDP16520番ポートと16529番ポートの通信文をチェックした後，呼び出さされる関数。
//呼び出される頻度が高いため，重い処理を記述しないこと。
//---------------------------------------------------------
void UserEveryLoop(){
  
}

//---------------------------------------------------------
//setup()実行後に呼び出されるメインループ
//この関数内ではUECSloop()関数を呼び出さなくてはならない。
//UserEveryLoop()に似ているがネットワーク関係の処理を行う前に呼び出される。
//必要に応じて処理を記述してもかまわない。
//呼び出される頻度が高いため,重い処理を記述しないこと。
//---------------------------------------------------------
void loop(){
  UECSloop();
}

//---------------------------------------------------------
//起動直後に１回呼び出される関数。
//様々な初期化処理を記述できる。
//この関数内ではUECSsetup()関数を呼び出さなくてはならない。
//必要に応じて処理を記述してもかまわない。
//---------------------------------------------------------
void setup(){
  char pgn[21];
  int i;
  Serial.begin(115200);
  Ethernet.init(53);
  pinMode(SW_ENTER,INPUT_PULLUP);
  pinMode(SW_UP,INPUT_PULLUP);
  pinMode(SW_DOWN,INPUT_PULLUP);
  pinMode(SW_LEFT,INPUT_PULLUP);
  pinMode(SW_RIGHT,INPUT_PULLUP);
  pinMode(SW_SAFE,INPUT_PULLUP);
  pinMode(SW_RLY,INPUT);
  pinMode(SELECT_VR,INPUT);
  pinMode(RLY1,OUTPUT);
  digitalWrite(RLY1,HIGH);
  pinMode(RLY2,OUTPUT);
  digitalWrite(RLY2,HIGH);
  pinMode(RLY3,OUTPUT);
  digitalWrite(RLY3,HIGH);
  pinMode(RLY4,OUTPUT);
  digitalWrite(RLY4,HIGH);
  pinMode(RLY5,OUTPUT);
  digitalWrite(RLY5,HIGH);
  pinMode(RLY6,OUTPUT);
  digitalWrite(RLY6,HIGH);
  pinMode(RLY7,OUTPUT);
  digitalWrite(RLY7,HIGH);
  pinMode(RLY8,OUTPUT);
  digitalWrite(RLY8,HIGH);
  lcd.begin(20,4);
  lcd.setCursor(0,0);
  for(i=0;i<20;i++) {
    pgn[i] = pgm_read_byte(&(U_name[i]));
  }
  pgn[i] = NULL;
  lcd.print(pgn);
  UECSsetup();
}

//---------------------------------------------------------
//サーモスタット動作を変化させる関数
//---------------------------------------------------------
void ChangeThermostat(){
//温度 やめる
  showValueTemp = U_ccmList[CCMID_InAirTemp].value;
  if(setONOFFAUTO_Temp==0) {
    U_ccmList[CCMID_CndInAirTemp].value=0;
    Serial.print("TEMP-OFF\r\n");
    sidewindow(0); // STOP
    //    digitalWrite(LED_Blue_Pin, LOW);
    //    digitalWrite(D0_Pin, LOW);
  } else if(setONOFFAUTO_Temp==1) {
    // Manual ON
    U_ccmList[CCMID_CndInAirTemp].value=1;
    Serial.println("TEMP-ON");
    sidewindow(0);
    //    digitalWrite(LED_Blue_Pin, HIGH);
    //    digitalWrite(D0_Pin, HIGH);
  }
  else if(setONOFFAUTO_Temp==2 && U_ccmList[CCMID_InAirTemp].validity && U_ccmList[CCMID_InAirTemp].value<setONTempFromWeb) {
    U_ccmList[CCMID_CndInAirTemp].value=1;
    Serial.println("TEMP-ON");
    //    digitalWrite(LED_Blue_Pin, HIGH);
    //    digitalWrite(D0_Pin, HIGH);
  }//Auto ON
  else {
    U_ccmList[CCMID_CndInAirTemp].value=0;
    Serial.println("TEMP-OFF");
    //    digitalWrite(LED_Blue_Pin, LOW);
    //     digitalWrite(D0_Pin, LOW);
 }//OFF
  showValueStatusTemp = U_ccmList[CCMID_CndInAirTemp].value;
  Serial.print("CndTemp: ");
  Serial.println(U_ccmList[CCMID_CndInAirHumid].value);

//湿度　温風機が動く
  showValueHumidity = U_ccmList[CCMID_InAirHumid].value;
  if(setONOFFAUTO_Humidity==0) {
    U_ccmList[CCMID_CndInAirHumid].value=0;
    Serial.println("HUMID-OFF");
    digitalWrite(RLY1, HIGH);
 }//Manual OFF
  else if(setONOFFAUTO_Humidity==1) {
    U_ccmList[CCMID_CndInAirHumid].value=1;
    Serial.println("HUMID-ON");
    digitalWrite(RLY1, LOW);
  }//Manual ON
  else if(setONOFFAUTO_Humidity==2 && U_ccmList[CCMID_InAirHumid].validity && U_ccmList[CCMID_InAirHumid].value > setONHumidityFromWeb) {
    U_ccmList[CCMID_CndInAirHumid].value=1;
    Serial.println("HUMID-ON");
    digitalWrite(RLY1, LOW);
  }//Auto ON
  else {
    U_ccmList[CCMID_CndInAirHumid].value=0;
    Serial.println("HUMID-OFF");
    digitalWrite(RLY1, HIGH);
  }//OFF
  showValueStatusHumidity = U_ccmList[CCMID_CndInAirHumid].value;
  Serial.print("CndHumid: ");
  Serial.println(showValueStatusHumidity);

//照度　LEDランプ
  showValueIlluminance = U_ccmList[CCMID_Illuminance].value;
  if(setONOFFAUTO_Illuminance==0) {
    U_ccmList[CCMID_CndIlluminance].value=0;
    Serial.println("ILLUMINANCE-OFF");
    led_lamp(0);
  }//Manual OFF
  else if(setONOFFAUTO_Illuminance==1) {
    U_ccmList[CCMID_CndIlluminance].value=1;
    Serial.println("ILLUMINANCE-ON");
    led_lamp(1);
  }//Manual ON
  else if(setONOFFAUTO_Illuminance==2 && U_ccmList[CCMID_Illuminance].validity && U_ccmList[CCMID_Illuminance].value<setONIlluminanceFromWeb) {
    U_ccmList[CCMID_CndIlluminance].value=1;
    Serial.println("ILLUMINANCE-ON");
    led_lamp(1);
  }//Auto ON
  else {
    U_ccmList[CCMID_CndIlluminance].value=0;
    Serial.println("ILLUMINANCE-OFF");
    led_lamp(0);
  }//OFF
  showValueStatusIlluminance = U_ccmList[CCMID_CndIlluminance].value;
  Serial.print("CndIlluminance: ");
  Serial.println(showValueStatusIlluminance);

//可変抵抗器　そくそう
  showValueREncoder = U_ccmList[CCMID_REncoder].value;
  if(setONOFFAUTO_REncoder==2 && U_ccmList[CCMID_REncoder].validity ) {
    if (U_ccmList[CCMID_REncoder].value <  300) {
      U_ccmList[CCMID_CndREncoder].value=1;
      Serial.println("REncoder-Under 300");
      sidewindow(1);
    } else if (U_ccmList[CCMID_REncoder].value >  700) {
      U_ccmList[CCMID_CndREncoder].value=2;
      Serial.println("REncoder-Over 700");
      sidewindow(2);
    } else {
      U_ccmList[CCMID_CndREncoder].value=4;
      Serial.println("REncoder-STOP");
      sidewindow(0);
    }
  }
  showValueStatusREncoder = U_ccmList[CCMID_CndREncoder].value;
  Serial.print("CndREncoder: ");
  Serial.println(showValueStatusREncoder);

}


//
// Side Window Motor Contorol
//
//   m : running mode
//       0 = STOP
//       1 = CLOSE
//       2 = OPEN
//       OTHERS = STOP
//
void sidewindow(int m) {
  switch(m) {
  case 1:
    digitalWrite(RLY7,LOW);
    digitalWrite(RLY8,HIGH);
    // Side window open
    break;
  case 2:
    digitalWrite(RLY7,HIGH);
    digitalWrite(RLY8,LOW);
    break;
  default:
    digitalWrite(RLY7,HIGH);
    digitalWrite(RLY8,HIGH);
  }
}

//
//  LED light on/off
//
void led_lamp(int m) {
  switch(m) {
  case 0:
    digitalWrite(RLY3,HIGH);
    break;
  default:
    digitalWrite(RLY3,LOW);
    break;
  }
}
