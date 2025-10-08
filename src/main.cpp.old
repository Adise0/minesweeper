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
#define DEV_MODE false

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
    bool isRunning = true;

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
    while (isRunning) {
        bool isPlaying = true;
        int boardSize;
        int nOfBombs;
        int truthBoard[MATRIX_SIZE * MATRIX_SIZE];
        int board[MATRIX_SIZE * MATRIX_SIZE];

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
                writeLine("  Input a board size \033[3m(4-10)\033[0m");
            else {
                writeLine("  Board size -> \033[38;5;32m" + to_string(boardSize) + "x" +
                          to_string(boardSize) + "\033[0m");
            }
            if (isSizePicked) {
                if (!isNOfBombsPicked) {
                    writeLine("  Input the number of bombs \033[3m(1-" + to_string(maxBombs) +
                              ")\033[0m");
                    writeLine("   \033[3mRecommended: " + to_string(int(maxBombs * 0.20)) +
                              "\033[0m");
                } else {
                    writeLine("  Number of bombs -> \033[38;5;32m" + to_string(nOfBombs) +
                              "\033[0m");
                }
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
                    if (boardSize < 4 || boardSize > 10) {
                        didIncorrectInput = true;
                        errorMessage = "Invalid size, please type a number between 4 and 10";
                        continue;
                    }
                } catch (...) {
                    didIncorrectInput = true;
                    errorMessage = "Invalid input, please type a number";
                    continue;
                }

                // Size of the board - 8 (neighbours) for the start - 2 (One for the initial cell
                // AND one for another free cell at least. else the game would insta end)
                maxBombs = boardSize * boardSize - NUM_NEIGHBOURS - 2;
                isSizePicked = true;
                didIncorrectInput = false;
                continue;
            }
            if (!isNOfBombsPicked) {
                try {
                    if (DEV_MODE) result = "10";

                    nOfBombs = stoi(result);
                    if (nOfBombs < 1 || nOfBombs > maxBombs) {
                        didIncorrectInput = true;
                        errorMessage =
                            "Invalid number of bombs, please type a number between 1 and " +
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

        string letters = "ABCDEFGHIJ";
        string keepMessage = "";

        for (size_t cellIndex = 0; cellIndex < boardSize * boardSize; cellIndex++) {
            truthBoard[cellIndex] = IS_EMPTY;
            board[cellIndex] = IS_DEFAULT;
        }

#pragma region Board Rendering

        function<void()> renderBoard = [&boardSize, &write, &board, &truthBoard, &space, &isPlaying,
                                        &letters, &writeLine, &keepMessage]() {
            system("cls");
            for (size_t row = 0; row < (boardSize + 1); row++) {
                for (size_t col = 0; col < (boardSize + 1); col++) {
                    if (col == 0 && row == 0) write("    ");
                    if (col == 0 && row != 0) write((row < 10 ? " " : "") + to_string(row) + " ");
                    if (row == 0 && col != 0) write(string() + letters[col - 1] + "   ");
                    if (row == 0 || col == 0) continue;

                    int buffer = -1;
                    int cellType =
                        (isPlaying ? board
                                   : truthBoard)[(row + buffer) * boardSize + (col + buffer)];

                    if (cellType > IS_NUMBER) {
                        write("\033[38;5;");
                        switch (cellType - IS_NUMBER) {
                            case 1:
                                write("21m");
                                break;
                            case 2:
                                write("28m");
                                break;
                            case 3:
                                write("160m");
                                break;
                            case 4:
                                write("18m");
                                break;
                            case 5:
                                write("88m");
                                break;
                            case 6:
                                write("31m");
                                break;
                            case 7:
                                write("255m");
                                break;
                            case 8:
                                write("242m");
                                break;
                        }
                        write(" " + to_string(cellType - IS_NUMBER) + "  \033[0m");
                        continue;
                    }
                    switch (cellType) {
                        case IS_DEFAULT:
                            write(" ## ");
                            break;
                        case IS_MARKED:
                            write(" \033[38;5;124m##\033[0m ");
                            break;
                        case IS_EMPTY:
                            write(" __ ");
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
            space();
            keepMessage = "";
        };
#pragma endregion

#pragma region Game Loop

        string maxChar = string() + letters[boardSize - 1];
        regex inputRegX(
            (boardSize >= 10 ? "^\\s*([A-" + maxChar + "])\\s*(10|[1-9])\\s*([ms])?\\s*$"
                             : "^\\s*([A-" + maxChar + "])\\s*([1-" + to_string(boardSize) +
                                   "])\\s*([ms])?\\s*$"),
            regex_constants::icase);

        function<void(int index, int neighbours[NUM_NEIGHBOURS])> getNeighbours =
            [&boardSize, &writeLine, &keepMessage](int index, int neighbours[NUM_NEIGHBOURS]) {
                bool isLeft = index % boardSize == 0;
                bool isRight = index % boardSize == boardSize - 1;
                bool isTop = index / boardSize == 0;
                bool isBottom = int(index / boardSize) == boardSize - 1;

                neighbours[NEIGHBOUR_TOP_LEFT] = (isTop || isLeft) ? -1 : index - boardSize - 1;
                neighbours[NEIGHBOUR_TOP] = isTop ? -1 : index - boardSize;
                neighbours[NEIGHBOUR_TOP_RIGHT] = (isTop || isRight) ? -1 : index - boardSize + 1;
                neighbours[NEIGHBOUR_RIGHT] = isRight ? -1 : index + 1;
                neighbours[NEIGHBOUR_BOTTOM_RIGHT] =
                    (isBottom || isRight) ? -1 : index + boardSize + 1;
                neighbours[NEIGHBOUR_BOTTOM] = isBottom ? -1 : index + boardSize;
                neighbours[NEIGHBOUR_BOTTOM_LEFT] =
                    (isBottom || isLeft) ? -1 : index + boardSize - 1;
                neighbours[NEIGHBOUR_LEFT] = isLeft ? -1 : index - 1;

                return neighbours;
            };

        function<void(int neighbours[], int cellIndex)> showNeighbours =
            [&board, &truthBoard, &getNeighbours, &showNeighbours, &writeLine](int neighbours[],
                                                                               int cellIndex) {
                if (board[cellIndex] != IS_DEFAULT) return;

                board[cellIndex] = truthBoard[cellIndex];
                if (truthBoard[cellIndex] != IS_EMPTY) return;

                int nestedNeighbours[NUM_NEIGHBOURS];
                getNeighbours(cellIndex, nestedNeighbours);
                for (size_t neighbourIndex = 0; neighbourIndex < NUM_NEIGHBOURS; neighbourIndex++) {
                    showNeighbours(nestedNeighbours, nestedNeighbours[neighbourIndex]);
                }
            };

        int row = -1;
        int col = -1;
        char action = '\0';
        int cell = row * boardSize + col;

        function<void()> getInput = [&row, &col, &action, &ask, &inputRegX, &keepMessage,
                                     &boardSize, &cell]() {
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

                    cell = row * boardSize + col;
                }
                // End of syntax copy
            } catch (...) {
                keepMessage +=
                    "\nInvalid input. Format is:\n"
                    "- \033[38;5;32m[Column][Row] [Action]\033[0m To pick the cell and "
                    "action.\n"
                    "OR"
                    "- \033[38;5;32m[Column][Row]\033[0m To pick the cell and "
                    "show the cell\n";
            }
        };

        do {
            renderBoard();
            getInput();
            if (action != 's') {
                keepMessage += "\n Please open an initial cell";
            }
        } while (action != 's');

#pragma region Generation

        for (size_t i = 0; i < nOfBombs; i++) {
            int bombIndex;

            // Safety while to make sure no bomb is generated on the starting cell
            bool isTooClose = false;
            do {
                isTooClose = false;
                int row = rand() % boardSize;
                int col = rand() % boardSize;
                bombIndex = row * boardSize + col;

                int bombNeighbours[NUM_NEIGHBOURS];
                getNeighbours(bombIndex, bombNeighbours);

                for (size_t neighbourIndex = 0; neighbourIndex < NUM_NEIGHBOURS; neighbourIndex++) {
                    if (bombNeighbours[neighbourIndex] == -1) continue;
                    if (bombNeighbours[neighbourIndex] == cell) isTooClose = true;
                }

            } while (bombIndex == cell || truthBoard[bombIndex] == IS_BOMB || isTooClose);

            truthBoard[bombIndex] = IS_BOMB;
        }

        for (size_t cellIndex = 0; cellIndex < boardSize * boardSize; cellIndex++) {
            if (truthBoard[cellIndex] == IS_BOMB) continue;

            int neighbours[NUM_NEIGHBOURS];
            getNeighbours(cellIndex, neighbours);

            int neighbouringBombs = 0;
            space();
            for (size_t neighbourIndex = 0; neighbourIndex < NUM_NEIGHBOURS; neighbourIndex++) {
                if (neighbours[neighbourIndex] == -1) continue;  // Ingore neighbours outside bounds
                if (truthBoard[neighbours[neighbourIndex]] == IS_BOMB) neighbouringBombs++;
            }

            if (neighbouringBombs == 0)
                truthBoard[cellIndex] = IS_EMPTY;
            else {
                truthBoard[cellIndex] =
                    IS_NUMBER + neighbouringBombs;  // Attach number directly to the cell type
            }
        }
#pragma endregion

        int neighbours[NUM_NEIGHBOURS];
        getNeighbours(cell, neighbours);
        showNeighbours(neighbours, cell);

        bool hasWon = false;

        while (isPlaying) {
            renderBoard();
            getInput();

            if (action == 'm') {
                board[cell] = IS_MARKED;
            }

            if (action == 's') {
                writeLine(to_string(cell) + " " + to_string(truthBoard[cell]));
                if (truthBoard[cell] == IS_BOMB) {
                    hasWon = false;
                    isPlaying = false;
                    break;
                }

                if (board[cell] == IS_DEFAULT || board[cell] == IS_MARKED) {
                    int neighbours[NUM_NEIGHBOURS];
                    getNeighbours(cell, neighbours);
                    showNeighbours(neighbours, cell);
                }
            }

            for (size_t cellIndex = 0; cellIndex < boardSize * boardSize; cellIndex++) {
                if (truthBoard[cellIndex] == IS_BOMB) continue;
                if (board[cellIndex] == IS_DEFAULT) break;
                if (board[cellIndex] == IS_MARKED) break;

                isPlaying = false;
                hasWon = true;
            }
        }

        system("cls");

        renderBoard();
        space();
        separator();
        space();
        writeLine("             " + string() +
                  (hasWon ? "\033[1;38;5;32mYOU WON!" : "\033[1;38;5;160mGAME OVER!") + "\033[0m");
        if (!hasWon) writeLine("              \033[3mGet good\033[0m");

        space();
        separator();
        space();

        while (true) {
            writeLine("Fancy another game? (y/n)");
            string result;
            result = ask();
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

            break;
        }
    }

#pragma endregion
#pragma endregion

    return 0;
}
