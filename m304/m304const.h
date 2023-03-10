/////////////////////////////////////
//IP reset jupmer pin setting
//IPアドレスリセット用ジャンパーピン設定
/////////////////////////////////////
//Pin ID. This pin is pull-upped automatically.
//ピンIDを入力、このピンは自動的にプルアップされます
//ピンIDは変更可能です
const byte U_InitPin = SW_SAFE;
const byte U_InitPin_Sense=LOW;

////////////////////////////////////
//Node basic infomation
//ノードの基本情報
///////////////////////////////////

const char U_name[] PROGMEM= "DEN-NOH01 v2.22\0";//MAX 20 chars
const char U_vender[] PROGMEM= "HOLLY&Co.Ltd.\0";//MAX 20 chars
const char U_uecsid[] PROGMEM= "10100C00000B\0";//12 chars fixed
const char U_footnote[] PROGMEM= "M304jp UARDECS version\0";
//const int U_footnoteLetterNumber = 48;//Abolished after Ver 0.6

//Total number of HTML table rows.
//web設定画面で表示すべき項目の総数
const int U_HtmlLine = 16;

//●表示素材の定義(1)数値表示
//UECSSHOWDATA
const char NAME0[] PROGMEM= "気温";
const char UNIT0[] PROGMEM= "C";
const char NOTE0[] PROGMEM= "";
//表示用の値を格納する変数
//小数桁数が1の場合、123が12.3と表示される
signed long showValueTemp;

//●表示素材の定義(2)選択肢表示
//UECSSELECTDATA
const char NAME1[]     PROGMEM= "動作モード";
const char NOTE1[]     PROGMEM= "";
const char UECSOFF[]   PROGMEM= "OFF";
const char UECSON[]    PROGMEM= "ON";
const char UECSSTOP[]  PROGMEM= "STOP";
const char UECSOPEN[]  PROGMEM= "OPEN";
const char UECSCLOSE[] PROGMEM= "CLOSE";
const char UECSRUN[]   PROGMEM= "RUN";
const char UECSAUTO[]  PROGMEM= "AUTO";
const char *stringSELECT[4]={
  UECSSTOP,
  UECSCLOSE,
  UECSOPEN,
  UECSAUTO
};

//入力された選択肢の位置を受け取る変数
//UECSOFFが0、UECSONで1、UECSAUTOで2になる
signed long setONOFFAUTO_Temp;

//●表示素材の定義(3)数値入力
//UECSINPUTDATA
const char NAME2[] PROGMEM= "気温しきい値";
const char UNIT2[] PROGMEM= "C";
const char NOTE2[] PROGMEM= "この値の前後で側窓の開閉が行われる";

//入力された数値を受け取る変数
//小数桁数が1の場合、例えばWeb上で12.3が入力されると123が代入される
signed long setONTempFromWeb;

//●表示素材の定義(4)文字表示
//UECSSHOWSTRING
const char NAME3[] PROGMEM= "状態";
const char NOTE3[] PROGMEM= "RLY7,RLY8</td><tr>\n<tr style=\"background:#888888;\"><td colspan=\"4\">";
const char SWIN_OPEN[]      PROGMEM= "OPEN RLY7,8(BREAK,MAKE)";
const char SWIN_CLOSE[]     PROGMEM= "CLOSE RLY7,8(MAKE,BREAK)";
const char SWIN_STOP[]      PROGMEM= "STOP RLY7,8(BREAK,BREAK)";
const char SHOWSTRING_OFF[] PROGMEM= "OUTPUT:OFF";
const char SHOWSTRING_ON [] PROGMEM= "OUTPUT:ON";
const char *TEMPstatus[3] ={
  SWIN_OPEN,
  SWIN_CLOSE,
  SWIN_STOP
};
const char *stringSHOW[2]={
  SHOWSTRING_OFF,
  SHOWSTRING_ON,
};
signed long showValueStatusTemp;

//●表示素材の定義(1)数値表示
//UECSSHOWDATA
const char NAME4[] PROGMEM= "湿度";
const char UNIT4[] PROGMEM= "%";
const char NOTE4[] PROGMEM= "";

//表示用の値を格納する変数
//小数桁数が1の場合、123が12.3と表示される
signed long showValueHumidity;

//●表示素材の定義(2)選択肢表示
//UECSSELECTDATA
const char NAME5[] PROGMEM= "動作モード";
const char NOTE5[] PROGMEM= "";
const char *stringSELECT_Humidity[3]={
  UECSOFF,
  UECSON,
  UECSAUTO,
};

//入力された選択肢の位置を受け取る変数
//UECSOFFが0、UECSONで1、UECSAUTOで2になる
signed long setONOFFAUTO_Humidity;

//●表示素材の定義(3)数値入力
//UECSINPUTDATA
const char NAME6[] PROGMEM= "湿度しきい値";
const char UNIT6[] PROGMEM= "%";
const char NOTE6[] PROGMEM= "この値よりも小さいとミストを動作\n";

//入力された数値を受け取る変数
//小数桁数が1の場合、例えばWeb上で12.3が入力されると123が代入される
signed long setONHumidityFromWeb;

//●表示素材の定義(4)文字表示
//UECSSHOWSTRING
const char NAME7[] PROGMEM= "状態";
const char NOTE7[] PROGMEM= "RLY1</td><tr>\n<tr style=\"background:#888888;\"><td colspan=\"4\">";
const char *stringSHOW_Humidity[2]={
  SHOWSTRING_OFF,
  SHOWSTRING_ON,
};
signed long showValueStatusHumidity;

//●表示素材の定義(1)数値表示
//UECSSHOWDATA
const char NAME8[] PROGMEM= "日射量";
const char UNIT8[] PROGMEM= "kW/m<sup>2</sup>";
const char NOTE8[] PROGMEM= "";

//表示用の値を格納する変数
//小数桁数が1の場合、123が12.3と表示される
signed long showValueRadiation;

//●表示素材の定義(2)選択肢表示
//UECSSELECTDATA
const char NAME9[] PROGMEM= "動作モード";
const char NOTE9[] PROGMEM= "";
const char *stringSELECT_Radiation[3]={
  UECSOFF,
  UECSON,
  UECSAUTO
};

//入力された選択肢の位置を受け取る変数
//UECSOFFが0、UECSONで1、UECSAUTOで2になる
signed long setONOFFAUTO_Radiation;

//●表示素材の定義(3)数値入力
//UECSINPUTDATA
const char NAME10[] PROGMEM= "日射量しきい値";
const char UNIT10[] PROGMEM= "kW/m<sup>2</sup>";
const char NOTE10[] PROGMEM= "この値を境に補光装置を動作させる。";

//入力された数値を受け取る変数
//小数桁数が1の場合、例えばWeb上で12.3が入力されると123が代入される
signed long setONRadiationFromWeb;

//●表示素材の定義(4)文字表示
//UECSSHOWSTRING
const char NAME11[] PROGMEM= "状態";
const char NOTE11[] PROGMEM= "RLY3</td><tr>\n<tr style=\"background:#888888;\"><td colspan=\"4\">";
const char *stringSHOW_Radiation[2]={
  SHOWSTRING_OFF,
  SHOWSTRING_ON,
};
signed long showValueStatusRadiation;

//●表示素材の定義(1)数値表示
//UECSSHOWDATA
const char NAME12[] PROGMEM= "CO<sub>2</sub>濃度";
const char UNIT12[] PROGMEM= "ppm";
const char NOTE12[] PROGMEM= "";

//表示用の値を格納する変数
//小数桁数が1の場合、123が12.3と表示される
signed long showValueCO2;

//●表示素材の定義(2)選択肢表示
//UECSSELECTDATA
const char NAME13[] PROGMEM= "動作モード";
const char NOTE13[] PROGMEM= "";
const char *stringSELECT_CO2[3]={
  UECSOFF,
  UECSON,
  UECSAUTO,
};

//入力された選択肢の位置を受け取る変数
//UECSOFFが0、UECSONで1、UECSAUTOで2になる
signed long setONOFFAUTO_CO2;

//●表示素材の定義(3)数値入力
//UECSINPUTDATA
const char NAME14[] PROGMEM= "CO<sub>2</sub>しきい値";
const char UNIT14[] PROGMEM= "ppm";
const char NOTE14[] PROGMEM= "この値を境にCO<sub>2</sub>発生装置のON/OFFを行う";

//入力された数値を受け取る変数
//小数桁数が1の場合、例えばWeb上で12.3が入力されると123が代入される
signed long setONCO2FromWeb;

//●表示素材の定義(4)文字表示
//UECSSHOWSTRING
const char NAME15[] PROGMEM= "状態";
const char NOTE15[] PROGMEM= "RLY4";
const char *stringSHOW_CO2[2]={
  SHOWSTRING_OFF,
  SHOWSTRING_ON,
};
signed long showValueStatusCO2;

//●ダミー素材の定義
//dummy value
const char NONES[] PROGMEM= "";
const char** DUMMY = NULL;

//表示素材の登録
struct UECSUserHtml U_html[U_HtmlLine]={
/*****
  {名前,
   入出力形式,
   単位,
   詳細説明,
   選択肢文字列,
   選択肢数,
   値,
   最小値,
   最大値,
   小数桁数
  }
*****/
  {NAME0,
   UECSSHOWDATA,
   UNIT0,
   NOTE0,
   DUMMY,
   0,
   &(showValueTemp),
   0,
   0,
   DECIMAL_DIGIT
  },
  {NAME1,
   UECSSELECTDATA,
   NONES,
   NOTE1,
   stringSELECT,
   4,
   &(setONOFFAUTO_Temp),
   0,
   0,
   DECIMAL_DIGIT_0},
  {NAME2,
   UECSINPUTDATA,
   UNIT2,
   NOTE2,
   DUMMY,
   0,
   &(setONTempFromWeb),
   -100,
   1000,
   DECIMAL_DIGIT},
  {NAME3,  UECSSHOWSTRING,NONES  ,NOTE3 ,TEMPstatus  ,3 , &(showValueStatusTemp)  , 0, 0, DECIMAL_DIGIT_0},
  {NAME4,  UECSSHOWDATA  ,UNIT4  ,NOTE4 ,DUMMY       ,           0 , &(showValueHumidity)  , 0, 0, DECIMAL_DIGIT_0},
  {NAME5,  UECSSELECTDATA,NONES  ,NOTE5 ,stringSELECT_Humidity , 3 , &(setONOFFAUTO_Humidity)  , 0, 0, DECIMAL_DIGIT_0},
  {NAME6,  UECSINPUTDATA ,UNIT6  ,NOTE6 ,DUMMY       ,0 , &(setONHumidityFromWeb) , 10, 100, DECIMAL_DIGIT_0},
  {NAME7,  UECSSHOWSTRING,NONES  ,NOTE7 ,stringSHOW_Humidity,    2 , &(showValueStatusHumidity)  , 0, 0, DECIMAL_DIGIT_0},
  {NAME8,  UECSSHOWDATA  ,UNIT8  ,NOTE8 ,DUMMY       ,0 , &(showValueRadiation)  , 0, 0, DECIMAL_DIGIT_0},
  {NAME9,  UECSSELECTDATA,NONES  ,NOTE9 ,stringSELECT_Radiation, 3 , &(setONOFFAUTO_Radiation)  , 0, 0, DECIMAL_DIGIT_0},
  {NAME10, UECSINPUTDATA ,UNIT10 ,NOTE10,DUMMY       ,0 , &(setONRadiationFromWeb) , 0, 1023, DECIMAL_DIGIT_0},
  {NAME11, UECSSHOWSTRING,NONES  ,NOTE11,stringSHOW_Radiation , 2 , &(showValueStatusRadiation)  , 0, 0, DECIMAL_DIGIT_0},
  {NAME12, UECSSHOWDATA  ,UNIT12 ,NOTE12,DUMMY       ,0 , &(showValueCO2)  , 0, 0, DECIMAL_DIGIT_0},
  {NAME13, UECSSELECTDATA,NONES  ,NOTE13,stringSELECT_CO2 , 3 , &(setONOFFAUTO_CO2)  , 0, 0, DECIMAL_DIGIT_0},
  {NAME14, UECSINPUTDATA ,UNIT14 ,NOTE14,DUMMY       ,0 , &(setONCO2FromWeb) , 0, 1023, DECIMAL_DIGIT_0},
  {NAME15, UECSSHOWSTRING,NONES  ,NOTE15,stringSHOW_CO2 , 2 , &(showValueStatusCO2)  , 0, 0, DECIMAL_DIGIT_0},
};

