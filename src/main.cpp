#include <stdio.h>
#include <stdlib.h>
///#include <conio.h>
#include "../include/gameInfo.hpp"
#include "../include/conio.h"
#include <vector>
#include <time.h>
#include <cstring>
#include <iostream>

int main(void)
{
    // 乱数をシャッフルする
    srand((unsigned int)time(NULL));

    // 開始ラベル
start:
    ; // 空文

    // ゲームモードを選択する
    selectMode();

    // 初期化
    init();

    int revCnt = 0;

    // 6-9 メインループ
    while (1) {

        // 置けるマスがあるかどうかをチェックする
        if (!checkCanPlaceAll(turn)) {
            // ターンを切り替える
            turn ^= 1;

            // 置けるマスがあるかどうかをチェックする
            if (!checkCanPlaceAll(turn)) {
                // 決着が付いたことにする
                turn = TURN_NONE;

                drawScreen();

                // キーボード入力を待つ
                getch();

                goto start;

            } else {
                // ターンをスキップして相手へ
                continue;
            }
        }

        // 石を置くマスを宣言する
        VEC2 placePosition;

        // 現在のターンがプレイヤーかどうかを判定する
        if (isPlayer[turn]) {
            // 石を置くマスを選択する関数
            placePosition = inputPosition();
        } else {
            // プレーヤーでないなら、
            // 盤面描画関数を呼ぶ
            drawScreen();

            // キーボード入力
            getch();

            // 次の手を決める
            bool random = false;//true;//
            searchNextTurn(placePosition, turn, random);
        }

        // 石をひっくり返す
        checkCanPlace(turn, placePosition, revCnt, true);

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

    // マスのスコアのセット
    setScore();

    // 中央に黒い石を置く
    board[4][3] = board[3][4] = TURN_BLACK;
    board[3][3] = board[4][4] = TURN_WHITE;

    // 黒のターンで初期化
    turn = TURN_BLACK;

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

        // プレイヤーの担当かどうか
        if (isPlayer[turn]) {

            // カーソルの描画
            if (y==cursorPosition.y) {
                printf("←");
            }
        }

        printf("\n");
    }

    // プレイヤーの担当かどうか
    if (isPlayer[turn]) {
        for (int x=0; x<BOARD_WIDTH; x++) {

            // カーソルの描画
            if (x==cursorPosition.x) {
                printf("↑");
            } else {
                printf("　");//(" ");//
            }
        }
    }

    // カーソルの描画が終わったら開業しておく
    printf("\n");

    // 決着が付いてないかどうかを判定する
    if (turn != TURN_NONE) {
        // ターンを表示する
        printf("\n%sのターンです\n", turnName[turn]);
    } else {
        // 決着が付いたなら
        int blackCount = getDiskCount(TURN_BLACK);
        int whiteCount = getDiskCount(TURN_WHITE);

        int winner = -1;

        if (blackCount >whiteCount) {
            winner = TURN_BLACK;
        }
        else if (whiteCount > blackCount) {
            winner = TURN_WHITE;
        }
        else {
            // 引き分け
            winner = TURN_NONE;
        }

        printf("%s%d--%s%d  ",
                turnName[TURN_BLACK],
                blackCount,
                turnName[TURN_WHITE],
                whiteCount
        );

        if (winner == TURN_NONE) {
            printf("draw game\n");
        } else {
            printf("the winner is %s.\n", turnName[winner]);
        }
    }

}

VEC2 inputPosition()
{
    // 置く位置を入力する

    int revCnt = 0;

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
                if (checkCanPlace(turn, cursorPosition, revCnt)) {
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

// 石を置けるかの判定、または石をひっくり返す関数
bool checkCanPlace(
    int     _color,     // 石の色
    VEC2    _position,  // 座標
    int     &reverseCount, // ひっくり返せる数
    bool    _turnOver   // ひっくり返すかどうか
){
    // 置けるフラグ
    bool canPlace = false;

    // リセット
    reverseCount = 0;

    // 対象座標に石があるか判定する
    if (board[_position.y][_position.x] != TURN_NONE) {
        // 石が既にあるので置けない、を返す
        return false;
    }

    // 置き石のカウント
    reverseCount += score_map[_position.y][_position.x];

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

        // いらなくね？
        //reverseCount += score_map[currentPosition.y][currentPosition.x];

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

                // ひっくり返しフラグが立っているかどうか判定する
                if (_turnOver) {
                    // ひっくり返す座標を宣言する
                    VEC2 reversePosition = _position;

                    // 隣のマスに移動する
                    reversePosition = vecAdd(reversePosition, directions[i]);

                    // 現在のターンの石が見つかるまで反復する
                    do {
                        // 相手の石をひっくり返す
                        board[reversePosition.y][reversePosition.x] = _color;
                        reverseCount += score_map[reversePosition.y][reversePosition.x];

                        // 隣のマスに移動する
                        reversePosition = vecAdd(reversePosition, directions[i]);

                    }while (board[reversePosition.y][reversePosition.x] != _color);

                }
            }
        }
    }

    return canPlace;
}

// 盤面上に石を置けるマスがあるかどうかを判定する
bool checkCanPlaceAll(int _color)
{
    int revCnt = 0;

    // 盤面をスキャン
    for (int y=0; y<BOARD_HEIGHT; y++) {
        for (int x=0; x<BOARD_WIDTH; x++) {
            // 判定座標の宣言
            VEC2 position = {x,y};

            // 対象座標に石を置けるかチェック
            if (checkCanPlace(_color, position, revCnt)) {
                // 置けます！
                return true;
            }
        }
    }

    // どこにも置けない
    return false;
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

int  getDiskCount(int _color)
{
    int count = 0;

    // 盤面のすべての石を数える
    for (int y=0; y<BOARD_HEIGHT; y++) {
        for (int x=0; x<BOARD_WIDTH; x++) {
            if (board[y][x] == _color) {
                count++;
            }
        }
    }

    return count;
}

void selectMode()
{
    // 初期化
    mode = MODE_1P;

    while(1) {
        // 画面のクリア
        system("clear");

        printf("select game mode\n");
        printf("\n\n");

        // モードの表示
        for (int i=0; i<MODE_MAX; i++) {

            // 現在のモードに＞をつける
            printf("%s  ",(i==mode)?">":" ");

            printf("%s\n",modeNames[i]);
            printf("\n");
        }

        // 入力で分岐
        switch(getch()) {
        case 'w':
            mode--;
            break;
        case 's':
            mode++;
            break;
        default:
            // 選択されたモードで分岐する
            switch (mode) {
            case MODE_1P:
                // AI対戦モード
                isPlayer[TURN_BLACK] = true;    // 黒をプレイヤー
                isPlayer[TURN_WHITE] = false;   // 白をAI
                break;
            case MODE_2P:
                // 人間どうし
                isPlayer[TURN_BLACK] = true;
                isPlayer[TURN_WHITE] = true;
                break;
            case MODE_WATCH:
                // AIどうし
                isPlayer[TURN_BLACK] = false;
                isPlayer[TURN_WHITE] = false;
                break;
            }

            // モード選択を抜ける
            return;
        }

        mode = (MODE_MAX+mode)%MODE_MAX;
    }
}

bool searchNextTurn(VEC2 &placePosition, int _turn, bool random)
{
    // 置ける座標を保持するvectorを宣言
    std::vector<VEC2> positoins;
    int revCnt = 0;

    // 盤面をスキャン
    for (int y=0; y<BOARD_HEIGHT; y++) {
        for (int x=0; x<BOARD_WIDTH; x++) {
            // 対象のマス
            VEC2 position = {x,y};

            // 置けるか判定する
            if (checkCanPlace(_turn, position, revCnt)) {
                // リストに追加
                positoins.push_back(position);
            }
        }
    }

    if (random) {
        // 置ける場所をランダムに取得する
        placePosition = positoins[rand() % positoins.size()];
        std::cout << "( " << placePosition.x << ", " << placePosition.y << " )" << std::endl;
        return true;
    }

    int reverseCount = 0;
    int maxCount = -1000;
    VEC2 maxPos;

    // 置ける位置で、ひっくり返せる数を数える
    for (const auto& e : positoins) {

        // TODO:次の評価に入る前に、盤面を元の状態に戻す必要がある

        std::cout << "( " << e.x << ", " << e.y << " )" << std::endl;

        // 現状のマスからコピー
        memcpy(board_tmp, board, BOARD_HEIGHT*BOARD_WIDTH*sizeof(int));

        // checkCanPlace関数で、ひっくり返し引数をTrueにする
        // checkCanPlace関数もひっくり返す数を数えるように変更する
        checkCanPlace(_turn, e, reverseCount, true);

        if (maxCount < reverseCount) {
            maxCount = reverseCount;
            maxPos = e;
        }
    }

    if (maxCount <= 0) return false;

    placePosition = maxPos;

    // よくわからんが、毎回返す
    return true;
}

void setScore()
{
    // マスのスコアのセット

    int score_tmp[] = {
         100, -200, 1, 1, 1, 1, -200,  100,
        -200, -200, 1, 1, 1, 1, -200, -200,
           1,    1, 1, 1, 1, 1,    1,    1,
           1,    1, 1, 1, 1, 1,    1,    1,
           1,    1, 1, 1, 1, 1,    1,    1,
           1,    1, 1, 1, 1, 1,    1,    1,
        -200, -200, 1, 1, 1, 1, -200, -200,
         100, -200, 1, 1, 1, 1, -200,  100
    };

    memcpy(score_map, score_tmp, sizeof(int)*BOARD_WIDTH*BOARD_HEIGHT);

}