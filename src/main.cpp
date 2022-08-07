#include <stdio.h>
#include <stdlib.h>
///#include <conio.h>
#include "../include/gameInfo.hpp"
#include "../include/conio.h"

int main(void)
{
    // 初期化
    init();

    // 黒のターンで初期化
    turn = TURN_BLACK;

    // 6-9 メインループ
    while (1) {
        // 石を置くマスを宣言する
        VEC2 placePosition;

        // 石を置くマスを選択する関数
        placePosition = inputPosition();

        // 石を置く
        board[placePosition.y][placePosition.x] = turn;

        // ターンを変える
        // ビット排他的論理和代入だって
        turn ^= 1;
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

    // カーソル位置の初期化
    cursorPosition = {3,3};

    // ボードの描画
    drawScreen();
}

// ボードの描画
void drawScreen()
{
    // 画面をクリア winは、clsだったので変更
    system("clear");

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

    for (int x=0; x<BOARD_WIDTH; x++) {

        // カーソルの描画
        if (x==cursorPosition.x) {
            printf("↑");
        } else {
            printf(" ");//　");
        }
    }

    // ターンを表示する
    printf("\n%sのターンです\n", turnName[turn]);

    printf("\n");
}

VEC2 inputPosition()
{
    // 置く位置を入力する

    // 置けるマスが選択されるまで無限ループ
    while (1) {
        drawScreen();

           // 入力されたキーによって分岐する
        switch (getch()) {
            case 'w':
                cursorPosition.y--;
                break;
            case 's':
                cursorPosition.y++;
                break;
            case 'a':
                cursorPosition.x--;
                break;
            case 'd':
                cursorPosition.x++;
                break;
            default:
                // 上記以外のキー入力されたら、カーソル座標を返す

                // カーソル位置に置けるかの判定
                if (checkCanPlace(turn, cursorPosition)) {
                    return cursorPosition;
                } else {
                    // 置けなければ
                    printf("そこには置けないよ");

                    // 入力を待つ
                    getch();
                }
                break;
        }

        // カーソルを左右ループさせる
        cursorPosition.x = (BOARD_WIDTH+cursorPosition.x) % BOARD_WIDTH;
        // カーソルを上下ループさせる
        cursorPosition.y = (BOARD_HEIGHT+cursorPosition.y) % BOARD_HEIGHT;
    }
}

bool checkCanPlace(
    int     _color,     // 石の色
    VEC2    _position   // 座標
){
    // 置けるフラグ
    bool canPlace = false;

    // 対象座標に石があるか判定する
    if (board[_position.y][_position.x] != TURN_NONE) {
        // 石が既にあるので置けない、を返す
        return false;
    }

    // 相手の石の色を宣言する
    int opponent = _color^1;

    // すべての方向を反復する
    for (int i=0; i<DIRECTION_MAX; i++) {
        // 現在チェック中の座標を宣言する
        VEC2 currentPosition = _position;

        // 隣のマスに移動する
        currentPosition = vecAdd(_position, directions[i]);

        // 相手の石でないか判定する
        if (board[currentPosition.y][currentPosition.x] != opponent){
            //相手の石でなければ、その方向のチェックを中断する
            continue;
        }

        // 無限ループする
        while(1) {
            // 隣のマスへ移動する
            currentPosition = vecAdd(currentPosition,directions[i]);

            // チェックするマスが盤面の範囲内かチェック
            if ((currentPosition.x < 0) ||
                (currentPosition.x >= BOARD_WIDTH) ||
                (currentPosition.y < 0) ||
                (currentPosition.y >= BOARD_HEIGHT)) {
                break;
            }

            // チェックするマスに石がないかどうかを判定
            if (board[currentPosition.y][currentPosition.x] == TURN_NONE) {
                // 石がなければ、現在の方向のチェックをやめる
                break;
            }

            // チェックするマスに自分の石があれば
            if (board[currentPosition.y][currentPosition.x] == _color) {
                // 石が置けることが確定する
                canPlace = true;
            }
        }
    }

    return canPlace;
}

VEC2 vecAdd(
    VEC2 _v0,   // 
    VEC2 _v1    // 
){
    // 加算したベクトルを返す
    return {
        _v0.x + _v1.x,
        _v0.y + _v1.y
    };
}