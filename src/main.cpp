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

    // Global
    int boardSize;
    int nOfBombs;

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

        string result = ask();
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
        string result = ask();
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
        string result = ask();

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

    return 0;
}
