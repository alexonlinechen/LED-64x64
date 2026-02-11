#include "Mario.h"


// ===================== 內部狀態 =====================

static float prev_m_y = 40.0;    //虛擬的基準線
// 角色動態
static float m_y = 40.0;           // 角色基準 Y（浮點讓跳躍更平滑）
static float m_vy = 0.0f;
static bool  m_jumping = false;

static bool  block_move = true;

static int last_min_shown = -1;

// 角色切換（0=Mario,1=Yoshi,2=Mushroom,3=Cloud）
static int currentHero = 0;

// 上一幀主角資訊：用 sprite 遮罩擦除，避免整塊 fillRect 閃爍
static int lastHeroX = 20, lastHeroY = 30, lastHeroW = 25, lastHeroH = 30;
static const unsigned short* lastHeroSprite = nullptr;
static int last_offsetY = 0;


// ===================== 基本繪圖：畫 sprite（透明=SKY_COLOR 不畫） =====================
static inline void drawMarioSprite(int x, int y, int w, int h, const unsigned short* data) {
  if (!data) return;
  for (int i = 0; i < h; i++) {
    for (int j = 0; j < w; j++) {
      uint16_t color = pgm_read_word(&(data[i * w + j]));
      if (color != SKY_COLOR) {
        display.drawPixel(x + j, y + i, color);
      }
    }
  }
}

// 用 sprite 遮罩擦除：把「非 SKY」像素擦回 SKY_COLOR（避免整塊挖空）
static inline void eraseMarioSprite(int x, int y, int w, int h, const unsigned short* data) {
  if (!data) return;
  for (int i = 0; i < h; i++) {
    for (int j = 0; j < w; j++) {
      uint16_t color = pgm_read_word(&(data[i * w + j]));
      if (color != SKY_COLOR) {
        display.drawPixel(x + j, y + i, SKY_COLOR);
      }
    }
  }
}


// ===================== 初始化 =====================
static void MarioInit() {

if (ModefirstRun) {
  // 清成天空色（只做一次）
  display.fillRect(0, 0, 64, 64, SKY_COLOR);

    display.fillScreen(SKY_COLOR);
    Serial.println("Mario 初始");
  
    // 畫背景 (這些只畫一次，之後除非被覆蓋否則不重畫)
    drawMarioSprite(0, 38, 20, 22, HILL);   
    drawMarioSprite(0, 21, 13, 12, CLOUD1); 
    drawMarioSprite(51, 7, 13, 12, CLOUD2); 
    drawMarioSprite(43, 51, 21, 9, BUSH);   
    for(int x = 0; x < 64; x += 8) {
      drawMarioSprite(x, 60, 8, 8, GROUND); 
    }

    // 畫靜態的小時磚塊 (它不會動，所以畫一次就好)

    drawMarioSprite(13, 4 , 19, 19, BLOCK);
    drawMarioSprite(32, 4 , 19, 19, BLOCK);
    show_mario_number(H, 7, 12, 14, 6, 0xF800);
    show_mario_number(M, 7, 12, 33, 6, 0xF800);

  // 畫初始主角
  drawMarioSprite(23, 34, 16, 26, MARIO);

    m_y = 40.0;    
    prev_m_y = 40.0;
    GetTime();
    last_min_shown = M; 
    ModefirstRun = false;

    display.showBuffer();
      }
    }


// ===================== 主模式：每次呼叫推進動畫一小步 =====================
void MarioMode() {
  MarioInit();

  GetTime(); 

  delay(80); //畫面更新延遲
  
  // ---- 觸發跳躍----

  if (M != last_min_shown && !m_jumping) {

    currentHero = random(0, 4);

    display.fillRect(20, (int)prev_m_y - 14, 25, 34, SKY_COLOR);
    if (currentHero == 0) drawMarioSprite(23, 34, 16, 26, MARIO); 
    if (currentHero == 1) drawMarioSprite(20, 30, 25, 30, YOSHI);
    if (currentHero == 2) drawMarioSprite(23, 30, 20, 30, MUSHROOM); 
    if (currentHero == 3) drawMarioSprite(20, 30, 25, 30, CLOUD);    
    delay(10000);
    
    m_jumping = true;
    m_vy = -3.5;  // 起跳速度數值越低 會跳得比較低
    
  }

  // ---- 擬真跳躍物理----
  if (m_jumping) {
    
   m_y += m_vy;
   m_vy += 0.45; 

    if (block_move && lastHeroY  < 20) {    //碰撞偵測：撞擊磚塊
     
    //Serial.println("碰撞");
    display.fillRect(13, 13, 38, 10, SKY_COLOR);   
    drawMarioSprite(13, -4 , 19, 19, BLOCK);
    drawMarioSprite(32, -4 , 19, 19, BLOCK);
    block_move = false ;
    }

    

    // 落地
    if (m_y >= 40.0f) {
      Serial.println("落地");
      m_y = 40.0f;
      m_jumping = false;
    display.fillRect(13, 0, 38, 4, SKY_COLOR);
    drawMarioSprite(13, 4 , 19, 19, BLOCK);
    drawMarioSprite(32, 4 , 19, 19, BLOCK);
    show_mario_number(H, 7, 12, 14, 6, 0xF800);
    show_mario_number(M, 7, 12, 33, 6, 0xF800);
    block_move = true ;
    }
    



  // ---- 先擦除上一幀主角（用 sprite 遮罩擦，避免整塊 fillRect 閃）----
  eraseMarioSprite(lastHeroX, lastHeroY, lastHeroW, lastHeroH, lastHeroSprite);


  // ---- 決定本幀主角 sprite ----
  int heroX = 20, heroY = (int)m_y - 10, heroW = 25, heroH = 30;
  const unsigned short* heroSprite = nullptr;

  if (currentHero == 0) { // Mario 16x26
    heroX = 23;
    heroW = 16; heroH = 26;
    heroY = (int)m_y - 6;
    heroSprite = (m_jumping ? MARIO_JUMP : MARIO);
  }
  else if (currentHero == 1) { // Yoshi 25x30
    heroX = 20;
    heroW = 25; heroH = 30;
    heroY = (int)m_y - 10;
    heroSprite = (m_jumping ? YOSHI_JUMP : YOSHI);
  }
  else if (currentHero == 2) { // Mushroom 20x30
    heroX = 23;
    heroW = 20; heroH = 30;
    heroY = (int)m_y - 10;
    heroSprite = MUSHROOM;
  }
  else { // Cloud 25x30
    heroX = 20;
    heroW = 25; heroH = 30;
    heroY = (int)m_y - 10;
    heroSprite = CLOUD;
  }

  // ---- 畫本幀主角 ----
  drawMarioSprite(heroX, heroY, heroW, heroH, heroSprite);

  // ---- 記錄本幀供下一幀擦除 ----
  lastHeroX = heroX; lastHeroY = heroY;
  lastHeroW = heroW; lastHeroH = heroH;
  lastHeroSprite = heroSprite;

  // ---- 一幀結束才 showBuffer（避免看到中間狀態）----
  last_min_shown = M; 
  display.showBuffer();

    }
    
}
