#include <iostream>
#include <string>
#include <windows.h>

using namespace std;

/**
 * Welcome to minesweeper.
 * I invite you to check out the README.md file before reviewing the code.
 */

// #region Source of truth
#define BOARD_SIZE 10
#define SKIP_QUESTIONS false


// #region ANSI CODES
#define ITALIC "\033[3m"
#define RESET_FORMAT "\033[0m"

// #region Colors
#define MAIN_COLOR "\033[38;5;27m"

#define NUM_1_COLOR "\033[38;5;21m"
#define NUM_2_COLOR "\033[38;5;28m"
#define NUM_3_COLOR "\033[38;5;160m"
#define NUM_4_COLOR "\033[38;5;18m"
#define NUM_5_COLOR "\033[38;5;88m"
#define NUM_6_COLOR "\033[38;5;31m"
#define NUM_7_COLOR "\033[38;5;255m"
#define NUM_8_COLOR "\033[38;5;242m"
// #endregion
// #endregion

// #region Render utils
#define CLEAR_TERMINAL system("cls")
#define SEPARATOR cout << "======================================" << endl
#define WRITE cout << string() +
#define ENDL << endl
#define SPACE cout << endl
#define ASK cin >>
// #endregion

// #region Render sprites
#define DEFAULT_SPRITE " ⬜ "
#define EMPTY_SPRITE " ⬛ "
#define MARKED_SPRITE " 🚩 "
#define MINE_SPRITE " 💣 "
#define MINE_EXPLODED_SPRITE " 💥 "
// #endregion



// #region Truth tables
// #region Cell types
#define IS_DEFAULT 0
#define IS_MARKED 1
#define IS_EMPTY 2
#define IS_BOMB 3
#define IS_NUMBER 4
// #endregion

// #region Neighbours
#define NEIGHBOUR_TOP_LEFT 0
#define NEIGHBOUR_TOP 1
#define NEIGHBOUR_TOP_RIGHT 2
#define NEIGHBOUR_RIGHT 3
#define NEIGHBOUR_BOTTOM_RIGHT 4
#define NEIGHBOUR_BOTTOM 5
#define NEIGHBOUR_BOTTOM_LEFT 6
#define NEIGHBOUR_LEFT 7
#define NUM_NEIGHBOURS 8
// #endregion

// #region State
// Poor man's enum
#define IS_IN_SETUP 0
#define IS_IN_GAME 1
#define IS_IN_END_SCREEN 2
// #endregion
// #endregion
// #endregion

int main() {
  // #region Setup
  srand(time(0));
  // AI reference for setting the console to utf-8 encoding
  SetConsoleOutputCP(CP_UTF8);
  SetConsoleCP(CP_UTF8);
  // End of AI reference
  // #endregion

  // #region Global variables
  int state = IS_IN_SETUP;
  bool didIncorrectInput = false;
  int boardSize;
  int nOfBombs;
  int truthBoard[BOARD_SIZE * BOARD_SIZE];
  int board[BOARD_SIZE * BOARD_SIZE];

  string persistentMessage = "";
  // #endregion

  // #region Runner
  while (true) {
    // #region Board cleanup
    for (size_t cellIndex = 0; cellIndex < BOARD_SIZE * BOARD_SIZE; cellIndex++) {
      truthBoard[cellIndex] = IS_EMPTY;
      board[cellIndex] = IS_DEFAULT;
    }
    // #endregion

    // #region Presentation
    /**
     * We could replace this bool-while with a do-while and check the output directly, but this is cleaner
     * more mantainable and more readable
     */
    bool isReady = false;
    while (!isReady) {
      // #region Render
      CLEAR_TERMINAL;

      SEPARATOR;
      SPACE;
      WRITE "             " + MAIN_COLOR + " MINESWEEPER" + RESET_FORMAT ENDL;
      WRITE "              " + ITALIC + "By: Adise" + RESET_FORMAT ENDL;
      SPACE;
      SEPARATOR;
      SPACE;
      WRITE MAIN_COLOR + "HOW TO PLAY:" + RESET_FORMAT ENDL;
      WRITE "  Type " + MAIN_COLOR + "[Column][Row][Action]" + RESET_FORMAT + " To pick a cell and perform the desired action.";
      WRITE ITALIC + " (Spacing does not matter)" + RESET_FORMAT ENDL;
      SPACE;
      WRITE "  Available actions:" ENDL;
      WRITE "  - " + MAIN_COLOR + "s" + RESET_FORMAT + " to show the cell. " + ITALIC + "(Default, if no action is typed it will execute this)" + RESET_FORMAT ENDL;
      WRITE "  - " + MAIN_COLOR + "m" + RESET_FORMAT + " to mark the cell as a potential mine." ENDL;
      SPACE;
      WRITE "  Examples:" ENDL;
      WRITE "  - " + MAIN_COLOR + "A1" + RESET_FORMAT + " To show cell at column A and row 1." ENDL;
      WRITE "  - " + MAIN_COLOR + "E7 m" + RESET_FORMAT + " To mark the cell at column E and row 7 as a potential mine." ENDL;
      SPACE;
      SPACE;
      SEPARATOR;
      if (didIncorrectInput) {
        SPACE;
        WRITE persistentMessage ENDL;

        persistentMessage = "";
      }
      SPACE;
      // #endregion

      // #region Asking
      WRITE "Are you ready? " + ITALIC + "(y/n)" + RESET_FORMAT ENDL;

      string result = "y";
      if (!SKIP_QUESTIONS) ASK result;
      char resultChar = tolower(result[0]);

      if (resultChar == 'n') {
        WRITE "Goodbye then!" ENDL;
        system("pause");
        return 0;
      }

      if (resultChar == 'y') {
        didIncorrectInput = false;
        isReady = true;
        break;
      }

      didIncorrectInput = true;
      persistentMessage += "Unknown response, please type \"y\" or \"n\"";
      // #endregion
    }

    system("pause");
    // #region How to play

    // #endregion
    // #endregion
  }
  // #endregion
}
