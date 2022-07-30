#include <stdio.h>
#include "../include/gameInfo.hpp"

int main(void)
{
    // 初期化
    init();

    // 6-9 メインループ
    while (1)
    {
        /* code */
    }
    

}


// ゲームの初期化
void init()
{
    for(int y=0; y<BOARD_HEIGHT; y++) {
        for (int x=0; x<BOARD_WIDTH; x++) {
            // 石が置いていない状態で初期化
            board[y][x] = TURN_NONE;
        }
    }

    // 中央に黒い石を置く
    board[4][3] = board[3][4] = TURN_BLACK;
    board[3][3] = board[4][4] = TURN_WHITE;

    // ボードの描画
    drawScreen();
}

// ボードの描画
void drawScreen()
{
    for(int y=0; y<BOARD_HEIGHT; y++) {
        for (int x=0; x<BOARD_WIDTH; x++) {
            // 石の描画
            printf("%s", diskAA[board[y][x]]);
        }
        printf("\n");
    }
}
