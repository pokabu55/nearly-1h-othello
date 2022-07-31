#include <stdio.h>
#include <stdlib.h>
///#include <conio.h>
#include "../include/gameInfo.hpp"

int main(void)
{
    // 初期化
    init();

    // 6-9 メインループ
    while (1) {
        // 石を置くマスを宣言する
        VEC2 placePosition;

        // 石を置くマスを選択する関数
        placePosition = inputPosition();
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

    cursorPosition.x = cursorPosition.y = 0;

    // ボードの描画
    drawScreen();

    switch (_getch()) {
        case "w":
            cursorPosition.y--;
            break;
        case "s":
            cursorPosition.y++;
            break;
        case "a":
            cursorPosition.x--;
            break;
        case "d":
            cursorPosition.x++;
            break;

    }
}

// ボードの描画
void drawScreen()
{
    // 画面をクリア
    system("cls");

    for(int y=0; y<BOARD_HEIGHT; y++) {
        for (int x=0; x<BOARD_WIDTH; x++) {

            // 石の描画
            printf("%s", diskAA[board[y][x]]);
        }

        // カーソルの描画
        if (y==cursorPosition.y) {
            printf("←");
        }

        printf("\n");
    }

    for(int y=0; y<BOARD_HEIGHT; y++) {
    }

    for (int x=0; x<BOARD_WIDTH; x++) {

        // カーソルの描画
        if (x==cursorPosition.x) {
            printf("↑");
        } else {
            printf("  ");
        }
    }

    printf("\n");
}

VEC2 inputPosition()
{
    // 置く位置を入力する

    // 置けるマスが選択されるまで無限ループ
    while (1) {
        drawScreen();
    }
}

