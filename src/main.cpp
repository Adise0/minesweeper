#include <conio.h>

#include <cstdlib>
#include <functional>
#include <iostream>
#include <regex>
#include <string>

/**
 * Please read README before grading
 */

using namespace std;

#define MATRIX_SIZE 10
#define DEV_MODE true

#pragma region Truth tables
#pragma region Cell type
#define IS_DEFAULT 0
#define IS_MARKED 1
#define IS_EMPTY 2
#define IS_BOMB 3
#define IS_NUMBER 4
#pragma endregion

#pragma region key codes
#define ARROW_UP_CODE 72
#define ARROW_DOWN_CODE 80
#define ARROW_LEFT_CODE 75
#define ARROW_RIGHT_CODE 77
#pragma endregion

#pragma region Arrow keys
#define ARROW_UP 0
#define ARROW_DOWN 1
#define ARROW_LEFT 2
#define ARROW_RIGHT 3
#pragma endregion

#pragma region Neighbours
#define NEIGHBOUR_TOP_LEFT 0
#define NEIGHBOUR_TOP 1
#define NEIGHBOUR_TOP_RIGHT 2
#define NEIGHBOUR_RIGHT 3
#define NEIGHBOUR_BOTTOM_RIGHT 4
#define NEIGHBOUR_BOTTOM 5
#define NEIGHBOUR_BOTTOM_LEFT 6
#define NEIGHBOUR_LEFT 7
#define NUM_NEIGHBOURS 8
#pragma endregion
#pragma endregion

int main() {
#pragma region Utils
    // Sorry Alberto, It just hurts too much to not use this....
    function<void(string line)> writeLine = [](string line) { cout << line << endl; };
    function<void(string line)> write = [](string line) { cout << line; };
    function<void()> space = []() { cout << endl; };
    function<void()> separator = []() { cout << "======================================" << endl; };
    function<string()> ask = []() {
        string result;
        getline(cin, result);
        return result;
    };
    function<int()> getArrowKey = []() {
        int key = _getch();

        switch (key) {
            case ARROW_UP_CODE:
                return ARROW_UP;
            case ARROW_DOWN_CODE:
                return ARROW_DOWN;
            case ARROW_LEFT_CODE:
                return ARROW_LEFT;
            case ARROW_RIGHT_CODE:
                return ARROW_RIGHT;
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
    int truthBoard[MATRIX_SIZE * MATRIX_SIZE];
    int board[MATRIX_SIZE * MATRIX_SIZE];

#pragma region Setup

    bool didIncorrectInput = false;

#pragma region How to play
    bool isReady = false;

    while (!isReady) {
#pragma region Rendering
        renderTitle();
        writeLine("\033[1mHOW TO PLAY:\033[0m");
        writeLine("  Type the cell row and column.");
        writeLine("  Once on the wanted cell, type:");
        writeLine("  - \033[38;5;32mm\033[0m To mark the cell for a potential mine");
        writeLine("  - \033[38;5;32ms\033[0m To open/show a cell");
        space();
        writeLine(" Format: ");
        writeLine(
            "  - \033[38;5;32m[Column][Row] [Action]\033[0m To pick the cell and "
            "action.");
        writeLine("  - Example: \033[38;5;32mA1 m\033[0m To pick the cell A1 and mark it");
        writeLine("  You will be promted to input any of the missing above\033[0m");
        space();
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
                if (DEV_MODE) result = "1";

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
        truthBoard[i] = 0;
        board[i] = 0;
    }

    for (size_t i = 0; i < nOfBombs; i++) {
        int row = rand() % boardSize;
        int col = rand() % boardSize;

        truthBoard[row * boardSize + col] = IS_BOMB;
    }
#pragma endregion

    isPlaying = true;
    string letters = "ABCDEFGHIJ";
    string keepMessage;

#pragma region Board Rendering

    function<void()> renderBoard = [&boardSize, &write, &board, &truthBoard, &space, &isPlaying,
                                    &letters, &writeLine, &keepMessage]() {
        for (size_t row = 0; row < (boardSize + 1); row++) {
            for (size_t col = 0; col < (boardSize + 1); col++) {
                if (col == 0 && row == 0) write("    ");
                if (col == 0 && row != 0) write((row < 10 ? " " : "") + to_string(row) + " ");
                if (row == 0) write(string() + letters[col] + "   ");
                if (row == 0 || col == 0) continue;

                int buffer = -1;
                int cellType =
                    (isPlaying ? board : truthBoard)[(row + buffer) * boardSize + (col + buffer)];
                switch (cellType) {
                    case IS_DEFAULT:
                        write(" ## ");
                        break;

                    case IS_MARKED:
                        write(" \033[38;5;124m##\033[0m ");
                        break;
                    case IS_EMPTY:
                        write(" \033[38;5;124m[]\033[0m ");
                        break;
                    case IS_NUMBER:
                        write(" \033[38;5;124m 1 \033[0m ");
                        break;
                    case IS_BOMB:
                        write(" \033[38;5;124m[]\033[0m ");
                        break;
                    default:
                        write(" \033[38;5;124m##\033[0m ");
                        break;
                }
            }
            space();
        }
        writeLine(keepMessage);
        keepMessage = "";
    };
#pragma endregion

#pragma region Game Loop

    string maxChar = string() + letters[boardSize - 1];
    regex inputRegX((boardSize >= 10 ? "^\\s*([A-" + maxChar + "])\\s*(10|[1-9])\\s*([ms])?\\s*$"
                                     : "^\\s*([A-" + maxChar + "])\\s*([1-" + to_string(boardSize) +
                                           "])\\s*([ms])?\\s*$"),
                    regex_constants::icase);

    function<void(int index, int neighbours[NUM_NEIGHBOURS])> getNeighbours =
        [&boardSize, &writeLine, &keepMessage](int index, int neighbours[NUM_NEIGHBOURS]) {
            bool isLeft = index % boardSize == 0;
            bool isRight = index % boardSize == 9;
            bool isTop = index / boardSize == 0;
            bool isBottom = int(index / boardSize) == 9;

            neighbours[NEIGHBOUR_TOP_LEFT] = (isTop || isLeft) ? -1 : index - boardSize - 1;
            neighbours[NEIGHBOUR_TOP] = isTop ? -1 : index - boardSize;
            neighbours[NEIGHBOUR_TOP_RIGHT] = (isTop || isRight) ? -1 : index - boardSize + 1;
            neighbours[NEIGHBOUR_RIGHT] = isRight ? -1 : index + 1;
            neighbours[NEIGHBOUR_BOTTOM_RIGHT] = (isBottom || isRight) ? -1 : index + boardSize + 1;
            neighbours[NEIGHBOUR_BOTTOM] = isBottom ? -1 : index + boardSize;
            neighbours[NEIGHBOUR_BOTTOM_LEFT] = (isBottom || isLeft) ? -1 : index + boardSize - 1;
            neighbours[NEIGHBOUR_LEFT] = isLeft ? -1 : index - 1;

            return neighbours;
        };

    while (isPlaying) {
        renderBoard();
        int row = -1;
        int col = -1;
        char action = '\0';

        string input = ask();
        try {
            // // AI for regex matching syntax in c++. Modified ofc
            smatch match;
            if (regex_match(input, match, inputRegX)) {
                char colChar = toupper(match[1].str()[0]);
                int rowNum = stoi(match[2].str());
                if (match[3].matched)
                    action = tolower(match[3].str()[0]);
                else
                    action = 's';

                col = colChar - 'A';
                row = rowNum - 1;
            }
            // End of syntax copy
        } catch (...) {
            writeLine("Error reading input");
            continue;
        }

        int cellIndex = row * boardSize + col;

        if (action == 'm') {
            board[cellIndex] = IS_MARKED;
        }

        if (action == 's') {
            writeLine(to_string(cellIndex) + " " + to_string(truthBoard[cellIndex]));
            if (truthBoard[cellIndex] == IS_BOMB) {
                isPlaying = false;
                break;
            }

            bool hasClearNeighbours = false;
            int neighbours[NUM_NEIGHBOURS];
            getNeighbours(cellIndex, neighbours);

            do {
                int bombCount = 0;
                hasClearNeighbours = false;
                for (size_t neighbour = 0; neighbour < NUM_NEIGHBOURS; neighbour++) {
                    if (neighbours[neighbour] == -1) continue;

                    int cellType = truthBoard[neighbours[neighbour]];
                    if (cellType == IS_EMPTY) {
                        hasClearNeighbours = true;
                    }
                }
            } while (hasClearNeighbours);

            system("cls");
        }
    }

    system("cls");
    writeLine("GAME OVER!");
    space();
    renderBoard();

#pragma endregion
    // Get input
    system("pause");

#pragma endregion

    return 0;
}
