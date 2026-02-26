#include "Pokemon.h"

// ==========================================
// 1. 結構體定義
// ==========================================
typedef struct {
  const unsigned short* image;
  int w;
  int h;
} Pokemon;

// ==========================================
// 2. 函式原型宣告
// ==========================================
void drawPokemonAdvance(int x, int y_bottom, Pokemon p, int currentLine, int mode, bool flip);
void drawPokemonSpriteBase(int x, int y_bottom, Pokemon p, bool flip);
const unsigned short* getHand(int type);
int getWinner(int p1, int p2);

// ==========================================
// 3. 全域變數與角色資料
// ==========================================
const unsigned short BG_COLOR = 0x0000;

static Pokemon p_list[] = {
  {PIKACHU, 34, 40}, 
    {PIKACHU2, 26, 34}, 
      {PIKACHU3, 28, 33}, 
          {DUCK, 32, 39}, 
            {DREAM, 32, 35}, 
                 {DEBOO, 32, 35}, 
                  {EGG, 26, 32},
                     {BONE, 32, 35},
  {SEED, 32, 30},    
  {MI, 30, 27},     
  {TURTLE, 31, 40}, 
  {S_DRAGON, 32, 39},   
  {BUTTERFLY, 25, 26},     
};

enum GameState { SPAWN, WAIT_GAME, CLEAR, PK_SHUFFLE, PK_RESULT, CLEAR2, DESPAWN, IDLE };
static GameState currentState = SPAWN;

static int scanLine = 0;
static unsigned long lastScanTime = 0;
static unsigned long stateStartTime = 0;
static bool needsRedraw = false;

static int p1_choice, p2_choice; 
static int p1_idx, p2_idx;

// 關鍵變數：紀錄贏家的角色索引與位置
static int lastWinnerIdx = -1;          
static int lastWinnerSide = -1;         // -1: 無, 1: 1P(左), 2: 2P(右)
static bool hasTriggeredThisCycle = false; 

// ==========================================
// 4. 繪圖實作
// ==========================================

void drawPokemonSpriteBase(int x, int y_bottom, Pokemon p, bool flip) {
  if (!p.image) return;
  int start_y = y_bottom - p.h;
  for (int i = 0; i < p.h; i++) {
    for (int j = 0; j < p.w; j++) {
      int fetch_x = flip ? (p.w - 1 - j) : j;
      uint16_t color = pgm_read_word(&(p.image[i * p.w + fetch_x]));
      if (color != BG_COLOR) display.drawPixel(x + j, start_y + i, color);
    }
  }
}

void drawPokemonAdvance(int x, int y_bottom, Pokemon p, int currentLine, int mode, bool flip) {
  if (!p.image) return;
  int start_y = y_bottom - p.h; 
  for (int i = 0; i < p.h; i++) {
    bool draw = (mode == 0) ? (i >= (p.h - currentLine)) : (i >= currentLine);
    if (draw) {
      for (int j = 0; j < p.w; j++) {
        int fetch_x = flip ? (p.w - 1 - j) : j;
        uint16_t color = pgm_read_word(&(p.image[i * p.w + fetch_x]));
        if (color != BG_COLOR) display.drawPixel(x + j, start_y + i, color);
      }
    }
  }
}

void drawPokemonClock() {
  GetTime();
  if (M != tempM) {
    display.fillRect(13, 0, 18, 18, 0);
    display.drawRect(12, 0, 40, 18, display.color565(255, 255, 255));
    showbit12number(H, 7, 12, 14, 3, hsv2rgb(hueh, saturation, value));
    showbit12number(M, 7, 12, 34, 3, hsv2rgb(huem, saturation, value));
    tempM = M;
  }
  if (S % 2 == 0) {
    display.drawPixel(32, 7, hsv2rgb(hue, saturation, value));
    display.drawPixel(32, 11, hsv2rgb(hue, saturation, value));
  } else {
    display.drawPixel(32, 7, 0);
    display.drawPixel(32, 11, 0);
  }
} 

const unsigned short* getHand(int type) {
  if (type == 0) return ROCK;
  if (type == 1) return PAPER;
  return SCISSORS;
}

int getWinner(int p1, int p2) {
  if (p1 == p2) return 0;
  if ((p1 == 0 && p2 == 2) || (p1 == 1 && p2 == 0) || (p1 == 2 && p2 == 1)) return 1;
  return 2;
}

// ==========================================
// 5. PokemonMode 流程
// ==========================================
void PokemonMode() {
  drawPokemonClock();
  unsigned long now = millis();

  // --- 五分鐘循環觸發 ---
  if ((M % 5 == 0 && !hasTriggeredThisCycle) || ModefirstRun) {
    currentState = SPAWN;
    scanLine = 0;
    stateStartTime = now;
    lastScanTime = now;
    
    // 角色挑選邏輯：保留贏家在原本的位置，並隨機挑選另一邊的角色
    if (lastWinnerSide == 1) {
      // 1P 贏了，保留 1P 原角色，隨機更新 2P
      p1_idx = lastWinnerIdx;
      do { p2_idx = random(0, 13); } while (p2_idx == p1_idx);
    } else if (lastWinnerSide == 2) {
      // 2P 贏了，保留 2P 原角色，隨機更新 1P
      p2_idx = lastWinnerIdx;
      do { p1_idx = random(0, 13); } while (p1_idx == p2_idx);
    } else {
      // 第一場比賽，隨機初始化
      p1_idx = random(0, 13);
      do { p2_idx = random(0, 13); } while (p2_idx == p1_idx);
    }

    ModefirstRun = false;
    hasTriggeredThisCycle = true;
    needsRedraw = true;
    display.fillRect(0, 20, 64, 44, 0); 
  }

  if (M % 5 != 0) hasTriggeredThisCycle = false;

  Pokemon p1 = p_list[p1_idx];
  Pokemon p2 = p_list[p2_idx];
  int ground = 64; 

  switch (currentState) {
    case SPAWN:
      if (now - lastScanTime > 80) {
        scanLine++;
        lastScanTime = now;
        needsRedraw = true;
        if (scanLine >= 40) { currentState = WAIT_GAME; stateStartTime = now; }
      }
      break;

    case WAIT_GAME:
      if (now - stateStartTime > 10000) { currentState = CLEAR; stateStartTime = now; }
      break;

    case CLEAR:
      if (now - stateStartTime > 1000) { 
        display.fillRect(0, 20, 64, 44, 0);
        currentState = PK_SHUFFLE; stateStartTime = now; needsRedraw = true; 
      }
      break;

    case PK_SHUFFLE:  
      if (now - stateStartTime < 2000) {
        if (now - lastScanTime > 150) { lastScanTime = now; needsRedraw = true; }
      } else {
        p1_choice = random(0, 3);
        p2_choice = random(0, 3);
        if (p1_choice == p2_choice) stateStartTime = now; 
        else { currentState = PK_RESULT; stateStartTime = now; }
        needsRedraw = true;
      }
      break;

    case PK_RESULT:
      if (now - stateStartTime > 3000) { 
        // 紀錄本場贏家角色與位置
        int winResult = getWinner(p1_choice, p2_choice);
        if (winResult == 1) {
          lastWinnerIdx = p1_idx;
          lastWinnerSide = 1;
        } else if (winResult == 2) {
          lastWinnerIdx = p2_idx;
          lastWinnerSide = 2;
        }
        
        currentState = CLEAR2;
        lastScanTime = now; 
      }
      break;

    case CLEAR2:
      if (now - stateStartTime > 1000) { 
        display.fillRect(0, 20, 64, 44, 0);
        currentState = DESPAWN; scanLine = 0; lastScanTime = now; needsRedraw = true; 
      }
      break;

    case DESPAWN:
      if (now - lastScanTime > 80) {
        scanLine++;
        lastScanTime = now;
        needsRedraw = true;
        if (scanLine >= 40) { currentState = IDLE; needsRedraw = true; }
      }
      break;

    case IDLE:
      break; 
  }

  // --- 繪圖執行 ---
  if (needsRedraw) {
    switch (currentState) {
      case SPAWN:
        drawPokemonAdvance(0, ground, p1, scanLine, 0, false);
        drawPokemonAdvance(32, ground, p2, scanLine, 0, true);
        break;

      case WAIT_GAME:
        drawPokemonSpriteBase(0, ground, p1, false);
        drawPokemonSpriteBase(32, ground, p2, true);
        break;

      case PK_SHUFFLE:
      case PK_RESULT:
        Pokemon h1; h1.image = getHand(currentState == PK_RESULT ? p1_choice : random(0,3)); h1.w = 30; h1.h = 30;
        Pokemon h2; h2.image = getHand(currentState == PK_RESULT ? p2_choice : random(0,3)); h2.w = 30; h2.h = 30;
        display.fillRect(1, 30, 30, 30, 0);
        drawPokemonSpriteBase(1, 60, h1, false);
        display.fillRect(33, 30, 30, 30, 0); 
        drawPokemonSpriteBase(33, 60, h2, true);
        break;

      case DESPAWN:
      case IDLE:
        int win = getWinner(p1_choice, p2_choice);
        // 1P 繪製邏輯
        if (win == 2) { // 2P 贏，1P 消失
          if (currentState == DESPAWN) {
            display.fillRect(0, 20, 32, 44, 0);
            drawPokemonAdvance(0, ground, p1, scanLine, 1, false);
          }
        } else { // 1P 贏或平手狀態，保留顯示
          drawPokemonSpriteBase(0, ground, p1, false);
        }

        // 2P 繪製邏輯
        if (win == 1) { // 1P 贏，2P 消失
          if (currentState == DESPAWN) {
            display.fillRect(32, 20, 32, 44, 0);
            drawPokemonAdvance(32, ground, p2, scanLine, 1, true); 
          }
        } else { // 2P 贏，保留顯示
          drawPokemonSpriteBase(32, ground, p2, true);
        }
        break;
    }
    needsRedraw = false; 
  }
}
