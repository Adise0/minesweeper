#include <functional>
#include <iostream>
#include <regex>
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
#define MIN_MINES 1
#define RECOMENDED_MINE_DENSITY 0.2
#define SKIP_QUESTIONS false
#define IS_DEBUGGING_BOARD false



// #region ANSI CODES
#define ITALIC "\033[3m"
#define RESET_FORMAT "\033[0m"

// #region Colors
#define MAIN_COLOR "\033[38;5;27m"
#define LOST_COLOR "\033[38;5;160m"

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

// #region Console utils
#define CLEAR_TERMINAL system("cls")
#define SEPARATOR cout << "======================================" << endl
#define WRITE cout << string() +
#define ENDL << endl
#define SPACE cout << endl
#define ASK getline(cin, input)
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
#define DEFAULT_SPRITE "⬛ "
#define EMPTY_SPRITE "⬜ "
#define MARKED_SPRITE "🚩 "
#define MINE_SPRITE "💣 "
#define MINE_EXPLODED_SPRITE "💥 "
// #endregion



// #region Truth tables
// #region Cell types
#define IS_DEFAULT 0
#define IS_MARKED 1
#define IS_EMPTY 2
#define IS_MINE 3
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
  int currentCell;
  string letters = "ABCDEFGHIJ";
  string maxChar;
  regex inputRegX;
  string persistentMessage = "";
  // #endregion

  // #region Runner
  while (true) {
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

      char resultChar;
      try {
        string input = "y";
        if (!SKIP_QUESTIONS) ASK;
        resultChar = tolower(input[0]);
      } catch (...) {

        didIncorrectInput = true;
        persistentMessage += "Invalid input, please type a y or n";
        continue;
      }
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
      // #region Board cleanup
      for (size_t cellIndex = 0; cellIndex < MAX_BOARD_SIZE * MAX_BOARD_SIZE; cellIndex++) {
        truthBoard[cellIndex] = IS_EMPTY;
        board[cellIndex] = IS_DEFAULT;
      }
      // #endregion

      bool isSizePicked = false;
      bool isNOfMinesPicked = false;
      int maxMines = 1;
      int explodedMine = 0;
      char currentAction = '0';


      // #region Utils
      /**
     * LAMBDA EXPRESSIONS ARE STILL IN main()!
     */

      // Helper function to get neighbouring cells
      function<void(int index, int neighbours[NUM_NEIGHBOURS])> getNeighbours = [&boardSize](int index, int neighbours[NUM_NEIGHBOURS]) {
        // #region getNeighbours
        bool isLeft = index % boardSize == 0;
        bool isRight = index % boardSize == boardSize - 1;
        bool isTop = index / boardSize == 0;
        bool isBottom = int(index / boardSize) == boardSize - 1;

        neighbours[NEIGHBOUR_TOP_LEFT] = (isTop || isLeft) ? -1 : index - boardSize - 1;
        neighbours[NEIGHBOUR_TOP] = isTop ? -1 : index - boardSize;
        neighbours[NEIGHBOUR_TOP_RIGHT] = (isTop || isRight) ? -1 : index - boardSize + 1;
        neighbours[NEIGHBOUR_RIGHT] = isRight ? -1 : index + 1;
        neighbours[NEIGHBOUR_BOTTOM_RIGHT] = (isBottom || isRight) ? -1 : index + boardSize + 1;
        neighbours[NEIGHBOUR_BOTTOM] = isBottom ? -1 : index + boardSize;
        neighbours[NEIGHBOUR_BOTTOM_LEFT] = (isBottom || isLeft) ? -1 : index + boardSize - 1;
        neighbours[NEIGHBOUR_LEFT] = isLeft ? -1 : index - 1;
        // #endregion
      };

      // Helper function to render the board
      function<void()> renderBoard = [&persistentMessage, &boardSize, &board, &truthBoard, &letters, &state, &currentCell, &explodedMine]() {
        // #region renderBoard
        system("cls");

        for (size_t row = 0; row < (boardSize + 1); row++) {
          for (size_t col = 0; col < (boardSize + 1); col++) {
            if (col == 0 && row == 0) WRITE "  ";
            if (col == 0 && row != 0) WRITE(row < 10 ? " " : "") + to_string(row) + " ";
            if (row == 0 && col != 0) WRITE "  " + letters[col - 1];
            if (row == 0 || col == 0) continue;

            int buffer = -1;
            int cell = (row + buffer) * boardSize + (col + buffer);

            int cellType = ((state == IS_IN_GAME && !IS_DEBUGGING_BOARD) ? board : truthBoard)[cell];

            if (cellType > IS_NUMBER) {
              switch (cellType - IS_NUMBER) {
              case 1:
                WRITE NUM_1_COLOR;
                break;
              case 2:
                WRITE NUM_2_COLOR;
                break;
              case 3:
                WRITE NUM_3_COLOR;
                break;
              case 4:
                WRITE NUM_4_COLOR;
                break;
              case 5:
                WRITE NUM_5_COLOR;
                break;
              case 6:
                WRITE NUM_6_COLOR;
                break;
              case 7:
                WRITE NUM_7_COLOR;
                break;
              case 8:
                WRITE NUM_8_COLOR;
                break;
              }
              WRITE " " + to_string(cellType - IS_NUMBER) + " " + RESET_FORMAT;
              continue;
            }
            switch (cellType) {
            case IS_DEFAULT:
              WRITE DEFAULT_SPRITE;
              break;
            case IS_MARKED:
              WRITE MARKED_SPRITE;
              break;
            case IS_EMPTY:
              WRITE EMPTY_SPRITE;
              break;
            case IS_MINE:
              WRITE string() + ((explodedMine != 0 && currentCell == cell) ? MINE_EXPLODED_SPRITE : MINE_SPRITE);
              break;
            default:
              // This should never reach
              WRITE DEFAULT_SPRITE;
              break;
            }
          }
          SPACE;
        }
        WRITE persistentMessage ENDL;
        persistentMessage = "";
        // #endregion
      };

      //Helper function to get the user to pick a cell
      function<char()> getInput = [&inputRegX, &persistentMessage, &boardSize, &currentCell]() {
        // #region getInput
        string input;
        ASK;

        char action = '0';

        try {
          // // AI for regex matching syntax in c++. Modified ofc
          smatch match;
          if (regex_match(input, match, inputRegX)) {
            char colChar = toupper(match[1].str()[0]);
            int rowNum = stoi(match[2].str());
            if (match[3].matched) action = tolower(match[3].str()[0]);
            else action = 's';

            int col = colChar - 'A';
            int row = rowNum - 1;

            currentCell = row * boardSize + col;
          }
          // End of syntax copy
        } catch (...) {
          persistentMessage += string() +
                               "\nInvalid input. Format is:\n"
                               "- " +
                               MAIN_COLOR + "[Column][Row] [Action]" + RESET_FORMAT +
                               " To pick the cell and "
                               "action.\n"
                               "OR"
                               "- " +
                               MAIN_COLOR + "[Column][Row]" + RESET_FORMAT +
                               " To pick the cell and "
                               "show the cell\n";
        }
        return action;
        // #endregion
      };

      // Helper function to recursivelly show cells
      function<void(int neighbours[], int cellIndex)> showNeighbours = [&board, &truthBoard, &getNeighbours, &showNeighbours](int neighbours[], int cellIndex) {
        if (board[cellIndex] != IS_DEFAULT) return;

        board[cellIndex] = truthBoard[cellIndex];
        if (truthBoard[cellIndex] != IS_EMPTY) return;

        int nestedNeighbours[NUM_NEIGHBOURS];
        getNeighbours(cellIndex, nestedNeighbours);
        for (size_t neighbourIndex = 0; neighbourIndex < NUM_NEIGHBOURS; neighbourIndex++) {
          showNeighbours(nestedNeighbours, nestedNeighbours[neighbourIndex]);
        }
      };
      // #endregion


      while (state == IS_IN_SETUP) {
        // #region Render
        CLEAR_TERMINAL;

        TITLE

        WRITE MAIN_COLOR + "BOARD SETUP:" + RESET_FORMAT ENDL;
        if (!isSizePicked) WRITE "  Input a board size " + ITALIC + "(" + to_string(MIN_BOARD_SIZE) + "-" + to_string(MAX_BOARD_SIZE) + ")" + RESET_FORMAT ENDL;
        else WRITE "  Board size -> " + MAIN_COLOR + to_string(boardSize) + "x" + to_string(boardSize) + RESET_FORMAT ENDL;

        if (isSizePicked) {
          if (!isNOfMinesPicked) {
            WRITE "  Input the number of mines " + ITALIC + "(" + to_string(MIN_MINES) + "-" + to_string(maxMines) + ")" + RESET_FORMAT ENDL;
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
        string input;


        if (!isSizePicked) {

          if (!SKIP_QUESTIONS) ASK;
          else input = "10";

          try {
            boardSize = stoi(input);
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

          if (!SKIP_QUESTIONS) ASK;
          else input = "18";

          try {
            nOfMines = stoi(input);
          } catch (...) {
            didIncorrectInput = true;
            persistentMessage += "Invalid input, please type a number";
            continue;
          }
          if (nOfMines < MIN_MINES || nOfMines > maxMines) {
            didIncorrectInput = true;
            persistentMessage += "Invalid number of mines, please type a number between " + to_string(MIN_BOARD_SIZE) + " and " + to_string(maxMines);
            continue;
          }
          didIncorrectInput = false;
          isNOfMinesPicked = true;
          continue;
        }


        if (!SKIP_QUESTIONS) ASK;
        else input = "y";

        char resultChar;
        try {
          resultChar = tolower(input[0]);
        } catch (...) {
          didIncorrectInput = true;
          persistentMessage += "Invalid input, please type a y or n";
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
      state = IS_IN_GAME;

      maxChar = string() + letters[boardSize - 1];
      inputRegX =
          regex((boardSize >= 10 ? "^\\s*([A-" + maxChar + "])\\s*(10|[1-9])\\s*([ms])?\\s*$" : "^\\s*([A-" + maxChar + "])\\s*([1-" + to_string(boardSize) + "])\\s*([ms])?\\s*$"),
                regex_constants::icase);


      do {
        renderBoard();
        currentAction = getInput();

        if (currentAction != 's') {
          persistentMessage += "\n Please open an initial cell";
        }
      } while (currentAction != 's');
      // #region Board generation
      for (size_t i = 0; i < nOfMines; i++) {
        int mineIndex;
        bool isTooClose = false;
        do {
          isTooClose = false; // Expect a vlaid position

          int row = rand() % boardSize;
          int col = rand() % boardSize;

          mineIndex = row * boardSize + col; // Flattened 2D array syntax

          /**
           * These early simple checks "break" early.
           * They avoid the computation, even if small, from `getNeighbours` if the valid condition already fails.
           */

          isTooClose = mineIndex == currentCell;
          if (isTooClose) continue;

          isTooClose = truthBoard[mineIndex] == IS_MINE;
          if (isTooClose) continue;


          int mineNeighbours[NUM_NEIGHBOURS];
          getNeighbours(mineIndex, mineNeighbours);

          for (size_t neighbour = 0; neighbour < NUM_NEIGHBOURS; neighbour++) {
            int neighbourIndex = mineNeighbours[neighbour];
            if (neighbourIndex == -1) continue; // Ignore neighbours that are out of bounds
            if (neighbourIndex == currentCell) isTooClose = true;
          }
        } while (mineIndex == currentCell || truthBoard[mineIndex] == IS_MINE || isTooClose);

        truthBoard[mineIndex] = IS_MINE;
      }

      for (size_t cellIndex = 0; cellIndex < boardSize * boardSize; cellIndex++) {
        if (truthBoard[cellIndex] == IS_MINE) continue;

        int neighbours[NUM_NEIGHBOURS];
        getNeighbours(cellIndex, neighbours);

        int neighbouringBombs = 0;

        for (size_t neighbourIndex = 0; neighbourIndex < NUM_NEIGHBOURS; neighbourIndex++) {
          if (neighbours[neighbourIndex] == -1) continue; // Ingore neighbours outside bounds
          if (truthBoard[neighbours[neighbourIndex]] == IS_MINE) neighbouringBombs++;
        }
        if (neighbouringBombs == 0) truthBoard[cellIndex] = IS_EMPTY;
        else truthBoard[cellIndex] = IS_NUMBER + neighbouringBombs; // Attach number directly to the cell type
      }
      // #endregion

      int neighbours[NUM_NEIGHBOURS];
      getNeighbours(currentCell, neighbours);
      showNeighbours(neighbours, currentCell);

      // #endregion

      // #region In game
      bool hasWon = false;
      while (state == IS_IN_GAME) {
        currentAction = '0';
        renderBoard();
        currentAction = getInput();


        if (currentAction == 'm') {
          board[currentCell] = IS_MARKED;
        }
        if (currentAction != 's') continue;

        if (truthBoard[currentCell] == IS_MINE) {
          state = IS_IN_END_SCREEN;
          hasWon = false;
          explodedMine = currentCell;
          break;
        }

        if (board[currentCell] == IS_DEFAULT || board[currentCell] == IS_MARKED) {
          int neighbours[NUM_NEIGHBOURS];
          getNeighbours(currentCell, neighbours);
          showNeighbours(neighbours, currentCell);
        }

        for (size_t cellIndex = 0; cellIndex < boardSize * boardSize; cellIndex++) {
          if (truthBoard[cellIndex] == IS_MINE) continue;
          if (board[cellIndex] == IS_DEFAULT) break;
          if (board[cellIndex] == IS_MARKED) break;

          state = IS_IN_END_SCREEN;
          hasWon = true;
        }
      }

      renderBoard();
      SPACE;
      SEPARATOR;
      SPACE;
      WRITE "             " + (hasWon ? (string() + MAIN_COLOR + "YOU WON!") : (string() + LOST_COLOR + "GAME OVER!")) + RESET_FORMAT ENDL;
      if (!hasWon) WRITE "              " + ITALIC + "Get good" + RESET_FORMAT ENDL;
      SPACE;
      SEPARATOR;
      SPACE;

      while (state == IS_IN_END_SCREEN) {
        WRITE "Fancy another game? " + ITALIC + "(y/n)" + RESET_FORMAT ENDL;
        string input;
        ASK;
        char resultChar;
        try {
          resultChar = tolower(input[0]);
        } catch (...) {
          didIncorrectInput = true;
          persistentMessage += "Invalid input, please type a y or n";
        }

        if (resultChar == 'n') {
          WRITE "Goodbye then!" ENDL;
          system("pause");
          return 0;
        }

        if (resultChar == 'y') {
          state = IS_IN_SETUP;
          didIncorrectInput = false;
          break;
        }

        didIncorrectInput = true;
        persistentMessage += "Unknown response, please type \"y\" or \"n\"";
      }

      // #endregion
    }
    // #endregion
  }
  // #endregion
}
