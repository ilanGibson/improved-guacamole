#include <unistd.h>
#define HEIGHT 10
#define WIDTH 20

enum direction_key {
  ARROW_LEFT = 'h',
  ARROW_RIGHT = 'l',
  ARROW_UP = 'k',
  ARROW_DOWN = 'j'
};

char PACMAN = 'P';
int pY = 2;
int pX = 2;
int old_pY;
int old_pX;

char GHOST = 'G';
int gY = 9;
int gX = 19;
int old_gY;
int old_gX;

char *get_cell(int row, int col);
void set_cell(int row, int col, char value);
void print_board(void);
void draw(void);
void print_player_movements(void);
void print_ghost_movements(void);
int check_collision(void);

char BOARD[HEIGHT][WIDTH + 1] = {
    "####################", "#..................#", "#.......#..........#",
    "#.......#..........#", "#.......#..........#", "#............#.....#",
    "#............#.....#", "#............#.....#", "#..................#",
    "####################"};

char *get_cell(int row, int col) { return &BOARD[row - 1][col - 1]; }
void set_cell(int row, int col, char value) { BOARD[row - 1][col - 1] = value; }

void print_board(void) {
  write(STDOUT_FILENO, "\x1b[?25l", 6);

  write(STDOUT_FILENO, "\x1b[2J", 4);
  write(STDOUT_FILENO, "\x1b[H", 3);
  int i, j;
  for (i = 1; i < HEIGHT + 1; i++) {
    for (j = 1; j < WIDTH + 1; j++) {
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
  /* save cursor pos */
  write(STDOUT_FILENO, "\x1b[s", 3);
}

void draw(void) {
  char buf[32];
  int len;

  /* go to old p pos
   * print '.'
   * go to new p pos
   * print 'P' */
  len = snprintf(buf, sizeof(buf), "\x1b[%d;%dH", old_pY, old_pX);
  write(STDOUT_FILENO, buf, len);
  write(STDOUT_FILENO, ".", 1);
  len = snprintf(buf, sizeof(buf), "\x1b[%d;%dH", pY, pX);
  write(STDOUT_FILENO, buf, len);
  write(STDOUT_FILENO, &PACMAN, 1);

  /* go to old g pos
   * print '.'
   * go to new g pos
   * print 'G' */
  len = snprintf(buf, sizeof(buf), "\x1b[%d;%dH", old_gY, old_gX);
  write(STDOUT_FILENO, buf, len);
  write(STDOUT_FILENO, ".", 1);
  len = snprintf(buf, sizeof(buf), "\x1b[%d;%dH", gY, gX);
  write(STDOUT_FILENO, buf, len);
  write(STDOUT_FILENO, &GHOST, 1);

  // len = snprintf(buf, sizeof(buf), "\x1b[%d;%dH", gY, gX);
  // write(STDOUT_FILENO, buf, len);

  //   /* for testing; prints coordinates */
  //   write(STDOUT_FILENO, "\x1b[u", 3); /* put cursor after board */
  //   write(STDOUT_FILENO, "\x1b[2K", 4);
  //   len = snprintf(buf, sizeof(buf), "g: %d;%d\n", gY, gX);
  //   write(STDOUT_FILENO, buf, len);
  //   len = snprintf(buf, sizeof(buf), "old g: %d;%d\n", old_gY, old_gX);
  //   write(STDOUT_FILENO, "\x1b[2K", 4);
  //   write(STDOUT_FILENO, buf, len);
}

int check_collision(void) {
  if ((pY == gY) && (pX == gX)) {
    return 1;
  }
  return 0;
}
