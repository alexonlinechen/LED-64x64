#include "Mario.h"



static float m_y = 40.0;      
static float prev_m_y = 40.0;    //虛擬的基準線
static float m_vy = 0.0;     
static bool m_jumping = false; 
static int m_offsetY = 0;        
static int last_min_shown = -1;  
static int currentHero = 0; // 0 代表 Mario, 1 代表 Yoshi

void MarioMode() {
  // --- 1. 只有第一次進入時畫完整的背景 (Static Layer) ---
  if (ModefirstRun) {
    display.fillScreen(SKY_COLOR);
    Serial.println("Mario 初始");

    
    // 畫背景 (這些只畫一次，之後除非被覆蓋否則不重畫)
    drawMarioSprite(0, 34, 20, 22, HILL);   
    drawMarioSprite(0, 21, 13, 12, CLOUD1); 
    drawMarioSprite(51, 7, 13, 12, CLOUD2); 
    drawMarioSprite(43, 47, 21, 9, BUSH);   
    for(int x = 0; x < 64; x += 8) {
      drawMarioSprite(x, 56, 8, 8, GROUND); 
    }
    
    // 畫靜態的小時磚塊 (它不會動，所以畫一次就好)


    drawMarioSprite(13, 4 , 19, 19, BLOCK);
    drawMarioSprite(32, 4 , 19, 19, BLOCK);
    show_mario_number(H, 7, 12, 14, 10, 0xFFFF);
    show_mario_number(M, 7, 33, 14, 10, 0xFFFF);
    
    drawMarioSprite(13, 4, 19, 19, BLOCK);

    
    drawMarioSprite(20, correctY, 16, 26, MARIO);
    
    m_y = 40.0;    
    prev_m_y = 40.0;
    GetTime();
    last_min_shown = M; 
    ModefirstRun = false;
  }

  GetTime(); 
 
/*
  M = M+1 ;
  H = H+1 ;
  Serial.println(M);
  if (M>=99) M = 5 ; 
  if (H>=99) H = 5 ; 
*/
  
  delay(80);
  
  // --- 2. 邏輯處理 (跳躍判斷) ---
  if (M != last_min_shown && !m_jumping) {
    m_jumping = true;
    m_vy = -3.5;   //  -3.5 數值越低 會跳得比較低

    // 隨機切換主角：產生 0 或 1
    currentHero = random(0, 2);
  // currentHero = 0 ;
  }

  // --- 3. 動態更新層 (Dynamic Layer) ---
  if (m_jumping || m_offsetY != 0) {
    
// [抹除背景]
      display.fillRect(20, (int)prev_m_y - 14, 25, 30, SKY_COLOR);
    

 
    
    // 物理計算
    m_y += m_vy;
    m_vy += 0.45; 
    
    if (m_vy < 0 &&(m_y - 10) <= 23) {    //碰撞偵測：撞擊磚塊
      m_offsetY = -4;        
      last_min_shown = M;    
    }
    if (m_y >= 40.0) { m_y = 40.0; m_jumping = false; }   //地板的限制
    
    // [修補磚塊背景] 抹掉分鐘磚塊舊軌跡
    display.fillRect(13, 0, 38, 23, SKY_COLOR);
    
    // [重繪分鐘磚塊]
    drawMarioSprite(13, 4 + m_offsetY, 19, 19, BLOCK);
    drawMarioSprite(32, 4 + m_offsetY, 19, 19, BLOCK);
    show_mario_number(last_min_shown, 7, 12, 33, 6 + m_offsetY, 0xFFFF);
    show_mario_number(H, 7, 12, 14, 6 + m_offsetY, 0xFFFF);


    
    if (m_offsetY < 0) m_offsetY += 1;

// [重繪主角：Mario 或 Yoshi]
if (currentHero == 0) {
    // 16x26 的 Mario 對齊腳部 (26-16 = 10)
    int correctY = (int)m_y - 10; 
    if (m_jumping) {
        drawMarioSprite(23, correctY, 16, 26, MARIO_JUMP);
    } else {
        drawMarioSprite(20, correctY, 16, 26, MARIO);
    }
} else {
      // 繪製 Yoshi (20x30)
      // 腳部對齊：Yoshi 比 Mario 高 14px (30-16)，所以 Y 座標要減 14
      int yoshiY = (int)m_y - 14; 
      drawMarioSprite(20, yoshiY, 25, 30, YOSHI); 
    }
    
    prev_m_y = m_y;
  }/* else {
    // 當一切靜止時，只畫靜止狀態的瑪利歐 (維持畫面不消失)
    drawMarioSprite(23, 40, 13, 16, MARIO_IDLE);
    // 靜止時的分鐘磚塊
    drawMarioSprite(32, 4, 19, 19, BLOCK);
    show_mario_number(last_min_shown, 7, 12, 33, 6, 0xFFFF);
    show_mario_number(H, 7, 12, 14, 6, 0xFFFF);
  }   */
}





// --- 繪圖函數：支援動態寬高 ---
void drawMarioSprite(int x, int y, int w, int h, const unsigned short* data) {
  if (data == NULL) return;
  for (int i = 0; i < h; i++) {
    for (int j = 0; j < w; j++) {
      uint16_t color = pgm_read_word(&(data[i * w + j]));

      // 修改重點：只跳過背景色 (SKY_COLOR)，但一定要畫出黑色 (0x0000)
      // 如果你的 assets.h 裡定義了 _MASK，也可以用 _MASK
      if (color != SKY_COLOR) { 
        display.drawPixel(x + j, y + i, color);
      }
    }
  }
}
