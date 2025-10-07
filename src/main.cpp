#include <conio.h>

#include <cstdlib>
#include <functional>
#include <iostream>
#include <string>

/**
 * Please read README before grading
 */

using namespace std;

#define MATRIX_SIZE 10
#define DEV_MODE true

// Truth table
#define IS_DEFAULT 0
#define IS_MARKED 1
#define IS_EMPTY 2
#define IS_NUMBER 3
#define IS_BOMB 4

int main() {
#pragma region Utils
    // Sorry Alberto, It just hurts too much to not use this....
    function<void(string line)> writeLine = [](string line) { cout << line << endl; };
    function<void(string line)> write = [](string line) { cout << line; };
    function<void()> space = []() { cout << endl; };
    function<void()> separator = []() { cout << "======================================" << endl; };
    function<string()> ask = []() {
        string result;
        cin >> result;
        return result;
    };
    function<int()> getArrowKey = []() {
        int key = _getch();

        switch (key) {
            case 72:
                return 0;
            case 80:
                return 1;
            case 77:
                return 2;
            case 75:
                return 3;
        }
    };
    function<void()> renderTitle = [&separator, &space, &writeLine]() {
        system("cls");
        separator();
        space();
        writeLine("             \033[1;38;5;32mMINESWEEPER\033[0m");
        writeLine("              \033[3mBy: Adise\033[0m");
        space();
        separator();
        space();
    };

#pragma endregion

    srand(time(0));
    // Global
    int boardSize;
    int nOfBombs;
    bool isPlaying;
    int board[MATRIX_SIZE * MATRIX_SIZE];

#pragma region Setup

    bool didIncorrectInput = false;

#pragma region GameMode
    bool hasChosenGameMode = false;
    int gameMode = 0;

    while (!hasChosenGameMode) {
#pragma region Rendering
        renderTitle();
        writeLine("\033[1mGAME MODE:\033[0m");
        writeLine("  Select a game mode:");
        writeLine("  - \033[38;5;32m[0]\033[0m Type to play");
        writeLine("  - \033[38;5;32m[1]\033[0m Play with arrow keys");
        space();
        separator();
        if (didIncorrectInput) {
            space();
            writeLine("Unknown response, please type \"0\" or \"1\"");
        }
        space();
#pragma endregion
        string result;
        if (!DEV_MODE)
            result = ask();
        else
            result = "0";

        if (!result._Equal("0") && !result._Equal("1")) {
            didIncorrectInput = true;
            system("cls");
            continue;
            /**
             * My take on flow breaking elements:
             *
             * Using flow breaking elements is NOT a bad practice!
             * They are a very poowerfull tool to avoid extra if statements and bool flag checks.
             *
             * I don't know from where you guys got the idea that flow break elements are bad
             * practices. I agree that goto is the exception, using goto is not a good approach.
             *
             * For example, continue here keeps the code lean and clean, since it avoids an extra
             * check for the code bellow. I'd compare this to:
             *
             * if(condition){return;}
             * else {//Do stuff}
             *
             * else here is unecessary. continue; provides the same value as return does in the code
             * above.
             *
             * Tremenda chapa
             */
        }
        gameMode = stoi(result);
        hasChosenGameMode = true;
        didIncorrectInput = false;
    }
#pragma endregion

#pragma region How to play
    bool isReady = false;

    while (!isReady) {
#pragma region Rendering
        renderTitle();
        writeLine("\033[1mHOW TO PLAY:\033[0m");

        if (gameMode == 1) {
            writeLine("  Move your selected cell with the \033[38;5;32marrow keys!\033[0m");
        } else {
            writeLine("  Type the cell row and column.");
        }

        writeLine("  Once on the wanted cell, type:");
        writeLine("  - \033[38;5;32mm\033[0m To mark the cell for a potential mine");
        writeLine("  - \033[38;5;32ms\033[0m To open/show a cell");

        if (gameMode == 0) {
            space();
            writeLine(" Format: ");
            writeLine(
                "  - \033[38;5;32m[Column][Row] [Action]\033[0m To pick the cell and "
                "action.");
            writeLine("  - Example: \033[38;5;32mA1 m\033[0m To pick the cell A1 and mark it");
            writeLine("  You will be promted to input any of the missing above\033[0m");
            space();
        }
        space();
        separator();
        if (didIncorrectInput) {
            space();
            writeLine("Unknown response, please type \"y\" or \"n\"");
        }
        space();
#pragma endregion

        writeLine("You ready? (y/n)");
        string result;
        if (!DEV_MODE)
            result = ask();
        else
            result = "y";
        char resultChar = tolower(result[0]);

        if (resultChar != 'y' && resultChar != 'n') {
            space();
            writeLine("Unknown response, please type \"y\" or \"n\"");
            space();
            didIncorrectInput = true;
            continue;
        }

        if (resultChar == 'n') {
            writeLine("Goodbye then!");
            system("pause");
            return 0;
        }
        didIncorrectInput = false;
        isReady = true;
    }
#pragma endregion

#pragma region Board setup

    isReady = false;
    string errorMessage = "";
    bool isSizePicked = false;
    bool isNOfBombsPicked = false;
    int maxBombs;

    while (!isReady) {
#pragma region Rendering
        renderTitle();
        writeLine("\033[1mBOARD SETUP:\033[0m");
        if (!isSizePicked)
            writeLine("  Input a board size \033[3m(1-10)\033[0m");
        else {
            writeLine("  Board size -> \033[38;5;32m" + to_string(boardSize) + "x" +
                      to_string(boardSize) + "\033[0m");
        }
        if (!isNOfBombsPicked) {
            writeLine("  Input the number of bombs \033[3m(1-" + to_string(maxBombs) + ")\033[0m");
        } else {
            writeLine("  Number of bombs -> \033[38;5;32m" + to_string(nOfBombs) + "\033[0m");
        }
        space();
        separator();
        if (didIncorrectInput) {
            space();
            writeLine(errorMessage);
        }
        space();
        if (isNOfBombsPicked && isSizePicked) writeLine("You ready? (y/n)");
#pragma endregion
        string result;
        if (!DEV_MODE)
            result = ask();
        else
            result = "10";

        if (!isSizePicked) {
            try {
                boardSize = stoi(result);
                if (boardSize < 1 || boardSize > 10) {
                    didIncorrectInput = true;
                    errorMessage = "Invalid size, please type a number between 1 and 10";
                    continue;
                }
            } catch (...) {
                didIncorrectInput = true;
                errorMessage = "Invalid input, please type a number";
                continue;
            }

            maxBombs = boardSize * boardSize - 1;
            isSizePicked = true;
            didIncorrectInput = false;
            continue;
        }
        if (!isNOfBombsPicked) {
            try {
                if (DEV_MODE) result = "50";

                nOfBombs = stoi(result);
                if (nOfBombs < 1 || nOfBombs > maxBombs) {
                    didIncorrectInput = true;
                    errorMessage = "Invalid number of bombs, please type a number between 1 and " +
                                   to_string(maxBombs);
                    continue;
                }
            } catch (...) {
                didIncorrectInput = true;
                errorMessage = "Invalid input, please type a number";
                continue;
            }
            didIncorrectInput = false;
            isNOfBombsPicked = true;
            continue;
        }

        if (DEV_MODE) result = "y";
        char resultChar = tolower(result[0]);
        if (resultChar != 'y' && resultChar != 'n') {
            didIncorrectInput = true;
            errorMessage = "Unknown response, please type \"y\" or \"n\"";
            continue;
        }

        if (resultChar == 'n') {
            isSizePicked = false;
            isNOfBombsPicked = false;
            didIncorrectInput = false;
            continue;
        }

        isReady = true;
    }
#pragma endregion
#pragma endregion

    system("cls");

#pragma region Generation
    for (size_t i = 0; i < boardSize * boardSize; i++) {
        board[i] = 0;
    }

    for (size_t i = 0; i < nOfBombs; i++) {
        int row = rand() % boardSize;
        int col = rand() % boardSize;

        board[row * boardSize + col] = IS_BOMB;
    }
#pragma endregion

#pragma region Game Loop

    isPlaying = true;
    string letters = "ABCDEFGHIJ";

    while (isPlaying) {
#pragma region Rendering
        for (size_t row = 0; row < (gameMode == 0 ? (boardSize + 1) : boardSize); row++) {
            for (size_t col = 0; col < (gameMode == 0 ? (boardSize + 1) : boardSize); col++) {
                if (gameMode == 0) {
                    if (col == 0 && row == 0) write("    ");
                    if (col == 0 && row != 0) write(to_string(row) + "  ");
                    if (row == 0) write(string() + letters[col] + "  ");
                    if (row == 0 || col == 0) continue;
                }

                int buffer = gameMode == 0 ? -1 : 0;
                int cellType = board[(row + buffer) * boardSize + (col + buffer)];
                switch (cellType) {
                    case IS_DEFAULT:
                        write(" ## ");
                        break;

                    case IS_MARKED:
                        write(" \033[38;5;124m##\033[0m ");
                        break;
                    case IS_EMPTY:
                        write(" \033[38;5;124m[ ]\033[0m ");
                        break;
                    case IS_NUMBER:
                        write(" \033[38;5;124m 1 \033[0m ");
                        break;
                    case IS_BOMB:
                        write(" \033[38;5;124m[ ]\033[0m ");
                        break;
                    default:
                        write("Bruh?");
                        break;
                }
            }
            space();
        }
#pragma endregion
        // Get input
        system("pause");
    }

#pragma endregion

    return 0;
}
