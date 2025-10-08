#include <iostream>
#include <string>
#include <windows.h>

using namespace std;

/**
 * Welcome to minesweeper.
 * I invite you to check out the README.md file before reviewing the code.
 */

// #region Source of truth
#define MAX_BOARD_SIZE 10
#define MIN_BOARD_SIZE 4
#define MIN_MINES 4
#define RECOMENDED_MINE_DENSITY 0.2
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
#define TITLE                                                                                                                                                                      \
  SEPARATOR;                                                                                                                                                                       \
  SPACE;                                                                                                                                                                           \
  WRITE "             " + MAIN_COLOR + " MINESWEEPER" + RESET_FORMAT ENDL;                                                                                                         \
  WRITE "              " + ITALIC + "By: Adise" + RESET_FORMAT ENDL;                                                                                                               \
  SPACE;                                                                                                                                                                           \
  SEPARATOR;                                                                                                                                                                       \
  SPACE;
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
#define IS_IN_PRESENTATION 0
#define IS_IN_SETUP 0
#define IS_IN_GAME 2
#define IS_IN_END_SCREEN 3
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
  int state = IS_IN_PRESENTATION;
  bool didIncorrectInput = false;
  int boardSize;
  int nOfMines;
  int truthBoard[MAX_BOARD_SIZE * MAX_BOARD_SIZE];
  int board[MAX_BOARD_SIZE * MAX_BOARD_SIZE];
  string letters = "ABCDEFGHIJ";

  string persistentMessage = "";
  // #endregion

  // #region Runner
  while (true) {
    // #region Board cleanup
    for (size_t cellIndex = 0; cellIndex < MAX_BOARD_SIZE * MAX_BOARD_SIZE; cellIndex++) {
      truthBoard[cellIndex] = IS_EMPTY;
      board[cellIndex] = IS_DEFAULT;
    }
    // #endregion

    // #region Presentation
    /**
     * We could replace this bool-while with a do-while and check the output directly, but this is cleaner
     * more mantainable and more readable
     */
    // #region How to play

    while (state == IS_IN_PRESENTATION) {
      // #region Render
      CLEAR_TERMINAL;

      TITLE

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
        state = IS_IN_SETUP;
        break;
      }

      didIncorrectInput = true;
      persistentMessage += "Unknown response, please type \"y\" or \"n\"";
      // #endregion
    }
    // #endregion
    // #endregion

    // #region Main loop
    while (state == IS_IN_SETUP || state == IS_IN_GAME) {
      // #region Board setup
      bool isSizePicked = false;
      bool isNOfMinesPicked = false;
      int maxMines;

      while (state == IS_IN_SETUP) {
        // #region Render
        CLEAR_TERMINAL;

        TITLE

        WRITE MAIN_COLOR + "BOARD SETUP:" + RESET_FORMAT ENDL;
        if (!isSizePicked) WRITE "  Input a board size " + ITALIC + "(" + to_string(MIN_BOARD_SIZE) + "-" + to_string(MAX_BOARD_SIZE) + ")" + RESET_FORMAT ENDL;
        else WRITE "  Board size -> " + MAIN_COLOR + to_string(boardSize) + "x" + to_string(boardSize) + RESET_FORMAT ENDL;

        if (isSizePicked) {
          if (!isNOfMinesPicked) {
            WRITE "  Input a board size " + ITALIC + "(" + to_string(MIN_MINES) + "-" + to_string(maxMines) + ")" + RESET_FORMAT ENDL;
            WRITE ITALIC + "  Recommended mines: " + to_string(int(maxMines * RECOMENDED_MINE_DENSITY)) + RESET_FORMAT ENDL;
          } else {
            WRITE "  Number of MINES -> " + MAIN_COLOR + to_string(nOfMines) + RESET_FORMAT ENDL;
          }
        }
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
        if (isSizePicked && isNOfMinesPicked) WRITE "Are you ready? " + ITALIC + "(y/n)" + RESET_FORMAT ENDL;
        string result;


        if (!isSizePicked) {

          if (!SKIP_QUESTIONS) ASK result;
          else result = "10";

          try {
            boardSize = stoi(result);
          } catch (...) {
            didIncorrectInput = true;
            persistentMessage += "Invalid input, please type a number";
            continue;
          }

          if (boardSize < MIN_BOARD_SIZE || boardSize > MAX_BOARD_SIZE) {
            didIncorrectInput = true;
            persistentMessage += "Invalid size, please type a number between " + to_string(MIN_BOARD_SIZE) + " and " + to_string(MAX_BOARD_SIZE);
            continue;
          }
          /**
         * Max mines is calculated by taking the amounf of cells in the picked board and removing:
         * - (8) The number of possible neighbours of the initial cell, since we want the initial cell to be clear
         * - (1) The actual initial shown cell
         * - (1) To have space for a free space somewhere else on the map, since if not, the game would instantly end uppon opening the first cell
         */
          maxMines = boardSize * boardSize - NUM_NEIGHBOURS - 2;

          didIncorrectInput = false;
          isSizePicked = true;
          continue;
        }

        if (!isNOfMinesPicked) {

          if (!SKIP_QUESTIONS) ASK result;
          else result = "18";

          try {
            nOfMines = stoi(result);
          } catch (...) {
            didIncorrectInput = true;
            persistentMessage += "Invalid input, please type a number";
            continue;
          }
          if (nOfMines < MIN_MINES || nOfMines > maxMines) {
            didIncorrectInput = true;
            persistentMessage += "Invalid number of bombs, please type a number between " + to_string(MIN_BOARD_SIZE) + " and " + to_string(maxMines);
            continue;
          }
          didIncorrectInput = false;
          isNOfMinesPicked = true;
          continue;
        }


        if (!SKIP_QUESTIONS) ASK result;
        else result = "y";

        char resultChar;
        try {
          resultChar = tolower(result[0]);
        } catch (...) {
          didIncorrectInput = true;
          persistentMessage += "Invalid input, please type a number";
        }

        if (resultChar == 'n') {
          //Resets this loop and asks again
          isSizePicked = false;
          isNOfMinesPicked = false;
          didIncorrectInput = false;
          continue;
        }

        if (resultChar == 'y') {
          didIncorrectInput = false;
          break;
        }

        didIncorrectInput = true;
        persistentMessage += "Unknown response, please type \"y\" or \"n\"";
        // #endregion
      }

      // #region Board generation


      // #endregion


      // #endregion
    }
    // #endregion

    system("pause");
  }
  // #endregion
}
