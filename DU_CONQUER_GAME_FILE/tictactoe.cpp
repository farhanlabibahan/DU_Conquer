#include "raylib.h"
#include <iostream>
#include <string>
#include "global.h"
using namespace std;

#define CELL_SIZE 150
#define WINDOW_SIZE 450

const int PLAYER = 1;
const int BOT = 2;

int board[3][3] = {0};
double revealTime[3][3];
const double REVEAL_DURATION = 2.0;

// Forward declaration of checkWin to fix scope error
bool checkWin(int who);

bool xTurn = true;
bool botThinking = false;
double botStartTime = 0;
bool botMovePending  = false;
bool ttt = false;
string winner = "";
bool gamOVert = false;

Rectangle restartButton;

void init_tictactoe() {
    for (int r = 0; r < 3; r++)
        for (int c = 0; c < 3; c++) {
            board[r][c] = 0;
            revealTime[r][c] = -10;
        }

    xTurn = true;
    winner = "";
    gamOVert = false;
    botThinking = false;
    botMovePending = false;
    restartButton = { (float)screenWidth / 2 - 100, (float)screenHeight / 2 + 100, 200, 50 };
}

void unload_tictactoe() {
    // No textures to unload for now
}

void botMove() {
    // Try to block player if they have 2 in a row
    for (int r = 0; r < 3; ++r)
        for (int c = 0; c < 3; ++c)
            if (board[r][c] == 0) {
                board[r][c] = PLAYER;
                if (checkWin(PLAYER)) {
                    board[r][c] = BOT;
                    revealTime[r][c] = GetTime();
                    return;
                }
                board[r][c] = 0;
            }

    // Else random move
    for (int r = 0; r < 3; ++r) {
        for (int c = 0; c < 3; ++c) {
            if (board[r][c] == 0) {
                board[r][c] = BOT;
                revealTime[r][c] = GetTime();
                return;
            }
        }
    }
}

bool checkWin(int who) {
    for (int i = 0; i < 3; ++i)
        if ((board[i][0] == who && board[i][1] == who && board[i][2] == who) ||
            (board[0][i] == who && board[1][i] == who && board[2][i] == who))
            return true;
    if ((board[0][0] == who && board[1][1] == who && board[2][2] == who) ||
        (board[0][2] == who && board[1][1] == who && board[2][0] == who))
        return true;
    return false;
}

bool isFull() {
    for (int r = 0; r < 3; ++r)
        for (int c = 0; c < 3; ++c)
            if (board[r][c] == 0)
                return false;
    return true;
}

void logic_tictactoe() {
    if (gamOVert) {
        if (IsKeyPressed(KEY_R)) init_tictactoe();
        return;
    }

    Vector2 offset = {
        (float)(screenWidth - WINDOW_SIZE) / 2.0f,
        (float)(screenHeight - WINDOW_SIZE) / 2.0f
    };

    if (xTurn && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        Vector2 mouse = GetMousePosition();
        int col = (mouse.x - offset.x) / CELL_SIZE;
        int row = (mouse.y - offset.y) / CELL_SIZE;

        if (row >= 0 && row < 3 && col >= 0 && col < 3) {
            if (board[row][col] == 0) {
                board[row][col] = PLAYER;
                revealTime[row][col] = GetTime();

                if (checkWin(PLAYER)) {
                    winner = "Player";
                    ttt = true;
                    return;
                }
                if (isFull()) {
                    winner = "Draw";
                    gamOVert = true;
                    return;
                }

                xTurn = false;
                botThinking = true;
                botStartTime = GetTime();
            } else {
                // Clicked on filled cell = lose
                winner = "Bot";
                gamOVert = true;
            }
        }
    }

    if (!xTurn && botThinking && GetTime() - botStartTime >= 2.0) {
        botMove();

        if (checkWin(BOT)) {
            winner = "Bot";
            gamOVert = true;
        } else if (isFull()) {
            winner = "Draw";
            gamOVert = true;
        } else {
            xTurn = true;
        }
        botThinking = false;
    }
}

void draw_tictactoe() {
    Vector2 offset = {
        (float)(screenWidth - WINDOW_SIZE) / 2.0f,
        (float)(screenHeight - WINDOW_SIZE) / 2.0f
    };

    DrawRectangleRec({ offset.x, offset.y, WINDOW_SIZE, WINDOW_SIZE }, DARKGRAY);

    for (int i = 1; i < 3; ++i) {
        DrawRectangle(offset.x, offset.y + i * CELL_SIZE, WINDOW_SIZE, 4, LIGHTGRAY);
        DrawRectangle(offset.x + i * CELL_SIZE, offset.y, 4, WINDOW_SIZE, LIGHTGRAY);
    }

    for (int r = 0; r < 3; ++r) {
        for (int c = 0; c < 3; ++c) {
            if (board[r][c] != 0 && GetTime() - revealTime[r][c] <= REVEAL_DURATION) {
                const char* sym = board[r][c] == PLAYER ? "X" : "O";
                Color col = board[r][c] == PLAYER ? RED : SKYBLUE;
                DrawText(sym, offset.x + c * CELL_SIZE + 55, offset.y + r * CELL_SIZE + 25, 100, col);
            }
        }
    }

    if (gamOVert) {
        const char* msg = winner == "Draw" ? "Draw!" : (winner + " Wins!").c_str();
        int width = MeasureText(msg, 30);
        DrawText(msg, screenWidth / 2 - width / 2, offset.y + WINDOW_SIZE + 10, 30, YELLOW);

        if (winner != "Player") {
            DrawRectangleRec(restartButton, GRAY);
            DrawText("Press R to Restart", restartButton.x + 10, restartButton.y + 10, 20, WHITE);
        }
    }

    if (ttt) {
        philosophy_game = true;
    }
}
