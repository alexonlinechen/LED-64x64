// =====================================================
// 圖形數據 (Sprite Data)
// =====================================================
// ESP8266：把大型常數表放到 Flash，避免佔用 RAM
#include <pgmspace.h>

// Pac-Man 張嘴圖案 (0=透, 1=身)
const uint8_t PROGMEM pacman2[4][4] = {
  {0,1,1,1},
  {1,1,0,0},
  {1,1,0,0},
  {0,1,1,1}
};
// Pac-Man 閉嘴圖案
const uint8_t PROGMEM pacmanClosed[4][4] = {
  {0,1,1,0},
  {1,1,1,1},
  {1,1,1,1},
  {0,1,1,0}
};
// 鬼的圖案 (2=眼睛)
const uint8_t PROGMEM ghost1[4][4] = {
  {0,1,1,0},
  {1,2,2,1},
  {1,1,1,1},
  {1,1,1,1}
};

// =====================================================
// 地圖數據 (Map Data)
// 0=路, 1=牆, 2=鬼屋入口(視覺上也是空地)
// =====================================================
const uint8_t PROGMEM mazeC1[16][16] = {
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
  {1,0,0,0,1,1,0,0,0,0,1,1,0,0,0,1},
  {1,0,1,0,0,0,0,1,1,0,0,0,0,1,0,1},
  {1,0,0,0,1,1,0,0,0,0,1,1,0,0,0,1},
  {1,0,1,0,1,1,0,1,1,0,1,1,0,1,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,1,0,1,1,1,1,1,1,1,1,1,1,0,1,1},
  {0,0,0,1,1,2,2,2,2,2,2,1,1,0,0,0},
  {1,1,0,1,1,2,2,2,2,2,2,1,1,0,1,1},
  {1,0,0,1,1,2,2,2,2,2,2,1,1,0,0,1},
  {1,1,0,1,1,1,1,1,1,1,1,1,1,0,1,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,1,0,1,1,0,1,1,0,1,1,0,1,0,1},
  {1,0,1,0,0,0,0,1,1,0,0,0,0,1,0,1},
  {1,0,0,0,1,1,0,0,0,0,1,1,0,0,0,1},
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};
const uint8_t PROGMEM mazeR1[16][16] = {
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,1,1,1,1,1,0,0,1,1,1,1,1,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,1,0,1,1,1,1,1,1,1,1,1,1,0,1,1},
  {1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1},
  {1,1,0,1,1,1,1,1,1,1,1,1,1,0,0,1},
  {0,0,0,1,1,2,2,2,2,2,2,1,1,0,0,1},
  {1,1,0,1,1,2,2,2,2,2,2,1,1,0,1,1},
  {1,1,0,1,1,2,2,2,2,2,2,1,1,0,1,1},
  {1,0,0,1,1,1,1,1,1,1,1,1,1,0,0,1},
  {1,0,1,0,0,0,0,0,0,0,0,0,0,1,0,1},
  {1,0,0,0,1,1,1,1,1,1,1,1,0,0,0,1},
  {1,0,1,0,0,0,0,0,0,0,0,0,0,1,0,1},
  {1,0,0,0,1,1,1,0,0,1,1,1,0,0,0,1},
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};
const uint8_t PROGMEM mazeL1[16][16] = {
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,1,1,1,1,1,1,0,1,1,1,1,1,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,1,1,1,1,1,1,0,1,1,1,0,1,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,1,0,1,1,1,1,1,1,1,1,1,1,0,1,1},
  {1,0,0,1,1,2,2,0,2,2,2,1,1,0,0,0},
  {1,1,0,1,1,2,2,0,2,2,2,1,1,0,1,1},
  {1,0,0,1,1,2,2,0,2,2,2,1,1,0,0,1},
  {1,0,0,1,1,1,1,0,0,1,1,1,1,0,0,1},
  {1,1,0,0,0,0,1,0,0,1,0,0,0,1,0,1},
  {1,0,0,1,1,0,0,0,0,0,0,1,0,1,0,1},
  {1,0,1,1,1,1,1,1,1,1,1,1,1,1,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};

// =====================================================
// 全域變數與設定
// =====================================================
enum MapId : uint8_t { MAP_L1, MAP_C1, MAP_R1 };
static MapId curMapId = MAP_C1;
// 目前地圖改用 PROGMEM 讀取，不再用指標直接存取

// 豆子系統：用 16x16 bitset 取代 bool[16][16]，大幅減少 RAM
// dotRowBits[y] 的第 x bit = 1 代表該格有豆子
static uint16_t dotRowBits[16];
unsigned long dotsEatenTime = 0;
bool allDotsEaten = false;

// 鬼 (Ghost) - 只在 R1 活動
static int gHX = 14, gHY = 7;     // 鬼的初始座標
static int gHDir = 2;             // 鬼的方向 (0右, 1下, 2左, 3上)
static bool ghostInit = false;

// 遊戲狀態
static bool gameOver = false;
static unsigned long gameOverAt = 0;
static bool gameWin = false;
static unsigned long gameWinAt = 0;

// Power Block (無敵道具) - 只在 L1
static bool powerBlockActive = true;
static bool invincible = false;
static unsigned long invincibleUntil = 0;
const int PBX = 7; // 道具位置 X
const int PBY = 7; // 道具位置 Y

// 方向向量 (0:右, 1:下, 2:左, 3:上)
const int dx[4] = { 1, 0,-1, 0};
const int dy[4] = { 0, 1, 0,-1};

// [可調參數]
const uint8_t PAC_TURN_CHANCE = 25;   // Pacman 在路口轉彎的機率 (%)
const uint8_t GHOST_TURN_CHANCE = 30; // 鬼在路口轉彎的機率 (%)

static inline void setMazeById(MapId id);

// =====================================================
// PROGMEM 地圖/豆子存取工具
// =====================================================
static inline uint8_t mazeCell(int x, int y) {
  // 0=路, 1=牆, 2=鬼屋入口(視覺上也是空地)
  if ((uint8_t)x > 15 || (uint8_t)y > 15) return 1; // 出界視為牆

  const uint8_t* base = nullptr;
  switch (curMapId) {
    case MAP_L1: base = &mazeL1[0][0]; break;
    case MAP_R1: base = &mazeR1[0][0]; break;
    case MAP_C1:
    default:     base = &mazeC1[0][0]; break;
  }
  return pgm_read_byte(base + (y * 16) + x);
}

static inline bool dotGet(int x, int y) {
  if ((uint8_t)x > 15 || (uint8_t)y > 15) return false;
  return (dotRowBits[y] >> x) & 0x01;
}

static inline void dotSet(int x, int y, bool v) {
  if ((uint8_t)x > 15 || (uint8_t)y > 15) return;
  const uint16_t mask = (uint16_t)1u << x;
  if (v) dotRowBits[y] |= mask;
  else   dotRowBits[y] &= (uint16_t)~mask;
}

// =====================================================
// 工具函式：判斷是否可走 (0 為路)
// =====================================================
static inline bool isWalkableCell(int x, int y) {
  if (x < 0 || x > 15 || y < 0 || y > 15) return false;
  // 2(鬼屋入口) 在本遊戲視覺上是空地，但不走進去，所以只允許 0
  return (mazeCell(x, y) == 0);
}

// =====================================================
// 工具函式：判斷是否是牆 (1 為牆) - 用於繪圖
// =====================================================
static inline bool isWall(int x, int y) {
  if (x < 0 || x > 15 || y < 0 || y > 15) return false;
  return (mazeCell(x, y) == 1);
}

// =====================================================
// 牆面繪製邏輯
// =====================================================
void drawFullMaze() {
  uint16_t wallColor = display.color565(33, 33, 255); // 牆壁顏色 (藍色)

  for (int y = 0; y < 16; y++) {
    for (int x = 0; x < 16; x++) {
      if (!isWall(x, y)) continue;

      int px = x * 4;
      int py = y * 4;

      // 1. 繪製基本邊框 (只畫牆與路的交界)
      if (!isWall(x, y - 1)) display.drawLine(px,     py,     px + 3, py,     wallColor); // 上
      if (!isWall(x, y + 1)) display.drawLine(px,     py + 3, px + 3, py + 3, wallColor); // 下
      if (!isWall(x - 1, y)) display.drawLine(px,     py,     px,     py + 3, wallColor); // 左
      if (!isWall(x + 1, y)) display.drawLine(px + 3, py,     px + 3, py + 3, wallColor); // 右

      // 2. 繪製圓角效果 (對角修補)
      if (isWall(x + 1, y) && isWall(x, y + 1) && !isWall(x + 1, y + 1))
        display.drawPixel(px + 3, py + 3, wallColor);
      if (isWall(x + 1, y) && isWall(x, y - 1) && !isWall(x + 1, y - 1))
        display.drawPixel(px + 3, py, wallColor);
      if (isWall(x - 1, y) && isWall(x, y + 1) && !isWall(x - 1, y + 1))
        display.drawPixel(px, py + 3, wallColor);
      if (isWall(x - 1, y) && isWall(x, y - 1) && !isWall(x - 1, y - 1))
        display.drawPixel(px, py, wallColor);
    }
  }
}

// =====================================================
// 豆子邏輯：重置與檢查
// =====================================================
void resetDots() {
  for (int y = 0; y < 16; y++) {
    dotRowBits[y] = 0;
    for (int x = 0; x < 16; x++) {
      // 只有在空地 (0) 且不在傳送門附近 (x=0 或 x=15) 才放豆子
      const bool put = (mazeCell(x, y) == 0) && (x != 0) && (x != 15);
      dotSet(x, y, put);
      if (put) {
        display.drawPixel(x * 4 + 2, y * 4 + 2, display.color565(150, 150, 150));
      }
    }
  }
}

void checkDotsStatus() {
  bool found = false;
  for (int y = 0; y < 16; y++) {
    if (dotRowBits[y] != 0) { found = true; break; }
  }
  if (!found && !allDotsEaten) {
    allDotsEaten = true;
    dotsEatenTime = millis();
    Serial.println(F("豆子吃光了！1分鐘後重置..."));
  }
}

// =====================================================
// 地圖切換
// =====================================================
static inline void setMazeById(MapId id) {
  curMapId = id;

  display.fillScreen(0);
  drawFullMaze();
  resetDots();
  drawClockOverlay(true); // 重畫時鐘
}

// =====================================================
// AI 移動邏輯：計算可行方向數 (不含回頭)
// =====================================================
static int countChoicesNoBack(int x, int y, int curDir) {
  int backDir = (curDir + 2) & 3; // 反方向
  int c = 0;
  for (int dir = 0; dir < 4; dir++) {
    if (dir == backDir) continue;
    int nx = x + dx[dir];
    int ny = y + dy[dir];
    if (isWalkableCell(nx, ny)) c++;
  }
  return c;
}

// =====================================================
// AI 移動邏輯：路口隨機轉彎
// =====================================================
static int chooseTurnAtJunction(int x, int y, int curDir, uint8_t chancePct) {
  int backDir = (curDir + 2) & 3;
  int opts[4], n = 0;

  // 找出所有可走方向
  for (int dir = 0; dir < 4; dir++) {
    if (dir == backDir) continue;
    if (isWalkableCell(x + dx[dir], y + dy[dir])) opts[n++] = dir;
  }

  if (n < 2) return curDir; // 不是路口 (只有直走一條路)
  if (random(100) >= chancePct) return curDir; // 依機率決定是否轉彎

  // 篩選出「非直走」的選項
  int turnOpts[4], m = 0;
  for (int i = 0; i < n; i++) {
    if (opts[i] != curDir) turnOpts[m++] = opts[i];
  }

  if (m > 0) return turnOpts[random(m)]; // 隨機選一個轉彎方向
  return curDir;
}

// =====================================================
// AI 移動邏輯：撞牆時選擇新方向
// =====================================================
static int chooseDirWhenBlocked(int x, int y, int curDir) {
  int backDir = (curDir + 2) & 3;
  int opts[4], n = 0;

  for (int dir = 0; dir < 4; dir++) {
    if (dir == backDir) continue; // 不回頭
    if (isWalkableCell(x + dx[dir], y + dy[dir])) {
      opts[n++] = dir;
    }
  }

  if (n > 0) return opts[random(n)]; // 有路就隨機選
  return backDir; // 沒路只好回頭
}

// =====================================================
// 繪圖：Ghost
// =====================================================
static inline void drawGhostCell(int cellX, int cellY) {
  uint16_t body = display.color565(255, 0, 0); // 鬼身體顏色 (紅)
  uint16_t eye  = display.color565(255, 255, 255); // 眼睛顏色 (白)

  int px = cellX * 4;
  int py = cellY * 4;

  for (int y = 0; y < 4; y++) {
    for (int x = 0; x < 4; x++) {
      uint8_t v = pgm_read_byte(&ghost1[y][x]);
      if (v == 0) continue;
      display.drawPixel(px + x, py + y, (v == 2) ? eye : body);
    }
  }
}

// =====================================================
// 繪圖：文字顯示 (GAME OVER / WIN)
// =====================================================
static inline void drawStatusText(bool win) {
  display.fillRect(13, 25, 38, 18, 0); // 清除中央區域
  if (win) {
     display.setTextColor(display.color565(0, 255, 0)); // 綠色
     display.setCursor(15, 26); display.print("YOU");
     display.setCursor(29, 35); display.print("WIN");
  } else {
     display.setTextColor(display.color565(255, 0, 0)); // 紅色
     display.setCursor(15, 26); display.print("GAME");
     display.setCursor(25, 35); display.print("OVER");
  }
}

// =====================================================
// 繪圖：Power Block (無敵道具)
// =====================================================
static inline bool isOnPowerBlock(int x, int y) {
  return powerBlockActive && (x == PBX || x == PBX + 1) && (y == PBY || y == PBY + 1);
}

static inline void drawPowerBlock() {
  uint16_t c = display.color565(255, 255, 0); // 黃色
  int px = PBX * 4 + 2;
  int py = PBY * 4 + 1;
  display.fillRect(px, py, 3, 3, c);
}

static inline void clearPowerBlockArea() {
  int px = PBX * 4 + 2;
  int py = PBY * 4 + 1;
  display.fillRect(px, py, 3, 3, 0);
}

// =====================================================
// 繪圖：時鐘重疊層
// =====================================================
static inline void drawClockOverlay(bool forceRedrawNumbers) {
  static int lastDrawnM = -1;
  GetTime();
  if (forceRedrawNumbers || M != lastDrawnM) {
    display.fillRect(13, 25, 38, 18, 0);
    showbit12number(H, 7, 12, 14, 28, hsv2rgb(hueh, saturation, value));
    showbit12number(M, 7, 12, 34, 28, hsv2rgb(huem, saturation, value));
    lastDrawnM = M;
  }
  // 冒號閃爍
  if (S % 2 == 0) {
    display.drawPixel(32, 32, hsv2rgb(hue, saturation, value));
    display.drawPixel(32, 36, hsv2rgb(hue, saturation, value));
  } else {
    display.drawPixel(32, 32, 0);
    display.drawPixel(32, 36, 0);
  }
}

// =====================================================
// 繪圖：Pacman Sprite 處理 (旋轉)
// =====================================================
static inline uint8_t sprRead4(const uint8_t spr[4][4], int x, int y) {
  return pgm_read_byte(&spr[y][x]);
}

static inline uint8_t spriteAt4(const uint8_t spr[4][4], int sx, int sy, int dir) {
  switch (dir & 3) {
    case 0:  return sprRead4(spr, sx, sy);            // 右
    case 2:  return sprRead4(spr, 3 - sx, sy);        // 左 (水平翻轉)
    case 1:  return sprRead4(spr, sy, 3 - sx);        // 下
    case 3:  return sprRead4(spr, 3 - sy, sx);        // 上
  }
  return 0;
}

static inline void drawSprite4x4Cell(int cellX, int cellY, const uint8_t spr[4][4], int dir, uint16_t bodyColor) {
  int px = cellX * 4;
  int py = cellY * 4;
  for (int sy = 0; sy < 4; sy++) {
    for (int sx = 0; sx < 4; sx++) {
      uint8_t v = spriteAt4(spr, sx, sy, dir);
      if (v == 0) continue;
      display.drawPixel(px + sx, py + sy, bodyColor);
    }
  }
}

static inline void drawPacmanSprite(int cellX, int cellY, int dir, bool invincible, bool mouthOpen) {
  uint16_t body = invincible ? display.color565(255, 255, 255) : display.color565(255, 255, 0);
  const uint8_t (*spr)[4] = mouthOpen ? pacman2 : pacmanClosed;
  drawSprite4x4Cell(cellX, cellY, spr, dir, body);
}

// =====================================================
// 主遊戲循環：PacmanMode
// =====================================================
void PacmanMode() {
  // --- Pacman 狀態 ---
  static int pX = 1, pY = 1;
  static int pDir = 0; // 0右 1下 2左 3上
  static unsigned long lastUpdate = 0;
  static int mouthFrame = 0;
  static int lastM = -1;

  // 碰撞偵測用的前一幀位置
  static int prevPX = -1, prevPY = -1;
  static int prevGX = -1, prevGY = -1;

  // 用來偵測地圖切換
  static MapId lastMap = MAP_C1;

  // 勝利/失敗處理
  if (gameWin || gameOver) {
    if (gameWin) drawStatusText(true);
    else         drawStatusText(false);

    delay(5000); // 等待 5 秒

    // 重置遊戲狀態
    gameWin = false;
    gameOver = false;
    ModefirstRun = true;
    ghostInit = false;
    lastM = -1;
    powerBlockActive = true;
    invincible = false;
    invincibleUntil = 0;
    return;
  }

  // 初始化
  if (ModefirstRun) {
    setMazeById(MAP_C1);
    pX = 8; pY = 5;
    pDir = 2;
    mouthFrame = 0;
    prevPX = prevPY = prevGX = prevGY = -1;
    ghostInit = false;
    powerBlockActive = true;
    invincible = false;
    ModefirstRun = false;
    allDotsEaten = false;
    lastMap = curMapId;
  }

  // 豆子重置冷卻機制
  if (allDotsEaten && (millis() - dotsEatenTime > 60000)) {
    resetDots();
    allDotsEaten = false;
    powerBlockActive = true;
  }

  // 遊戲速度控制 (每 200ms 更新一次畫面)
  if (millis() - lastUpdate <= 200) return;
  lastUpdate = millis();
  mouthFrame++;

  // 記錄前一幀位置
  prevPX = pX; prevPY = pY;
  prevGX = gHX; prevGY = gHY;

  // 地圖切換檢查 (重畫 PowerBlock)
  if (curMapId != lastMap) {
    lastMap = curMapId;
    if (curMapId == MAP_L1 && powerBlockActive) drawPowerBlock();
  }

  // 清除 Pacman 舊影像
  display.fillRect(pX * 4, pY * 4, 4, 4, 0);

  // --- Pacman 移動邏輯 ---
  // 1. 在路口檢查是否隨機轉彎
  if (countChoicesNoBack(pX, pY, pDir) >= 2) {
    pDir = chooseTurnAtJunction(pX, pY, pDir, PAC_TURN_CHANCE);
  }

  int nextPX = pX + dx[pDir];
  int nextPY = pY + dy[pDir];

  // 2. 處理地圖邊界傳送 (L1 <-> C1 <-> R1)
  if (pDir == 0 && nextPX > 15) { // 向右出界
    if (curMapId == MAP_C1)      setMazeById(MAP_R1);
    else if (curMapId == MAP_L1) setMazeById(MAP_C1);
    nextPX = 0;
  } else if (pDir == 2 && nextPX < 0) { // 向左出界
    if (curMapId == MAP_C1)      setMazeById(MAP_L1);
    else if (curMapId == MAP_R1) setMazeById(MAP_C1);
    nextPX = 15;
  }

  // 3. 執行移動或撞牆轉向
  if (isWalkableCell(nextPX, nextPY)) {
    pX = nextPX;
    pY = nextPY;
  } else {
    // 撞牆了，找新方向
    pDir = chooseDirWhenBlocked(pX, pY, pDir);
    nextPX = pX + dx[pDir];
    nextPY = pY + dy[pDir];
    if (isWalkableCell(nextPX, nextPY)) {
      pX = nextPX;
      pY = nextPY;
    }
  }

  // --- 互動邏輯 ---
  // 吃道具 (變無敵)
  if (curMapId == MAP_L1 && isOnPowerBlock(pX, pY)) {
    powerBlockActive = false;
    clearPowerBlockArea();
    invincible = true;
  }

  // 吃豆子
  if (dotGet(pX, pY)) {
    dotSet(pX, pY, false);
    checkDotsStatus();
  }

  // 繪製 Pacman
  drawPacmanSprite(pX, pY, pDir, invincible, (mouthFrame % 2 == 0));

  // --- Ghost 邏輯 (只在 R1) ---
  if (curMapId == MAP_R1) {
    if (!ghostInit) {
      gHX = 14; gHY = 7;
      if (!isWalkableCell(gHX, gHY)) { gHX = 13; gHY = 7; } // 防卡住
      gHDir = 2;
      ghostInit = true;
    }

    display.fillRect(gHX * 4, gHY * 4, 4, 4, 0); // 清除舊鬼

    // 鬼的路口轉彎
    if (countChoicesNoBack(gHX, gHY, gHDir) >= 2) {
      gHDir = chooseTurnAtJunction(gHX, gHY, gHDir, GHOST_TURN_CHANCE);
    }

    int nextGX = gHX + dx[gHDir];
    int nextGY = gHY + dy[gHDir];

    // 鬼的移動與撞牆處理
    if (isWalkableCell(nextGX, nextGY)) {
      gHX = nextGX;
      gHY = nextGY;
    } else {
      gHDir = chooseDirWhenBlocked(gHX, gHY, gHDir);
      nextGX = gHX + dx[gHDir];
      nextGY = gHY + dy[gHDir];
      if (isWalkableCell(nextGX, nextGY)) {
        gHX = nextGX;
        gHY = nextGY;
      }
    }

    drawGhostCell(gHX, gHY);

    // 碰撞偵測 (Pacman vs Ghost)
    bool sameCell  = (gHX == pX && gHY == pY); // 同格
    bool crossSwap = (prevPX == gHX && prevPY == gHY && prevGX == pX && prevGY == pY); // 交換位置(擦身)

    if (sameCell || crossSwap) {
      if (invincible) {
        gameWin = true;
        gameWinAt = millis();
        drawStatusText(true);
      } else {
        gameOver = true;
        gameOverAt = millis();
        drawStatusText(false);
      }
      return;
    }
  } else {
    ghostInit = false; // 離開 R1 後重置鬼的狀態
  }

  // 時間顯示
  GetTime();
  drawClockOverlay(false);
}
