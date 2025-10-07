#include <cstdlib>
#include <functional>
#include <iostream>
#include <string>

using namespace std;

#define MATRIX_SIZE 10

int main() {
#pragma region Utils
    // Sorry Alberto, It just hurts too much to not use this....
    function<void(string line)> writeLine = [](string line) { cout << line << endl; };
    function<void(string line)> write = [](string line) { cout << line; };
    function<void()> space = []() { cout << endl; };
    function<void()> separator = []() { cout << "======================================" << endl; };
    function<void(string & result)> ask = [](string& result) { cin >> result; };
#pragma endregion

#pragma region GameMode

    bool didIncorrectInput = false;
    bool hasChosenGameMode = false;
    int gameMode = 0;

    while (!hasChosenGameMode) {
#pragma region Rendering
        separator();
        space();
        writeLine("             \033[1;38;5;32mMINESWEEPER\033[0m");
        writeLine("              \033[3mBy: Adise\033[0m");
        space();
        separator();
        space();
        writeLine("\033[1mGAME MODE:\033[0m");
        writeLine("  Select a game mode:");
        write("  - ");
        writeLine(string(gameMode == 0 ? "\033[4;38;5;32m" : "") + "[0] Type to play\033[0m");
        write("  - ");
        writeLine(string(gameMode == 1 ? "\033[4;38;5;32m" : "") +
                  "[1] Play with arrow keys\033[0m");
        space();
        separator();
        if (didIncorrectInput) {
            space();
            writeLine("Unknown response, please type \"0\" or \"1\" OR move with the arrow keys!");
        }
        space();
#pragma endregion

        string result;
        ask(result);
        if (!result._Equal("0") && !result._Equal("1")) {
            didIncorrectInput = true;
            system("cls");
            continue;
        }
        didIncorrectInput = false;  // Unused but for clarity
        gameMode = stoi(result);
    }

    // writeLine("\033[1mHOW TO PLAY:\033[0m");
    // writeLine("  Move your selected cell with the arrow keys!");
    // writeLine("  Once on the wanted cell, type:");
    // writeLine("  - \033[3mm\033[0m To mark the cell for a potential mine");
    // writeLine("  - \033[3ms\033[0m To open/show a cell");
    // space();
    // separator();
    // space();
#pragma endregion

    system("pause");  // TEMP
}
