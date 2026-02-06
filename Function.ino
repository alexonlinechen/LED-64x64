
uint16_t hsv2rgb(uint16_t hue, uint8_t saturation, uint8_t value)
{
    uint8_t red = 0;
    uint8_t green = 0;
    uint8_t blue = 0;
    uint16_t hi = (hue / 60) % 6;
    uint16_t F = 100 * hue / 60 - 100 * hi;
    uint16_t P = value * (100 - saturation) / 100;
    uint16_t Q = value * (10000 - F * saturation) / 10000;
    uint16_t T = value * (10000 - saturation * (100 - F)) / 10000;
    if(hue == 0){
      return display.color565(255*value/100, 255*value/100, 255*value/100);
    }
    switch (hi)
    {
    case 0:
        red = value;
        green = T;
        blue = P;
        break;
    case 1:
        red = Q;
        green = value;
        blue = P;
        break;
    case 2:
        red = P;
        green = value;
        blue = T;
        break;
    case 3:
        red = P;
        green = Q;
        blue = value;
        break;
    case 4:
        red = T;
        green = P;
        blue = value;
        break;
    case 5:
        red = value;
        green = P;
        blue = Q;
        break;
    default:
        return display.color565(255, 0, 0);
    }
    red = red * 255 / 100;
    green = green * 255 / 100;
    blue = blue * 255 / 100;
    return display.color565(red, green, blue);
}

    //畫X線 
void drawFastXLine(int16_t x, int16_t y, int16_t h, uint16_t c){
  for(int i=x;i<x+h;i++){
    display.drawPixel(i,y,c);
  }
}

void drawFastYLine(int16_t x, int16_t y, int16_t h, int16_t c){
  for(int i=y;i<y+h;i++){
    display.drawPixel(x,i,c);
  }
}

void showbigbitnumber(int number, int xlength, int ylength, int x, int y, uint16_t colorxy){
  String numStr = String(number);
  if(number<10){
    showbitmap(bitdata30[(int)(0)],xlength,ylength, x, y, colorxy);
    showbitmap(bitdata30[(int)((String(numStr.charAt(0)).toInt() + 1) - 1)],xlength,ylength, x+14, y, colorxy);
  }else if(number<100){
    showbitmap(bitdata30[(int)((String(numStr.charAt(0)).toInt() + 1) - 1)],xlength,ylength, x, y, colorxy);
    showbitmap(bitdata30[(int)((String(numStr.charAt(1)).toInt() + 1) - 1)],xlength,ylength, x+14, y, colorxy);
  }
}


//7x12
void showbit12number(int number, int xlength, int ylength, int x, int y, uint16_t colorxy){
  String numStr = String(number);
  if(number<10){
    showbitmap(bitdata12[(int)(0)],xlength,ylength, x, y, colorxy);
    showbitmap(bitdata12[(int)((String(numStr.charAt(0)).toInt() + 1) - 1)],xlength,ylength, x+9, y, colorxy);
  }else if(number<100){
    showbitmap(bitdata12[(int)((String(numStr.charAt(0)).toInt() + 1) - 1)],xlength,ylength, x, y, colorxy);
    showbitmap(bitdata12[(int)((String(numStr.charAt(1)).toInt() + 1) - 1)],xlength,ylength, x+9, y, colorxy);
  }
}





void showbitmapWeek(int number, int xlength, int ylength, int x, int y, uint16_t colorxy){
  String numStr = String(number);
    //showbitmap(weekdata[(int)(0)],xlength,ylength, x, y, colorxy);
    showbitmap(weekdata[(int)((String(numStr.charAt(0)).toInt() + 1) - 1)],xlength,ylength, x, y, colorxy);
}


void showbitmap(String bitrgbstr, int xlength, int ylength, int x, int y, uint16_t colorxy) {
  //Serial.println("bitrgbstr = " + bitrgbstr);
  for (int i = x; i < x+(xlength); i = i + (1)) {
    for(int j = y; j < y+(ylength); j = j + (1)){
      if (String(bitrgbstr.charAt(((j-y)*xlength+i-x))).toInt() != 0) {
        display.drawPixel(i,j,colorxy);
      } else {
        display.drawPixel(i,j,display.color565( 0, 0, 0));
      }
    }
  }
  }


  //大冒號
void showbigColon(int x,int y,bool l,uint16_t colorxy){
    if(l){
      drawFastXLine(x,y,2,colorxy);
      drawFastXLine(x,y+1,2,colorxy);
     // display.drawPixel(x,y,colorxy);
    }else{
      //display.drawPixel(x,y,display.color565(0, 0, 0));
      drawFastXLine(x,y,2,display.color565(0, 0, 0));
      drawFastXLine(x,y+1,2,display.color565(0, 0, 0));
    }
}


//設定wifi啟動
  void setwifi(){
    
  WiFi.mode(WIFI_STA);
  dataType d;
  EEPROM_readAnything(90, d);
  ssid = d.testssid;
  password = d.testpass;
  Serial.print("SSID: ");
  Serial.println(ssid);
  Serial.print("Password: ");
  Serial.println(password);
  WiFi.begin(ssid, password);

  //如果wifi 沒有連接則 啟動AP
  if (testWifi() == false) {   
     Serial.println(F("啟動AP模式 "));
     setupAP();
     return;   }
  
  }


  bool testWifi(void) {
  int c = 0;
  Serial.println(F("等待 Wifi 連接"));
  while ( c <= 10 ) {
  if (WiFi.status() == WL_CONNECTED) { 
  Serial.print(F("本機 IP: "));
  Serial.println(WiFi.localIP());
  return true;   }
      
  Serial.print(WiFi.status());  
  delay(1000);
  Serial.print(".");
  c++;
  }
  Serial.println("");
  Serial.println(F("無法連接WIFI, 啟動AP模式"));
  return false;
  }




//設定WIFI為AP模式
  void setupAP() {

 
  WiFi.disconnect();
  delay(1000);
  const char* APssid = "64x64數字時鐘192.168.4.1";        
  const char* APpassword = "";  
  WiFi.mode(WIFI_AP);
  WiFi.softAP(APssid, APpassword);    // IP is usually 192.168.4.1
  Serial.println();
  Serial.print(F("SoftAP IP: "));
  Serial.println(WiFi.softAPIP());
  launchWeb();
  Serial.print(F("SoftAP IP: "));
  Serial.println(WiFi.softAPIP());
 }


  //保存ssid数据
  void save_ssid()            
  {
  dataType d; // 宣告結構變數，放入資料
  strcpy(d.testssid, setssid.c_str());
  strcpy(d.testpass, setpwd.c_str());
  EEPROM_writeAnything(90, d);
  Serial.println(F(" 寫入資料中"));
  Serial.println(F("---------------"));
  }


 //读取ssid数据
 void read_ssid()                                
 {
 dataType d;
 EEPROM_readAnything(90, d);
 Serial.println(F(" 讀取資料中"));

 Serial.print(F("SSID: "));
 Serial.println(d.testssid);
 Serial.print(F("Password: "));
 Serial.println(d.testpass);
  }



  void TimeCheck(){

    time(&now);
    localtime_r(&now, &tm);
    H = tm.tm_hour;
    M = tm.tm_min;
    S = tm.tm_sec;
    D = tm.tm_wday;
    currentMonth = tm.tm_mon + 1 ;  //取得月份
    monthDay = tm.tm_mday;          //取得日期


     //定時顯示/關閉 時鐘模式
    if(runonce == true){     
      if(H == end_H && M == end_M ){
             Serial.println(F(" 關閉時鐘"));
             Mode = 99;
             display.clearDisplay();   
             runonce = false;
             }
         }

      if(runonce == false){       
         if(H == start_H && M == start_M){
             Serial.println(F(" 開啟時鐘")); 
             Mode = EEPROM.read(EEPROM_MODE);
             runonce = true;
             }
          }   
  }


void ShowIP() {
  //顯示已連線IP 
  String myip = WiFi.localIP().toString().c_str();
  display.clearDisplay();   
  display.setTextColor(display.color565(255,0,0));  //預設文字顏色
  display.setCursor(3, 28);
  display.print(myip);    
  delay(5000);
  display.clearDisplay();  
}


void savedata() {
  
    Serial.print(F("時鐘模式: ")); 
    Serial.println(Mode); 
    Serial.print(F("亮度: ")); 
    Serial.println(brightnessNow); 
    
    Serial.print(F("隨機時鐘模式-間隔(分): "));
    Serial.println(random_min); 
          
    Serial.print(F("GIF重複次數: ")); 
    Serial.println(gifcount); 
    
    Serial.print(F("GIF隨機播放數量: ")); 
    Serial.println(random_gif_no); 

    Serial.print(F("GIF延遲: ")); 
    Serial.println(gifdelay);     
    
}
