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
  CCMID_Radiation,
  CCMID_CndRadiation,
  CCMID_CO2,
  CCMID_CndCO2,
  CCMID_dummy, //CCMID_dummyは必ず最後に置くこと
};

//CCM格納変数の宣言
//ここはこのままにして下さい
const int U_MAX_CCM = CCMID_dummy;
UECSCCM U_ccmList[U_MAX_CCM];

//CCM定義用の素材
const char ccmNameTemp[] PROGMEM= "Temperature";
const char ccmTypeTemp[] PROGMEM= "InAirTemp";
const char ccmUnitTemp[] PROGMEM= "C";

const char ccmNameCndTemp[] PROGMEM= "NodeCond_Temp";
const char ccmTypeCndTemp[] PROGMEM= "CndTemp.aXX";
const char ccmUnitCndTemp[] PROGMEM= "";

const char ccmNameHumid[] PROGMEM= "AirHumid";
const char ccmTypeHumid[] PROGMEM= "InAirHumid";
const char ccmUnitHumid[] PROGMEM= "%";

const char ccmNameCndHumid[] PROGMEM= "NodeCond_Humid";
const char ccmTypeCndHumid[] PROGMEM= "CndHumid.aXX";
const char ccmUnitCndHumid[] PROGMEM= "";

const char ccmNameRadiation[] PROGMEM= "Radiation";
const char ccmTypeRadiation[] PROGMEM= "InRadiation";
const char ccmUnitRadiation[] PROGMEM= "";

const char ccmNameCndRadiation[] PROGMEM= "NodeCond_Radiation";
const char ccmTypeCndRadiation[] PROGMEM= "CndRadiation.aXX";
const char ccmUnitCndRadiation[] PROGMEM= "";

const char ccmNameCO2[] PROGMEM= "CO2";
const char ccmTypeCO2[] PROGMEM= "InAirCO2";
const char ccmUnitCO2[] PROGMEM= "";

const char ccmNameCndCO2[] PROGMEM= "NodeCond_CO2";
const char ccmTypeCndCO2[] PROGMEM= "CndCO2.aXX";
const char ccmUnitCndCO2[] PROGMEM= "";

//------------------------------------------------------
//UARDECS初期化用関数
//主にCCMの作成とMACアドレスの設定を行う
//------------------------------------------------------
void UserInit() {
//MAC address is printed on sticker of Ethernet Shield.
//You must assign unique MAC address to each nodes.
//MACアドレス設定、必ずEthernet Shieldに書かれた値を入力して下さい。
//全てのノードに異なるMACアドレスを設定する必要があります。
  U_orgAttribute.mac[0] = atmem.read(0x06); //0x02;
  U_orgAttribute.mac[1] = atmem.read(0x07); //0xA2;
  U_orgAttribute.mac[2] = atmem.read(0x08); //0x73;
  U_orgAttribute.mac[3] = atmem.read(0x09); //0x0B;
  U_orgAttribute.mac[4] = atmem.read(0x0a); //0x00;
  U_orgAttribute.mac[5] = atmem.read(0x0b); //0x2b;
  
//Set ccm list
//CCMの作成
//UECSsetCCM(送/受の別, CCMID(固有の整数), CCM名(表示用), type, 単位,優先度(通常29), 小数桁数, 送受信頻度);
//true:送信 false:受信になります
  UECSsetCCM(false, CCMID_InAirTemp, ccmNameTemp, ccmTypeTemp, ccmUnitTemp, 29, DECIMAL_DIGIT, A_10S_0);
  UECSsetCCM(true,  CCMID_CndInAirTemp, ccmNameCndTemp, ccmTypeCndTemp, ccmUnitCndTemp, 29, DECIMAL_DIGIT_0, A_1S_0);
  UECSsetCCM(false, CCMID_InAirHumid, ccmNameHumid, ccmTypeHumid, ccmUnitHumid, 29, DECIMAL_DIGIT_0, A_10S_0);
  UECSsetCCM(true,  CCMID_CndInAirHumid, ccmNameCndHumid, ccmTypeCndHumid, ccmUnitCndHumid, 29,  DECIMAL_DIGIT_0, A_1S_0);
  UECSsetCCM(false, CCMID_Radiation, ccmNameRadiation, ccmTypeRadiation, ccmUnitRadiation, 29, DECIMAL_DIGIT_0, A_10S_0);
  UECSsetCCM(true,  CCMID_CndRadiation, ccmNameCndRadiation, ccmTypeCndRadiation, ccmUnitCndRadiation, 29,  DECIMAL_DIGIT_0, A_1S_0);
  UECSsetCCM(false, CCMID_CO2, ccmNameCO2, ccmTypeCO2, ccmUnitCO2, 29, DECIMAL_DIGIT_0, A_10S_0);
  UECSsetCCM(true,  CCMID_CndCO2, ccmNameCndCO2, ccmTypeCndCO2, ccmUnitCndCO2, 29,  DECIMAL_DIGIT_0, A_1S_0);
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
  pgm2mem(pgn,U_name);
  lcd.print(pgn);
  UECSsetup();
  sprintf(pgn,"%02X%02X.%02X%02X.%02X%02X",
	  U_orgAttribute.mac[0],U_orgAttribute.mac[1],
	  U_orgAttribute.mac[2],U_orgAttribute.mac[3],
	  U_orgAttribute.mac[4],U_orgAttribute.mac[5]);
  lcd.setCursor(0,1);
  lcd.print(pgn);
}

void pgm2mem(char a[],char b[]) {
  int i;
  for(i=0;i<20;i++) {
    a[i] = pgm_read_byte(&(b[i]));
  }
  a[i] = NULL;
}

//---------------------------------------------------------
//サーモスタット動作を変化させる関数
//---------------------------------------------------------
void ChangeThermostat(){
//温度 やめる
  showValueTemp = U_ccmList[CCMID_InAirTemp].value;
  Serial.print("showValueTemp=");
  Serial.println(showValueTemp);
  Serial.print("setONOFFAUTO_Temp=");
  Serial.println(setONOFFAUTO_Temp);
  if(setONOFFAUTO_Temp==0) {
    U_ccmList[CCMID_CndInAirTemp].value=0;
    sidewindow(0); // STOP
  } else if(setONOFFAUTO_Temp==1) {
    // Manual ON
    U_ccmList[CCMID_CndInAirTemp].value=1;
    sidewindow(0);
  } else if(setONOFFAUTO_Temp==2 && U_ccmList[CCMID_InAirTemp].validity && U_ccmList[CCMID_InAirTemp].value<setONTempFromWeb) {
    U_ccmList[CCMID_CndInAirTemp].value=1;
  } //Auto ON
  else {
    U_ccmList[CCMID_CndInAirTemp].value=0;
  }//OFF
  showValueStatusTemp = U_ccmList[CCMID_CndInAirTemp].value;
  Serial.print("CndTemp: ");
  Serial.println(U_ccmList[CCMID_CndInAirTemp].value);

//湿度　温風機が動く
  showValueHumidity = U_ccmList[CCMID_InAirHumid].value;
  Serial.print("setONOFFAUTO_Humidity=");
  Serial.println(setONOFFAUTO_Humidity);
  if(setONOFFAUTO_Humidity==0) {
    U_ccmList[CCMID_CndInAirHumid].value=0;
    digitalWrite(RLY1, HIGH);
 }//Manual OFF
  else if(setONOFFAUTO_Humidity==1){ 
    U_ccmList[CCMID_CndInAirHumid].value=1;
    digitalWrite(RLY1, LOW);
  }//Manual ON
  else if(setONOFFAUTO_Humidity==2 && U_ccmList[CCMID_InAirHumid].validity && U_ccmList[CCMID_InAirHumid].value > setONHumidityFromWeb) {
    U_ccmList[CCMID_CndInAirHumid].value=1;
    digitalWrite(RLY1, LOW);
  }//Auto ON
  else {
    U_ccmList[CCMID_CndInAirHumid].value=0;
    digitalWrite(RLY1, HIGH);
  }//OFF
  showValueStatusHumidity = U_ccmList[CCMID_CndInAirHumid].value;
  Serial.print("CndHumid: ");
  Serial.println(showValueStatusHumidity);

//照度　LEDランプ
  Serial.print("setONOFFAUTO_Radiation=");
  Serial.println(setONOFFAUTO_Radiation);
  showValueRadiation = U_ccmList[CCMID_Radiation].value;
  Serial.print("U_ccmList[CCMID_Radiation].validity=");
  Serial.println(U_ccmList[CCMID_Radiation].validity);
  if(setONOFFAUTO_Radiation==0) {
    U_ccmList[CCMID_CndRadiation].value=0;
    led_lamp(0);
  }//Manual OFF
  else if(setONOFFAUTO_Radiation==1) {
    U_ccmList[CCMID_CndRadiation].value=1;
    led_lamp(1);
  }//Manual ON
  else if(setONOFFAUTO_Radiation==2 && U_ccmList[CCMID_Radiation].validity && U_ccmList[CCMID_Radiation].value<setONRadiationFromWeb) {
    U_ccmList[CCMID_CndRadiation].value=1;
    led_lamp(1);
  }//Auto ON
  else {
    U_ccmList[CCMID_CndRadiation].value=0;
    led_lamp(0);
  }//OFF
  showValueStatusRadiation = U_ccmList[CCMID_CndRadiation].value;
  Serial.print("CndRadiation: ");
  Serial.println(showValueStatusRadiation);

//CO2 
  Serial.print("setONOFFAUTO_CO2=");
  Serial.println(setONOFFAUTO_CO2);
  showValueCO2 = U_ccmList[CCMID_CO2].value;
  Serial.print("U_ccmList[CCMID_CO2].validity=");
  Serial.println(U_ccmList[CCMID_CO2].validity);
  if(setONOFFAUTO_CO2==2 && U_ccmList[CCMID_CO2].validity ) {
    if (U_ccmList[CCMID_CO2].value <  300) {
      U_ccmList[CCMID_CndCO2].value=1;
      Serial.println("CO2-Under 300");
      co2_gen(1);
      sidewindow(1);
    } else if (U_ccmList[CCMID_CO2].value >  700) {
      U_ccmList[CCMID_CndCO2].value=2;
      Serial.println("CO2-Over 700");
      co2_gen(0);
      sidewindow(2);
    } else {
      U_ccmList[CCMID_CndCO2].value=4;
      Serial.println("CO2-STOP");
      co2_gen(0);
      sidewindow(0);
    }
  } else {
    if (setONOFFAUTO_CO2==0) {
      co2_gen(0);
      U_ccmList[CCMID_CndCO2].value=0;
    } else if (setONOFFAUTO_CO2==1) {
      co2_gen(1);
      U_ccmList[CCMID_CndCO2].value=1;
    }
  }
  showValueStatusCO2 = U_ccmList[CCMID_CndCO2].value;
  Serial.print("CndCO2: ");
  Serial.println(showValueStatusCO2);

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

//
//  CO2 Generator on/off
//    0: Off
//    1: On
//
void co2_gen(int m) {
  switch(m) {
  case 0:
    digitalWrite(RLY4,HIGH);
    break;
  default:
    digitalWrite(RLY4,LOW);
    break;
  }
}
