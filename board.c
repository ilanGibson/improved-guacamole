#include <stdio.h>

#define HEIGHT 10
#define WIDTH 20

char PACMAN = 'P';
int pX = 2;
int pY = 2;

char GHOST = 'G';
int gY = 5;
int gX = 2;

char *get_cell(int row, int col);
void set_cell(int row, int col, char value);
char BOARD[HEIGHT][WIDTH + 1] = {
    "####################", "#..................#", "#.......#..........#",
    "#.......#..........#", "#.......#..........#", "#............#.....#",
    "#............#.....#", "#............#.....#", "#..................#",
    "####################"};

char *get_cell(int row, int col) { return &BOARD[row - 1][col - 1]; }
void set_cell(int row, int col, char value) { BOARD[row - 1][col - 1] = value; }
