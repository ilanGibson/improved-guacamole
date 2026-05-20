#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>

#include "board.h"
#include "endgame.h"
#include "ghost.h"
#include "pacman.h"

struct termios orig_termios;
void disable_raw_mode(void);
void enable_raw_mode(void);
time_t get_time(void);

void disable_raw_mode(void) {
  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1)
    die("tcsetattr");
}

void enable_raw_mode(void) {
  if (tcgetattr(STDIN_FILENO, &orig_termios) == -1)
    die("tcgetattr");
  atexit(disable_raw_mode);

  struct termios raw = orig_termios;
  raw.c_lflag &= ~(ECHO | ICANON | ISIG); // turn off echo + canonical mode
  raw.c_cc[VMIN] = 0;
  raw.c_cc[VTIME] = 1;

  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1)
    die("tcgetattr");
}

int64_t get_time_ms(void) {
  struct timespec ts;

  clock_gettime(CLOCK_REALTIME, &ts);

  /* tv_sec -> seconds
   * tv_nsec -> nanoseconds
   * multiple seconds by 1000
   * divide nanoseconds by 1,000,000
   * add together to get ms */
  return ((int64_t)ts.tv_sec * 1000) + (ts.tv_nsec / 1000000);
}

int main(void) {
  srand(time(NULL));
  enable_raw_mode();

  int64_t last_ghost_move = get_time_ms();
  print_board();

  POS start = {gX, gY};
  POS target = {pX, pY};
  int pathLen = 0;
  POS path[100];
  BFS_with_path(start, target, path, &pathLen);

  while (1) {
    process_keypress();

    int64_t now = get_time_ms();
    if (now - last_ghost_move > GHOST_INTERVAL) {
      DIRTY = 1;
      process_BFS(path, &pathLen);
      last_ghost_move = get_time_ms();
    }

    if (check_collision()) {
      endgame();
      return 0;
    }

    if (DIRTY)
      draw();
    DIRTY = 0;
  }
  return 0;
}
