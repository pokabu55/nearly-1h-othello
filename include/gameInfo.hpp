// ゲームの定数定義
#define BOARD_WIDTH (8)
#define BOARD_HEIGHT (8)

// 各マスの状態
int board[BOARD_HEIGHT][BOARD_WIDTH];

// ターンの種類
enum {
    TURN_BLACK, // 黒
    TURN_WHITE, // 白
    TURN_NONE,  // なし
    TURN_MAX    // ターンの最大数
};

// 方向の種類を定義する
enum {
    DIRECTION_UP,
    DIRECTION_UP_LEFT,
    DIRECTION_LEFT,
    DIRECTION_DOWN_LEFT,
    DIRECTION_DOWN,
    DIRECTION_DOWN_RIGHT,
    DIRECTION_RIGHT,
    DIRECTION_UP_RIGHT,
    DIRECTION_MAX
};

// ゲームモード
enum {
    MODE_1P,    // COMと対戦
    MODE_2P,    // 人間どうし
    MODE_WATCH, // COMどうし
    MODE_MAX    // モードの数
};

// 石のアスキーアート
const char* diskAA[TURN_MAX]=
{
    //"○",    // black
    //"●",    // white
    //"□"     // none
    "Ｘ",   // black
    "＠",   // white
    "・"    // none
};

// モード名
const char* modeNames[] = {
    "1P GAME",
    "2P GAME",
    "WATCH"
};

// ベクトル構造体
typedef struct
{
    // 座標
    int x, y;
}VEC2;

const char* turnName[] = {
    //"黒",   // TURN_BLACK
    //"白"    // TURN_WHITE
    "Ｘ",     // black
    "＠"      // white
};

//方向を宣言する
VEC2 directions[DIRECTION_MAX] = {
    { 0,-1},    // 上
    {-1,-1},    // 左上
    {-1, 0},    // 左
    {-1, 1},    // 左下
    { 0, 1},    // 下
    { 1, 1},    // 右下
    { 1, 0},    // 右
    { 1,-1}     // 右上
};

VEC2 cursorPosition;    // 現在のカーソル位置
int  turn;              // 現在のターンの宣言
int  mode;              // ゲームモードの番号
bool isPlayer[TURN_MAX];// 各ターンがプレーヤーかどうかの宣言

// 関数
void init();
void drawScreen();
VEC2 inputPosition();
bool checkCanPlace(int _color, VEC2 _position, bool _turnOver = false);
bool checkCanPlaceAll(int _color);
VEC2 vecAdd(VEC2 _v0, VEC2 _v1);
int  getDiskCount(int _color);
void selectMode();
