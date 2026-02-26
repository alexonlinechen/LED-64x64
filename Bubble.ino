

#define MAX_AIRS 10
struct AirBubble {
  int x;
  int y;
  bool active;
  bool isPink; // 新增：標記是否為 2P 的粉紅氣泡
  unsigned long lastMoveTime;
};

AirBubble airs[MAX_AIRS]; 
unsigned long lastAirSpawnTime = 0;


// 氣泡圖案 (0=黑, 1=藍 2=白)
const uint8_t PROGMEM AIR[7][7] = {
  0, 0, 1, 1, 1, 0, 0,
  0, 1, 2, 2, 2, 1, 0,
  1, 2, 2, 2, 2, 2, 1,
  1, 2, 2, 2, 2, 2, 1,
  1, 2, 2, 2, 2, 2, 1,
  0, 1, 2, 2, 2, 1, 0,
  0, 0, 1, 1, 1, 0, 0
};



// 泡泡龍圖案 (0=黑, 1=黃 2=綠2 3=白)
const uint8_t PROGMEM Bubble[16][16] = {
  0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 
  0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 
  0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 2, 0, 0, 0, 0, 
  0, 0, 0, 0, 1, 1, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 
  0, 0, 0, 0, 0, 2, 2, 2, 2, 3, 3, 2, 3, 2, 0, 0, 
  0, 0, 1, 1, 1, 2, 2, 2, 3, 3, 0, 2, 0, 3, 0, 0, 
  0, 0, 0, 1, 2, 2, 2, 2, 3, 3, 0, 2, 0, 3, 0, 0, 
  0, 0, 0, 0, 2, 2, 2, 2, 3, 3, 0, 2, 0, 3, 2, 0, 
  0, 0, 1, 1, 2, 2, 2, 2, 3, 3, 0, 2, 0, 3, 2, 0, 
  0, 0, 0, 1, 2, 2, 2, 2, 2, 3, 3, 2, 3, 2, 2, 0, 
  0, 0, 0, 0, 2, 1, 2, 2, 0, 0, 0, 3, 0, 0, 0, 0, 
  0, 0, 0, 2, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 0, 0, 
  0, 0, 0, 2, 1, 1, 1, 2, 2, 3, 3, 3, 3, 0, 0, 0, 
  0, 0, 1, 2, 1, 1, 2, 2, 3, 3, 3, 3, 3, 3, 0, 0, 
  0, 1, 2, 2, 2, 2, 2, 1, 1, 1, 3, 3, 3, 3, 0, 0, 
  2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 3, 3, 1, 1, 1
};


const uint8_t PROGMEM Bubble2[16][16] = {
  0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 
  0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 
  0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 2, 0, 0, 0, 0, 
  0, 0, 0, 0, 1, 1, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 
  0, 0, 0, 0, 0, 2, 2, 2, 2, 3, 3, 2, 3, 2, 0, 0, 
  0, 0, 1, 1, 1, 2, 2, 2, 3, 3, 0, 2, 0, 3, 0, 0, 
  0, 0, 0, 1, 2, 2, 2, 2, 3, 3, 0, 2, 0, 3, 0, 0, 
  0, 0, 0, 0, 2, 2, 2, 2, 3, 3, 0, 2, 0, 3, 2, 0, 
  0, 0, 1, 1, 2, 2, 2, 2, 3, 3, 0, 2, 0, 3, 2, 0, 
  0, 0, 0, 1, 2, 2, 2, 2, 2, 3, 3, 2, 3, 2, 2, 0, 
  0, 0, 0, 0, 2, 1, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 
  0, 0, 0, 2, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 0, 0, 
  2, 0, 0, 2, 1, 1, 1, 2, 2, 3, 3, 3, 3, 0, 0, 0, 
  2, 1, 1, 2, 1, 1, 2, 2, 3, 1, 1, 3, 3, 3, 0, 0, 
  0, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 3, 3, 0, 0, 
  0, 0, 2, 2, 2, 2, 1, 1, 1, 1, 1, 3, 3, 1, 1, 0
};

// 動畫計時器
unsigned long lastAnimTime = 0;

bool useBubble2 = false; // 切換 Bubble 與 Bubble2




// 顏色映射函數 (請根據您的庫修改，如 display.drawPixel)
void drawBubble(int x, int y, int size, const uint8_t art[][16], int rows, int cols) {
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      uint8_t colorIdx = pgm_read_byte(&(art[i][j])); 
      if (colorIdx > 0) { // 0 為黑色/透明不繪製
        uint16_t color;
        // 根據索引給顏色
        if (colorIdx == 1) color = display.color565(255, 126, 0);   // 黃/藍 (視陣列定義) 
        if (colorIdx == 2) color = display.color565(34, 144, 76);     // 綠 
        if (colorIdx == 3) color = display.color565(255, 255, 255); // 白 
        display.drawPixel(x + j, y + i, color);
      }
    }
  }
}


void drawBubbleFlip(int x, int y, int size, const uint8_t art[][16], int rows, int cols, bool is2P) {
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      // 水平翻轉：讀取時將索引 j 改為 (cols - 1 - j)
      uint8_t colorIdx = pgm_read_byte(&(art[i][cols - 1 - j]));
      if (colorIdx > 0) {
        uint16_t color;
        if (colorIdx == 1) color = display.color565(255, 126, 0); // 黃色不變
        if (colorIdx == 2) {
          // 2P 顏色替換：綠色 (原本) -> 粉紅色 (2P)
          color = is2P ? display.color565(255, 105, 180) : display.color565(0, 255, 0);
        }
        if (colorIdx == 3) color = display.color565(255, 255, 255); // 白色不變
        display.drawPixel(x + j, y + i, color);
      }
    }
  }
}




void initBubble() {

 if (ModefirstRun) {
  
  for (int i = 0; i < MAX_AIRS; i++) {
    airs[i].active = false;
  }
  lastAirSpawnTime = millis(); // 順便重置計時器，避免一進入就噴氣泡
 }
}



void drawBubbleClock() {

  GetTime(); //取得時間

  if (M != tempM) {

    display.fillRect(13, 5, 38, 18, 0);
    display.drawRect(12, 5, 40, 18,display.color565(255,255,255));

    showbit12number(H, 7, 12, 14, 8, hsv2rgb(hueh, saturation, value));
    showbit12number(M, 7, 12, 34, 8, hsv2rgb(huem, saturation, value));
    tempM = M;
  }
  
  // 冒號閃爍
  if (S % 2 == 0) {
    display.drawPixel(32, 12, hsv2rgb(hue, saturation, value));
    display.drawPixel(32, 16, hsv2rgb(hue, saturation, value));
  } else {
    display.drawPixel(32, 12, 0);
    display.drawPixel(32, 16, 0);
  }
} 




void BubbleMode() {
  drawBubbleClock();
  unsigned long now = millis();

  // --- 1. 泡泡龍動畫 (1P: x=15, 2P: x=5) ---
  if (now - lastAnimTime > 500) {
    display.fillRect(10, 48, 16, 16, 0); // 抹除 1P
    display.fillRect(39, 48, 16, 16, 0);  // 抹除 2P
    useBubble2 = !useBubble2;
    lastAnimTime = now;
  }
  
  // 繪製 1P (原始)
  drawBubble(10, 48, 16, useBubble2 ? Bubble2 : Bubble, 16, 16);
  
  // 繪製 2P (翻轉 + 綠變粉)
  drawBubbleFlip(39, 48, 16, useBubble2 ? Bubble2 : Bubble, 16, 16, true);

  // --- 2. 多氣泡產生邏輯 ---
  if (now - lastAirSpawnTime > random(3, 6) * 1000) {
    for (int i = 0; i < MAX_AIRS; i++) {
      if (!airs[i].active) {
        // 隨機決定是 1P 還是 2P 噴出
        bool p2Spawn = random(0, 2); 
        airs[i].x = p2Spawn ? 32 : 27; // 2P 座標或 1P 座標
        airs[i].y = 55;
        airs[i].isPink = p2Spawn; 
        airs[i].active = true;
        airs[i].lastMoveTime = now;
        lastAirSpawnTime = now;
        break;
      }
    }
  }

  // --- 3. 更新氣泡 (包含顏色判斷) ---
  for (int i = 0; i < MAX_AIRS; i++) {
    if (airs[i].active) {
      if (now - airs[i].lastMoveTime > 100) {
        display.fillRect(airs[i].x, airs[i].y, 7, 7, 0);
        
        bool hitBottom = (airs[i].y == 23 && airs[i].x > random(0, 6) && airs[i].x < random(52, 57));
        if (hitBottom) {
          if (airs[i].x + 3 < 32) airs[i].x--;
          else airs[i].x++;
        } else {
          airs[i].y--;
          if (airs[i].y <= 40 && !(airs[i].y < 23 && airs[i].y > -2)) {
            airs[i].x += (random(0, 3) - 1);
          }
        }
        airs[i].lastMoveTime = now;
      }

      if (airs[i].y > -7) {
        for (int r = 0; r < 7; r++) {
          for (int c = 0; c < 7; c++) {
            uint8_t colorIdx = pgm_read_byte(&(AIR[r][c]));
            if (colorIdx == 1) {
              // 氣泡邊框：2P 噴出則為粉紅，否則為藍
              uint16_t bColor = airs[i].isPink ? display.color565(255, 105, 180) : display.color565(0, 255, 255);
              display.drawPixel(airs[i].x + c, airs[i].y + r, bColor);
            }
            if (colorIdx == 2) display.drawPixel(airs[i].x + c, airs[i].y + r, display.color565(255, 255, 255));
          }
        }
      } else {
        airs[i].active = false;
      }
    }
  }
}
