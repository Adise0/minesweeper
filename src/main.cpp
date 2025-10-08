#include <cstdlib>
#include <functional>
#include <iostream>
#include <regex>
#include <string>

using namespace std;

/**
 * Welcome to minesweeper.
 * I invite you to check out the README.md file before reviewing the code.
 */

#pragma region Truth tables

#pragma region Cell types
#define IS_DEFAULT 0
#define IS_MARKED 1
#define IS_EMPTY 2
#define IS_BOMB 3
#define IS_NUMBER 4
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

}
