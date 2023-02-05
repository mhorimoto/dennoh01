/////////////////////////////////////
//IP reset jupmer pin setting
//IPアドレスリセット用ジャンパーピン設定
/////////////////////////////////////
//Pin ID. This pin is pull-upped automatically.
//ピンIDを入力、このピンは自動的にプルアップされます
//ピンIDは変更可能です
const byte U_InitPin = SW_SAFE;
const byte U_InitPin_Sense=LOW;
//When U_InitPin status equals this value,IP address is set "192.168.38.7".
//(This is added Ver0.3 over)

////////////////////////////////////
//Node basic infomation
//ノードの基本情報
///////////////////////////////////

const char U_name[] PROGMEM= "M304jp Node v2.14\0";//MAX 20 chars
const char U_vender[] PROGMEM= "HOLLY&Co.Ltd.\0";//MAX 20 chars
const char U_uecsid[] PROGMEM= "10100C00000B\0";//12 chars fixed
const char U_footnote[] PROGMEM= "M304jp UARDECS version\0";
//const int U_footnoteLetterNumber = 48;//Abolished after Ver 0.6

//Total number of HTML table rows.
//web設定画面で表示すべき項目の総数
const int U_HtmlLine = 16;

//●表示素材の定義(1)数値表示
//UECSSHOWDATA
const char NAME0[] PROGMEM= "Temperature";
const char UNIT0[] PROGMEM= "C";
const char NOTE0[] PROGMEM= "SHOWDATA";
//表示用の値を格納する変数
//小数桁数が1の場合、123が12.3と表示される
signed long showValueTemp;

//●表示素材の定義(2)選択肢表示
//UECSSELECTDATA
const char NAME1[] PROGMEM= "UserSwitch";
const char NOTE1[] PROGMEM= "SELECTDATA";
const char UECSOFF[] PROGMEM= "OFF";
const char UECSON[] PROGMEM= "ON";
const char UECSAUTO[] PROGMEM= "AUTO";
const char *stringSELECT[3]={
  UECSOFF,
  UECSON,
  UECSAUTO,
};

//入力された選択肢の位置を受け取る変数
//UECSOFFが0、UECSONで1、UECSAUTOで2になる
signed long setONOFFAUTO_Temp;

//●表示素材の定義(3)数値入力
//UECSINPUTDATA
const char NAME2[] PROGMEM= "SetTemp";
const char UNIT2[] PROGMEM= "C";
const char NOTE2[] PROGMEM= "INPUTDATA";

//入力された数値を受け取る変数
//小数桁数が1の場合、例えばWeb上で12.3が入力されると123が代入される
signed long setONTempFromWeb;

//●表示素材の定義(4)文字表示
//UECSSHOWSTRING
const char NAME3[] PROGMEM= "Now status";
const char NOTE3[] PROGMEM= "SHOWSTRING";
const char SHOWSTRING_OFF[] PROGMEM= "OUTPUT:OFF";
const char SHOWSTRING_ON [] PROGMEM= "OUTPUT:ON";
const char *stringSHOW[2]={
  SHOWSTRING_OFF,
  SHOWSTRING_ON,
};
signed long showValueStatusTemp;

//●表示素材の定義(1)数値表示
//UECSSHOWDATA
const char NAME4[] PROGMEM= "Humidity";
const char UNIT4[] PROGMEM= "%";
const char NOTE4[] PROGMEM= "SHOWDATA";

//表示用の値を格納する変数
//小数桁数が1の場合、123が12.3と表示される
signed long showValueHumidity;

//●表示素材の定義(2)選択肢表示
//UECSSELECTDATA
const char NAME5[] PROGMEM= "UserSwitch";
const char NOTE5[] PROGMEM= "SELECTDATA";
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
const char NAME6[] PROGMEM= "SetHumidity";
const char UNIT6[] PROGMEM= "%";
const char NOTE6[] PROGMEM= "INPUTDATA";

//入力された数値を受け取る変数
//小数桁数が1の場合、例えばWeb上で12.3が入力されると123が代入される
signed long setONHumidityFromWeb;

//●表示素材の定義(4)文字表示
//UECSSHOWSTRING
const char NAME7[] PROGMEM= "Now status";
const char NOTE7[] PROGMEM= "SHOWSTRING";
const char *stringSHOW_Humidity[2]={
  SHOWSTRING_OFF,
  SHOWSTRING_ON,
};
signed long showValueStatusHumidity;

//●表示素材の定義(1)数値表示
//UECSSHOWDATA
const char NAME8[] PROGMEM= "Illuminance";
const char UNIT8[] PROGMEM= "";
const char NOTE8[] PROGMEM= "SHOWDATA";

//表示用の値を格納する変数
//小数桁数が1の場合、123が12.3と表示される
signed long showValueIlluminance;

//●表示素材の定義(2)選択肢表示
//UECSSELECTDATA
const char NAME9[] PROGMEM= "UserSwitch";
const char NOTE9[] PROGMEM= "SELECTDATA";
const char *stringSELECT_Illuminance[3]={
  UECSOFF,
  UECSON,
  UECSAUTO,
};

//入力された選択肢の位置を受け取る変数
//UECSOFFが0、UECSONで1、UECSAUTOで2になる
signed long setONOFFAUTO_Illuminance;

//●表示素材の定義(3)数値入力
//UECSINPUTDATA
const char NAME10[] PROGMEM= "SetIlluminance";
const char UNIT10[] PROGMEM= "";
const char NOTE10[] PROGMEM= "INPUTDATA";

//入力された数値を受け取る変数
//小数桁数が1の場合、例えばWeb上で12.3が入力されると123が代入される
signed long setONIlluminanceFromWeb;

//●表示素材の定義(4)文字表示
//UECSSHOWSTRING
const char NAME11[] PROGMEM= "Now status";
const char NOTE11[] PROGMEM= "SHOWSTRING";
const char *stringSHOW_Illuminance[2]={
  SHOWSTRING_OFF,
  SHOWSTRING_ON,
};
signed long showValueStatusIlluminance;

//●表示素材の定義(1)数値表示
//UECSSHOWDATA
const char NAME12[] PROGMEM= "REncoder";
const char UNIT12[] PROGMEM= "";
const char NOTE12[] PROGMEM= "SHOWDATA";

//表示用の値を格納する変数
//小数桁数が1の場合、123が12.3と表示される
signed long showValueREncoder;

//●表示素材の定義(2)選択肢表示
//UECSSELECTDATA
const char NAME13[] PROGMEM= "UserSwitch";
const char NOTE13[] PROGMEM= "SELECTDATA";
const char *stringSELECT_REncoder[3]={
  UECSOFF,
  UECSON,
  UECSAUTO,
};

//入力された選択肢の位置を受け取る変数
//UECSOFFが0、UECSONで1、UECSAUTOで2になる
signed long setONOFFAUTO_REncoder;

//●表示素材の定義(3)数値入力
//UECSINPUTDATA
const char NAME14[] PROGMEM= "SetREncoder";
const char UNIT14[] PROGMEM= "";
const char NOTE14[] PROGMEM= "INPUTDATA";

//入力された数値を受け取る変数
//小数桁数が1の場合、例えばWeb上で12.3が入力されると123が代入される
signed long setONREncoderFromWeb;

//●表示素材の定義(4)文字表示
//UECSSHOWSTRING
const char NAME15[] PROGMEM= "Now status";
const char NOTE15[] PROGMEM= "SHOWSTRING";
const char *stringSHOW_REncoder[2]={
  SHOWSTRING_OFF,
  SHOWSTRING_ON,
};
signed long showValueStatusREncoder;

//●ダミー素材の定義
//dummy value
const char NONES[] PROGMEM= "";
const char** DUMMY = NULL;

//表示素材の登録
struct UECSUserHtml U_html[U_HtmlLine]={
//{名前,入出力形式  ,単位 ,詳細説明,選択肢文字列  ,選択肢数,値     ,最小値,最大値,小数桁数}
  {NAME0, UECSSHOWDATA  ,UNIT0  ,NOTE0  , DUMMY   , 0 , &(showValueTemp)  , 0, 0, DECIMAL_DIGIT},
  {NAME1, UECSSELECTDATA  ,NONES  ,NOTE1  , stringSELECT  , 3 , &(setONOFFAUTO_Temp)  , 0, 0, DECIMAL_DIGIT_0},
  {NAME2, UECSINPUTDATA ,UNIT2  ,NOTE2  , DUMMY   , 0 , &(setONTempFromWeb) , 100, 1000, DECIMAL_DIGIT},
  {NAME3, UECSSHOWSTRING  ,NONES  ,NOTE3  , stringSHOW  , 2 , &(showValueStatusTemp)  , 0, 0, DECIMAL_DIGIT_0},
  {NAME4, UECSSHOWDATA  ,UNIT4  ,NOTE4  , DUMMY   , 0 , &(showValueHumidity)  , 0, 0, DECIMAL_DIGIT_0},
  {NAME5, UECSSELECTDATA  ,NONES  ,NOTE5  , stringSELECT_Humidity , 3 , &(setONOFFAUTO_Humidity)  , 0, 0, DECIMAL_DIGIT_0},
  {NAME6, UECSINPUTDATA ,UNIT6  ,NOTE6  , DUMMY   , 0 , &(setONHumidityFromWeb) , 10, 100, DECIMAL_DIGIT_0},
  {NAME7, UECSSHOWSTRING  ,NONES  ,NOTE7  , stringSHOW_Humidity , 2 , &(showValueStatusHumidity)  , 0, 0, DECIMAL_DIGIT_0},
  {NAME8, UECSSHOWDATA  ,UNIT8  ,NOTE8  , DUMMY   , 0 , &(showValueIlluminance)  , 0, 0, DECIMAL_DIGIT_0},
  {NAME9, UECSSELECTDATA  ,NONES  ,NOTE9  , stringSELECT_Illuminance , 3 , &(setONOFFAUTO_Illuminance)  , 0, 0, DECIMAL_DIGIT_0},
  {NAME10, UECSINPUTDATA ,UNIT10  ,NOTE10, DUMMY   , 0 , &(setONIlluminanceFromWeb) , 0, 1023, DECIMAL_DIGIT_0},
  {NAME11, UECSSHOWSTRING  ,NONES  ,NOTE11  , stringSHOW_Illuminance , 2 , &(showValueStatusIlluminance)  , 0, 0, DECIMAL_DIGIT_0},
  {NAME12, UECSSHOWDATA  ,UNIT12  ,NOTE12  , DUMMY   , 0 , &(showValueREncoder)  , 0, 0, DECIMAL_DIGIT_0},
  {NAME13, UECSSELECTDATA  ,NONES  ,NOTE13  , stringSELECT_REncoder , 3 , &(setONOFFAUTO_REncoder)  , 0, 0, DECIMAL_DIGIT_0},
  {NAME14, UECSINPUTDATA ,UNIT14  ,NOTE14  , DUMMY   , 0 , &(setONREncoderFromWeb) , 0, 1023, DECIMAL_DIGIT_0},
  {NAME15, UECSSHOWSTRING  ,NONES  ,NOTE15  , stringSHOW_REncoder , 2 , &(showValueStatusREncoder)  , 0, 0, DECIMAL_DIGIT_0},
};

