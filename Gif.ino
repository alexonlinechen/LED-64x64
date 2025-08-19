
//佔記憶體14%

#include "GifPlayer.h"
GifPlayer gifPlayer;

void playGif() {

   Dir dir = fileSystem->openDir("/");

  while (dir.next()) {
       uint32_t result;
       String fileName = "/gifs/"+ GIFname;
      Serial.print(F("GIF NAME: "));
      Serial.println(GIFname);   

        File imageFile = fileSystem->open(fileName, "r");
           if (!imageFile) {
               Serial.println(F("打開檔案失敗"));           
                return;
                 }

        gifPlayer.setFile(imageFile);
       
 //單一GIF重複播放次數 gifcount
  for (uint8_t c=0; c<gifcount; c++) {
      if (!gifPlayer.parseGifHeader()) {
        imageFile.close();
        Serial.println(F("No gif header"));
        return;
      }

     // display.clearDisplay();       
      
      gifPlayer.parseLogicalScreenDescriptor();
      gifPlayer.parseGlobalColorTable();
      Serial.println(F("Processing gif"));
      do {
        gifPlayer.drawFrame();
        result = gifPlayer.drawFrame();
        
        delay(150);
     } while (result != ERROR_FINISHED);
      imageFile.seek(0);
    }

      
      Serial.println(F("GIF 播放完畢"));
      imageFile.close();
      delay(500);
      return;
      }

         }//end playgif



void randomGif() {

   Dir dir = fileSystem->openDir("/");

  while (dir.next()) {
       uint32_t result;
       GIF_R_name = random(1, random_gif_no);
      
       String fileName = "/gifs/"+ String(GIF_R_name) + ".gif" ;
      Serial.print(F("隨機 GIF NAME: "));
      Serial.print(GIF_R_name);   
      Serial.println(F(".gif"));


        File imageFile = fileSystem->open(fileName, "r");
           if (!imageFile) {
               Serial.println(F("打開檔案失敗"));           
                return;
                 }

        gifPlayer.setFile(imageFile);
       
 //單一GIF重複播放次數 gifcount
  for (uint8_t c=0; c<gifcount; c++) {
      if (!gifPlayer.parseGifHeader()) {
        imageFile.close();
        Serial.println(F("No gif header"));
        return;
      } 
      
      gifPlayer.parseLogicalScreenDescriptor();
      gifPlayer.parseGlobalColorTable();
      Serial.println(F("Processing gif"));
      do {
        gifPlayer.drawFrame();
        result = gifPlayer.drawFrame();
        
        delay(150);
     } while (result != ERROR_FINISHED);
      imageFile.seek(0);
    }

      
      Serial.println(F("GIF 播放完畢"));
      imageFile.close();
      delay(500);
      return;
      }

         }//end        
