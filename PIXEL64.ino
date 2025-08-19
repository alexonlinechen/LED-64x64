#include "Config.h"

#include <PxMatrix.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPUpdateServer.h>


const char* ssid ;
const char* password ;

#include <Ticker.h>
Ticker display_ticker;
#define P_LAT 16
#define P_A 5
#define P_B 4
#define P_C 15
#define P_D 12
#define P_E 0
#define P_OE 2

PxMATRIX display(64,64,P_LAT, P_OE,P_A,P_B,P_C,P_D);
//PxMATRIX display(64,64,P_LAT, P_OE,P_A,P_B,P_C,P_D,P_E);


// ISR for display refresh
void display_updater()
{
display.display(10);
}


void setup() {

  Serial.begin(115200);
  EEPROM.begin(EEPROM_SIZE);
    brightnessNow = EEPROM.read(EEPROM_BRIGHTNESS);
    Mode = EEPROM.read(EEPROM_MODE);
    hue = int(EEPROM.read(EEPROM_COLOR))*2;
    hueh = int(EEPROM.read(EEPROM_COLOR_H))*2;
    huem = int(EEPROM.read(EEPROM_COLOR_M))*2;
    hues = int(EEPROM.read(EEPROM_COLOR_S))*2;
    huew = int(EEPROM.read(EEPROM_COLOR_W))*2;

    read_start_H = int(EEPROM.read(EEPROM_START_H));
    read_start_M = int(EEPROM.read(EEPROM_START_M));
    read_end_H = int(EEPROM.read(EEPROM_END_H));
    read_end_M = int(EEPROM.read(EEPROM_END_M));
    start_H = int(EEPROM.read(EEPROM_START_H));
    start_M = int(EEPROM.read(EEPROM_START_M));
    end_H = int(EEPROM.read(EEPROM_END_H));
    end_M = int(EEPROM.read(EEPROM_END_M));
    
    gifcount = int(EEPROM.read(EEPROM_GIF_COUNT));
    random_gif_no = int(EEPROM.read(EEPROM_GIF_NO));
    gifdelay = int(EEPROM.read(EEPROM_GIF_DELAY));
    random_min = int(EEPROM.read(EEPROM_RANDOM_MIN));

  //設定WIFI 優先,如果ssid 密碼不正確　自動啟動AP模式 
  setwifi(); 

  server.begin();
  fssetup();
  launchWeb();
  delay(1000);
  configTime(MY_TZ, MY_NTP_SERVER); 

  display.begin(32);
  display.setFastUpdate(true);
  display.setBrightness(brightnessNow);
  display.setColorOrder(RRBBGG);
  display_ticker.attach(0.004, display_updater);

  ShowIP(); //顯示IP
  


  delay(1000);
  GetTime();
  savedata();
}


void loop() {
server.handleClient();   
SwitchMode();
TimeCheck();

}
