

void SwitchMode(){
 switch(Mode){
      case 1:
            ClockMode1();
      break; 
      case 2:
            ClockMode2();      
      break;       
      case 3:
            ClockMode3();      
      break;        
      case 4:
            ClockMode4();      
      break;
      case 5:
            ClockMode5();      
      break;                       
      }    
    } 


void GetTime(){

    time(&now);
    localtime_r(&now, &tm);
    H = tm.tm_hour;
    M = tm.tm_min;
    S = tm.tm_sec;
    D = tm.tm_wday;
    currentMonth = tm.tm_mon + 1 ;  //取得月份
    monthDay = tm.tm_mday;          //取得日期

 Serial.print(F("時間: ")); 
  Serial.print(H); 
   Serial.print(F(":")); 
     Serial.println(M); 
   }

/// ####  時鐘顯示模式   ####


//时钟模式一  HH:mm
void ClockMode1(){        


if ( millis() - last_H_Time > 60000){   //間隔20秒顯示一次 避免閃爍
GetTime();


//時間H   變化/動畫效果
   if( tempH != H){
   for(int i=-20;i<21;i++){
      display.fillRect(0,0,64,20,display.color565(0,0,0));
      showbigbitnumber(H,12,20,2,i,hsv2rgb(hueh, saturation, value)); 
      delay(3);
      tempH = H;    
         }
      }    
      
     showbigbitnumber(tempH,12,20,2,20,hsv2rgb(hueh, saturation, value));  
     last_H_Time = millis();
  } 

  
  if ( millis() - colonTime > 1000 )
  {
    colon = !colon;
    colonTime = millis();
  }
  showbigColon(31,25,colon,hsv2rgb(hue, saturation, value));
  showbigColon(31,34,colon,hsv2rgb(hue, saturation, value));
      


if ( millis() - last_M_Time > 25000){ 
  
//時間M   變化/動畫效果
 if( tempM != M){
   for(int i=-20;i<21;i++){
      display.fillRect(0,0,64,20,display.color565(0,0,0));
      showbigbitnumber(M,12,20,36,i,hsv2rgb(huem, saturation, value));  
      delay(3);
      tempM = M;
           }
          }
     showbigbitnumber(tempM,12,20,36,20,hsv2rgb(huem, saturation, value));
     last_M_Time = millis();
   
           }      
         }





//时钟模式一  HH:mm + 日期 
void ClockMode2(){        


if ( millis() - last_H_Time > 60000){   //間隔20秒顯示一次 避免閃爍
GetTime();


//時間H   變化/動畫效果
   if( tempH != H){
   for(int i=-20;i<7;i++){
      display.fillRect(0,0,64,6,display.color565(0,0,0));
      showbigbitnumber(H,12,20,2,i,hsv2rgb(hueh, saturation, value)); 
      delay(3);
      tempH = H;    
         }
      }    
      
     showbigbitnumber(tempH,12,20,2,6,hsv2rgb(hueh, saturation, value));  
     last_H_Time = millis();
  } 

  
  if ( millis() - colonTime > 1000 )
  {
    colon = !colon;
    colonTime = millis();
  }
  showbigColon(31,11,colon,hsv2rgb(hue, saturation, value));
  showbigColon(31,20,colon,hsv2rgb(hue, saturation, value));
      


if ( millis() - last_M_Time > 25000){ 
  
//時間M   變化/動畫效果
 if( tempM != M){
   for(int i=-20;i<7;i++){
      display.fillRect(0,0,64,6,display.color565(0,0,0));
      showbigbitnumber(M,12,20,36,i,hsv2rgb(huem, saturation, value));  
      delay(3);
      tempM = M;
           }
          }
     showbigbitnumber(tempM,12,20,36,6,hsv2rgb(huem, saturation, value));
     last_M_Time = millis();
 


  //顯示 日期 星期
  display.fillRect(0,33,64,64,display.color565(0,0,0));

   showbit12number(currentMonth,7,12,2,35,hsv2rgb(hueh, saturation, value));  // 數字月
   showbitmapWeek(9,9,12,22,35,hsv2rgb(huew, saturation, value));   //月
   showbit12number(monthDay,7,12,35,35,hsv2rgb(huem, saturation, value));    // 數字日
   showbitmapWeek(0,9,12,54,35,hsv2rgb(huew, saturation, value));   //日

   showbitmapWeek(7,9,12,14,50,hsv2rgb(huew, saturation, value));   //星
   showbitmapWeek(8,9,12,26,50,hsv2rgb(huew, saturation, value));   //期
   
   if(D == 6) showbitmapWeek(D,9,12,40,50,display.color565(0,255,0));  
   else if (D == 0) showbitmapWeek(D,9,12,40,50,display.color565(255,0,0)); 
   else showbitmapWeek(D,9,12,40,50,hsv2rgb(hueh, saturation, value));  
  
   
 } 
          
}





//时钟模式  HH:mm  + GIF
void ClockMode3(){        


//間隔30秒顯示時間
if ( millis() - lastTime > 30000){  

      GetTime();
     //清空時鐘顯示區域
     display.fillRect(0,0,64,14,display.color565(0,0,0));

     showbit12number(H,7,12,14,1,hsv2rgb(hueh, saturation, value));   
     display.drawPixel(32,4,hsv2rgb(hue, saturation, value));
     display.drawPixel(32,8,hsv2rgb(hue, saturation, value));
     showbit12number(M,7,12,35,1,hsv2rgb(huem, saturation, value)); 
        
         GifClock = true ;    //  啟用 讓Gif 配合時間  向下偏移 

          if(GifRandom)  randomGif();
          else playGif();  
          
         GifClock = false ;
         lastTime = millis();
       }   
}



//大时钟模式  HH:mm  + GIF
void ClockMode4(){        


//間隔30秒顯示時間
if ( millis() - lastTime > 30000){  

      GetTime();
     //清空時鐘顯示區域
     display.fillRect(0,0,64,14,display.color565(0,0,0));

     showbit12number(H,7,12,14,1,hsv2rgb(hueh, saturation, value));   
     display.drawPixel(32,4,hsv2rgb(hue, saturation, value));
     display.drawPixel(32,8,hsv2rgb(hue, saturation, value));
     showbit12number(M,7,12,35,1,hsv2rgb(huem, saturation, value)); 
        
         GifClock = true ;    //  啟用 讓Gif 配合時間  向下偏移 

          if(GifRandom)  randomGif();
          else playGif();  
          
         GifClock = false ;

      delay(6000);
      display.clearDisplay();
      showbigbitnumber(H,12,20,2,20,hsv2rgb(hueh, saturation, value)); 
      showbigbitnumber(M,12,20,36,20,hsv2rgb(huem, saturation, value));
      drawFastXLine(31,25,2,hsv2rgb(hue, saturation, value));
      drawFastXLine(31,34,2,hsv2rgb(hue, saturation, value));
       
      delay(10000); 
   
         lastTime = millis();
       }   
}




 //隨機模式
void ClockMode5(){

  switch(randomMode){
      case 1:
            ClockMode1();
      break; 
      case 2:
            ClockMode2();
      break; 
      case 3:     
           ClockMode3();    
      break;
      
      case 4:     
           ClockMode4();    
      break;                     
      default:
           ClockMode1();
      } 
      
if ( millis() - randomTime > random_min*60000){    //間隔 - 分鐘顯示時間

   randomMode = random(1, 5);
   Serial.print(F("切換隨機模式："));
   Serial.println(randomMode);
   display.clearDisplay();
   randomTime = millis();
 } 
  
}
