

  //設定web各項功能
  void launchWeb(){
    
    httpUpdateServer.setup(&server);

    server.on("/bigclock", HTTP_POST, []() {  
    display.clearDisplay();   
    Mode = 1;
    Serial.println(F("時間模式")); 
    });  
    
    server.on("/weekclock", HTTP_POST, []() {  
    display.clearDisplay();   
    Mode = 2;
    Serial.println(F("時間 + 日期模式")); 
    });  

    server.on("/gifclock", HTTP_POST, []() {   
    display.clearDisplay();   
    Mode = 3;
    Serial.println(F("時間 + Gif模式")); 
     });

    server.on("/gifbigclock", HTTP_POST, []() {   
    display.clearDisplay();   
    Mode = 4;
    Serial.println(F("大時間 + Gif模式")); 
     });

    server.on("/pacmanmode", HTTP_POST, []() {  
    display.clearDisplay();   
    ModefirstRun = true;  
    Mode = 5;
    Serial.println(F("Pacman 小精靈模式")); 
    server.send(200, "text/plain", "OK");
    });


    server.on("/Tetrisclock", HTTP_POST, []() {  
    display.clearDisplay();   
    ModefirstRun = true;  
    Mode = 6;
    Serial.println(F("俄羅斯方塊模式")); 
    server.send(200, "text/plain", "OK");
    });

     server.on("/Marioclock", HTTP_POST, []() {  
    display.clearDisplay();   
    ModefirstRun = true;  
    Mode = 7;
    Serial.println(F("瑪利歐模式")); 
    server.send(200, "text/plain", "OK");
    });  

    server.on("/randommode", HTTP_POST, []() {   
    display.clearDisplay();  
    Mode = 8; 
    Serial.println(F("隨機模式")); 
    }); 


    server.on("/gifmode", HTTP_POST, []() {   
    display.clearDisplay(); 
    Mode = 99;  
    playGif();
    Serial.println(F("Gif模式")); 
    }); 


    server.on("/clear", HTTP_POST, []() {   
    display.clearDisplay();   
    }); 


    server.on("/reset", HTTP_POST, []() {   
    ESP.reset(); 
    }); 


    server.on("/savedata", HTTP_POST, []() {   

         EEPROM.write(EEPROM_MODE, Mode);
         EEPROM.write(EEPROM_COLOR, hue/2);
         EEPROM.write(EEPROM_COLOR_H, hueh/2);
         EEPROM.write(EEPROM_COLOR_M, huem/2);
         EEPROM.write(EEPROM_COLOR_S, hues/2);
         EEPROM.write(EEPROM_COLOR_W, huew/2);
         EEPROM.write(EEPROM_BRIGHTNESS, brightnessNow);
         
         EEPROM.write(EEPROM_START_H, start_H);
         EEPROM.write(EEPROM_START_M, start_M);
         EEPROM.write(EEPROM_END_H, end_H);
         EEPROM.write(EEPROM_END_M, end_M);
         
         EEPROM.write(EEPROM_GIF_NO, random_gif_no);
         EEPROM.write(EEPROM_GIF_COUNT, gifcount);
         EEPROM.write(EEPROM_GIF_DELAY, gifdelay);
         EEPROM.write(EEPROM_RANDOM_MIN, random_min);

         
         EEPROM.commit();
         Serial.println(F("儲存資料完成")); 
         savedata();
    }); 



      //解析顏色數值
     server.on("/getColor", HTTP_POST, []() {   
     
     //str轉char
    String input = server.arg("value");
     strcpy(socketMessage,input.c_str());
    
         
      if(socketMessage[0] == 'h'){//颜色设置
                int hh= ((int)socketMessage[2]-48)*100+((int)socketMessage[3]-48)*10+(int)socketMessage[4]-48;
                Serial.print(F("傳入色值："));
                Serial.println(hh);
                if(hh>=0&&hh<=360){  
                    switch (socketMessage[1])
                    {
                     
                    case 'a':
                        hue = hh;
                        hueh = hh;
                        huem = hh;
                        hues = hh;
                        huew = hh;
                        hueb = hh;
                        break;
                        
                    case 'b':
                        brightnessNow = hh ;
                        display.setBrightness(brightnessNow);
                        break; 
                                                
                    case 'd':
                        hue = hh;
                        break;
                    case 'h':
                        hueh = hh;
                        break;
                    case 'm':
                        huem = hh;
                        break;
                    case 's':
                        hues = hh;
                        break;   
                    case 'w':
                        huew = hh;
                        break;  
                                                                        
                    case 'o':   
                        start_H = hh;
                        Serial.println(start_H);
                        break;
                        
                    case 'p':
                        start_M = hh;
                          Serial.println(start_M);
                        break;
                        
                    case 'q':
                        end_H = hh;
                          Serial.println(end_H);
                        break;
                        
                    case 'r':
                        end_M = hh;
                         Serial.println(end_M);
                        break;

                    case 'g':
                        gifcount = hh;
                        Serial.print(F("設定Gif播放次數: ")); 
                        Serial.println(gifcount);
                        break; 

                    case 'n':
                        random_gif_no = hh;
                        Serial.print(F("設定隨機Gif數量: ")); 
                        Serial.println(random_gif_no);
                        break;                         

                    case 'u':
                        random_min = hh;
                        Serial.print(F("隨機模式切換時間: "));
                        Serial.println(random_min);
                        break;
                           
                    case 'y':
                        gifdelay = hh;
                        Serial.print(F("gif延遲時間: "));
                        Serial.println(gifdelay);                        
                        break;                             
                                                                                      
                    default:
                        break;
                    }
                }
          }
     });  


    //設定播放GIF 檔名
    server.on("/gifname", HTTP_POST, []() {  
    display.clearDisplay();   
    Mode = 99;        
    GIFname = server.arg("gifname");
    display.print(GIFname);
    playGif();
     }); 


    //Gif 固定播放
    server.on("/fixedgif", HTTP_POST, []() {        
 
    Serial.println(F("固定播放-GIF"));
    GifRandom = false ;
     }); 


    //Gif 隨機播放
    server.on("/randomgif", HTTP_POST, []() {        
   
    Serial.println(F("GIF隨機播放"));
    GifRandom = true ;
     }); 


    //web  wifi功能
    server.on("/setwifi", HTTP_POST, []() {   
    
    setssid = server.arg("hh2");
    setpwd = server.arg("mm2");    
    delay(500);
    save_ssid();
    delay(500);
   // read_ssid();
    server.send(200, "text/json", "{\"result\":\"ok\"}");
    
          display.clearDisplay();
          display.setCursor(20, 8);
          display.print(F("Saved"));
          delay(1000); 
          display.setCursor(14, 32);
          display.print(F("RE-BOOT"));
          delay(4000);    
          ESP.reset();
    }); 





      //Web 顯示已設定Wifi數值
     server.on("/readssid", HTTP_POST, []() {   
     dataType d;
     EEPROM_readAnything(90, d);
     String readssid = d.testssid;
     server.send(200, "text/json", readssid); 
     });    






       //Web 顯示已設定 開始/結束  時間數值
     server.on("/readtime", HTTP_POST, []() {   

       String one ="{ \"startH\":";
       String two = ",\"startM\":";
       String three = ",\"endH\":";
       String four = ",\"endM\":";
       String five = "}";          
       String JSON = one + read_start_H + two + read_start_M + three + read_end_H + four+  read_end_M + five;
  
      server.send(200, "text/json", JSON); 
     });   
    
    
  }
