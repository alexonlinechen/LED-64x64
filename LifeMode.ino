// --- 生命模式相關變數 ---



const byte CELL[225] PROGMEM = {
  0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,
  0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,
  0,0,1,1,1,1,1,1,1,1,1,1,1,0,0,
  0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
  0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
  1,1,1,2,2,2,1,1,1,2,2,2,1,1,1,
  1,1,2,0,0,0,2,1,2,0,0,0,2,1,1,
  1,1,2,0,0,0,2,1,2,0,0,0,2,1,1,
  1,1,2,0,0,0,2,1,2,0,0,0,2,1,1, 
  1,1,1,2,2,2,1,1,1,2,2,2,1,1,1,
  0,1,1,1,1,1,1,1,1,1,1,1,1,1,0, 
  0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
  0,0,1,1,1,1,1,1,1,1,1,1,1,0,0,
  0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,
  0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,
};


#define MAX_CELLS 3     // 同時存在最多細胞數
#define FOOD_COUNT 3    // 畫面上的食物數

struct CellInstance {
  bool active = false;
  float x, y, vx, vy;
  float energy = 100.0; // 使用 float 讓能量消耗更細膩
  int lastX, lastY;
  int age = 0;
};

struct FoodInstance {
  bool active = false;
  int x, y;
};

// 外部宣告
extern CellInstance cells[MAX_CELLS];
extern FoodInstance foods[FOOD_COUNT];
extern unsigned long lastEvolutionTick;



// ---------------------------------------------------------
// 核心生命模式：處理移動、渲染與生命週期
// ---------------------------------------------------------
// ===================== 基本繪圖與遮罩擦除 =====================

CellInstance cells[MAX_CELLS];
FoodInstance foods[FOOD_COUNT];
unsigned long lastEvolutionTick = 0;

// --- 繪圖函式：支援動態變色 ---
void drawCell15(int x, int y, uint16_t bodyColor) {
  for (int i = 0; i < 15; i++) {
    for (int j = 0; j < 15; j++) {
      byte val = pgm_read_byte(&(CELL[i * 15 + j]));
      if (val == 1) {
        display.drawPixel(x + j, y + i, bodyColor); // 身體色 (1)
      } else if (val == 2) {
        display.drawPixel(x + j, y + i, 0xFFFF);    // 永遠白色 (2)
      }
      // val 為 0 則不畫（黑色/透明）
    }
  }
}

// --- 擦除函式：精準擦除有像素的地方 ---
void eraseCell15(int x, int y) {
  for (int i = 0; i < 15; i++) {
    for (int j = 0; j < 15; j++) {
      byte val = pgm_read_byte(&(CELL[i * 15 + j]));
      if (val > 0) { // 只要是 1 或 2 的地方都擦除
        display.drawPixel(x + j, y + i, 0x0000);
      }
    }
  }
}

void LifeMode() {
  if (millis() - lastEvolutionTick > 80) {
    
    // 食物邏輯 (維持綠色點)
    for (int f = 0; f < FOOD_COUNT; f++) {
      if (!foods[f].active) {
        foods[f].x = random(2, 62);
        foods[f].y = random(15, 62);
        foods[f].active = true;
      }
      display.drawPixel(foods[f].x, foods[f].y, 0x07E0); 
    }

    int aliveCount = 0;
    for (int i = 0; i < MAX_CELLS; i++) {
      if (!cells[i].active) continue;
      aliveCount++;

     // --- 決定顏色：健康(0x9F2A) -> 飢餓(黃色) -> 垂死(紅色) ---
      uint16_t currentBodyColor;
      
      if (cells[i].energy > 60.0) {
        currentBodyColor = 0x9F2A; // 正常狀態（你指定的顏色）
      } 
      else if (cells[i].energy > 30.0) {
        currentBodyColor = 0xFFE0; // 飢餓狀態（黃色）
      } 
      else {
        currentBodyColor = 0xF800; // 垂死狀態（紅色）
      }

      // 2. 擦除上一幀
      eraseCell15(cells[i].lastX, cells[i].lastY);

      // 3. 移動邏輯
      cells[i].x += cells[i].vx;
      cells[i].y += cells[i].vy;

      // 邊界檢查 (15x15 邊界：64-15 = 49)
      if (cells[i].x < 0 || cells[i].x > 49) cells[i].vx *= -1;
      if (cells[i].y < 15 || cells[i].y > 49) cells[i].vy *= -1;

      // 4. 進食判定 (15x15)
      for (int f = 0; f < FOOD_COUNT; f++) {
        if (foods[f].active && 
            foods[f].x >= cells[i].x && foods[f].x <= cells[i].x + 15 &&
            foods[f].y >= cells[i].y && foods[f].y <= cells[i].y + 15) {
          
          //限制能量上限為 200 ---
          if (cells[i].energy < 200.0) {
            cells[i].energy += 30.0;
            // 確保不會超過 200 (例如 190+50 會變成 240，這行能強制鎖定在 200)
            if (cells[i].energy > 200.0) cells[i].energy = 200.0;
          }
          
          foods[f].active = false;
          display.drawPixel(foods[f].x, foods[f].y, 0x0000); // 擦掉食物
        }
      }

      // 5. 能量與生命循環
      cells[i].energy -= 0.4; // 消耗速度
      
      if (cells[i].energy > 160.0) { // 分裂門檻
          for (int j = 0; j < MAX_CELLS; j++) {
              if (!cells[j].active) {
                  cells[j].active = true;
                  cells[j].x = cells[i].x; cells[j].y = cells[i].y;
                  cells[j].vx = -cells[i].vx + (random(-20, 21)/100.0);
                  cells[j].vy = -cells[i].vy + (random(-20, 21)/100.0);
                  cells[j].energy = 80.0;
                  cells[i].energy = 80.0;
                  break;
              }
          }
      }

      if (cells[i].energy <= 0) {
        cells[i].active = false;
        eraseCell15((int)cells[i].x, (int)cells[i].y);
        continue;
      }

      // 6. 繪製新位置
      drawCell15((int)cells[i].x, (int)cells[i].y, currentBodyColor);
      cells[i].lastX = (int)cells[i].x;
      cells[i].lastY = (int)cells[i].y;
    }

   // --- 修改後的重啟邏輯 ---
    if (aliveCount == 0) {
      // 1. 強制重置所有食物，讓它們在下一幀重新隨機生成位置
      for (int f = 0; f < FOOD_COUNT; f++) {
          // 先把螢幕上舊的食物殘影擦掉
          display.drawPixel(foods[f].x, foods[f].y, 0x0000); 
          // 設為不活躍，這會強迫 loop 開頭的食物邏輯重新執行 random()
          foods[f].active = false; 
      }

      // 2. 重新啟動第一個細胞
      cells[0].active = true; 
      // 隨機出生位置 (盡量避開最邊緣)
      cells[0].x = random(10, 45); 
      cells[0].y = random(20, 45);
      
      // 3. 重要：給予真正的隨機初速，否則移動路徑會永遠固定
      // randomSeed(analogRead(A0)) 若無效，這裡可以嘗試加上 randomSeed(micros());
      cells[0].vx = (random(20, 70) / 100.0) * (random(0, 2) == 0 ? 1 : -1);
      cells[0].vy = (random(20, 70) / 100.0) * (random(0, 2) == 0 ? 1 : -1);
      
      cells[0].energy = 100.0;
      
      // 4. 小撇步：在全滅時額外消耗掉一些隨機數，確保下一局序列偏移
      for(int r=0; r < (millis() % 10); r++) random(100);
    }

    display.showBuffer();
    lastEvolutionTick = millis();
  }

   GetTime();
  // 頂部時間更新
  if (M != last_min_shown) {
    //display.fillRect(0, 0, 64, 13, 0x0000);
    showbit12number(H, 7, 12, 14, 1, hsv2rgb(hueh, saturation, value));
    display.drawPixel(32, 4, hsv2rgb(hue, saturation, value));
    display.drawPixel(32, 8, hsv2rgb(hue, saturation, value));
    showbit12number(M, 7, 12, 35, 1, hsv2rgb(huem, saturation, value));
    last_min_shown = M;
    
   }
 display.showBuffer();
  
}


/*
  // 2. 處理頂部時間 (獨立刷新，不影響下方細胞)
  if (millis() - lastTime > 1000) {
    GetTime();
    // 僅清除頂部時間區
    display.fillRect(0, 0, 64, 13, 0x0000);
    showbit12number(H, 7, 12, 14, 1, hsv2rgb(hueh, saturation, value));
    display.drawPixel(32, 4, hsv2rgb(hue, saturation, value));
    display.drawPixel(32, 8, hsv2rgb(hue, saturation, value));
    showbit12number(M, 7, 12, 35, 1, hsv2rgb(huem, saturation, value));
    lastTime = millis();
    display.showBuffer();
  }

  */
