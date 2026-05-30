#include "board.h"
#include "pac_ansi.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

const char PACMAN = 'P';
int pY;
int pX;
int old_pY;
int old_pX;
int score = 0;

const char GHOST = 'G';
int gY;
int gX;
int old_gY;
int old_gX;

void initialize_game(void) {
  pY = 3;
  pX = 2;
  old_pY = pY;
  old_pX = pX;

  score = 0;

  gY = 10;
  gX = 14;
  old_gY = gY;
  old_gX = gX;
}

int DIRTY = 0;
int DIRTY_PACMAN = 0;
char BOARD[BOARD_HEIGHT][BOARD_WIDTH + 1] = {
    "###########################", "#.........................#",
    "<.#######.#######.#######.>", "#...##....#######....##...#",
    "#.#....##.#######.##....#.#", "#.##.#.##.........##.#.##.#",
    "#.#..#.##.#######.##.#..#.#", "#...##.#...........#.##...#",
    "#.#.##.#.#.## ##.#.#.##.#.#", "#.#......#.|   |.#......#.#",
    "#.####.#.#.|___|.#.#.####.#", "#.#....#.#.......#.#....#.#",
    "#.#.####.#.##.##.#.####.#.#", "#...###....##.##....###...#",
    "#.#.....#.###.###.#.....#.#", "#.#####.#.###.###.#.#####.#",
    "#.#.....#.###.###.#.....#.#", "#.#.#####.###.###.#####.#.#",
    "#.........................#", "<.###.#######.#######.###.>",
    "#.........................#", "###########################"};

char *get_cell(int row, int col) { return &BOARD[row - 1][col - 1]; }
int get_board_quadrant(int row, int col) {
  /* q = 3 -> bottom right quad
   * q = 2 -> bottom left quad
   * q = 1 -> top right quad
   * q = 0 -> top left quad */

  int q = 3;
  if (row <= 10)
    q -= 2;
  if (col <= 13)
    q -= 1;

  return q;
}

char CURR_BOARD[BOARD_HEIGHT][BOARD_WIDTH + 1] = {
    "###########################", "#.........................#",
    "#.#######.#######.#######.#", "#...##....#######....##...#",
    "#.#....##.#######.##....#.#", "#.##.#.##.........##.#.##.#",
    "#.#..#.##.#######.##.#..#.#", "#...##.#...........#.##...#",
    "#.#.##.#.#.## ##.#.#.##.#.#", "#.#......#.|   |.#......#.#",
    "#.####.#.#.|___|.#.#.####.#", "#.#....#.#.......#.#....#.#",
    "#.#.####.#.##.##.#.####.#.#", "#...###....##.##....###...#",
    "#.#.....#.###.###.#.....#.#", "#.#####.#.###.###.#.#####.#",
    "#.#.....#.###.###.#.....#.#", "#.#.#####.###.###.#####.#.#",
    "#.........................#", "#.###.#######.#######.###.#",
    "#.........................#", "###########################"};

char *get_curr_cell(int row, int col) { return &CURR_BOARD[row - 1][col - 1]; }
void set_curr_cell(int row, int col, char value) {
  CURR_BOARD[row - 1][col - 1] = value;
  increase_score();
}
void increase_score(void) { score++; }

void print_board(void) {
  // see pac_ansi.h
  WRITE_ESC(ESC_HIDE_CURSOR);
  WRITE_ESC(ESC_CLEAR_SCREEN);
  WRITE_ESC(ESC_MOVE_CURSOR_HOME);
  int i, j;
  for (i = 1; i < BOARD_HEIGHT + 1; i++) {
    for (j = 1; j < BOARD_WIDTH + 1; j++) {
      if (i == gY && j == gX) {
        write(STDOUT_FILENO, &GHOST, 1);
        continue;
      }
      if (i == pY && j == pX) {
        write(STDOUT_FILENO, &PACMAN, 1);
        continue;
      }
      write(STDOUT_FILENO, get_cell(i, j), 1);
    }
    putchar('\n');
  }
  // save cursor pos for testing
  WRITE_ESC(ESC_SAVE_CURSOR_POS);
}

void draw(void) {
  char buf[32];
  int len;

  /* go to old p pos
   * print get_curr_cell(old pos)
   * go to new p pos
   * print 'P' */
  len = snprintf(buf, sizeof(buf), "\x1b[%d;%dH", old_pY, old_pX);
  write(STDOUT_FILENO, buf, len);
  write(STDOUT_FILENO, get_curr_cell(old_pY, old_pX), 1);

  len = snprintf(buf, sizeof(buf), "\x1b[%d;%dH", pY, pX);
  write(STDOUT_FILENO, buf, len);
  write(STDOUT_FILENO, &PACMAN, 1);

  /* if (old ghost pos != curr player pos)
   * go to old g pos
   * print get_curr_cell(old pos) */
  // otherwise get bug where player cannot
  // be directly 'behind' ghost

  /* go to new g pos
   * print 'G' */
  if (old_gY != pY || old_gX != pX) {
    len = snprintf(buf, sizeof(buf), "\x1b[%d;%dH", old_gY, old_gX);
    write(STDOUT_FILENO, buf, len);
    write(STDOUT_FILENO, get_curr_cell(old_gY, old_gX), 1);
  }

  len = snprintf(buf, sizeof(buf), "\x1b[%d;%dH", gY, gX);
  write(STDOUT_FILENO, buf, len);
  write(STDOUT_FILENO, &GHOST, 1);

  /* display score */
  WRITE_ESC(ESC_LOAD_CURSOR_POS);
  WRITE_ESC(ESC_CLEAR_LINE);
  len = snprintf(buf, sizeof(buf), "score: %d\n", score);
  write(STDOUT_FILENO, buf, len);
}

int check_collision(void) {
  if ((pY == gY) && (pX == gX)) {
    return 1;
  }
  return 0;
}

void die(const char *s) {
  WRITE_ESC(ESC_CLEAR_SCREEN);
  WRITE_ESC(ESC_MOVE_CURSOR_HOME);
  perror(s); // display GET/SET TERMINAL SETTINGS error
  exit(1);
}
