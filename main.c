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
  // attempts to set terminal settings
  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1)
    // if attempt fails specify SET TERMINAL SETTINGS error
    die("tcsetattr");
}

void enable_raw_mode(void) {
  // attempts to get terminal settings
  if (tcgetattr(STDIN_FILENO, &orig_termios) == -1)
    // if attempt fails specify GET TERMINAL SETTINGS error
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

void run_program() {
  initialize_game();
  srand(time(NULL));
  enable_raw_mode();

  int64_t last_ghost_move = get_time_ms();
  int64_t last_BFS_calc = get_time_ms();
  print_board();
  // initialize pacman eatting first '.'
  set_curr_cell(pY, pX, ' ');

  POS start = {gX, gY, 0};
  POS target = {pX, pY, 0};
  int pathLen = 0;
  POS path[100];
  a_star_with_path(start, target, path, &pathLen);
  // return 0;

  // BFS_with_path(start, target, path, &pathLen);

  // player starting quadrant
  // int p_quad = get_board_quadrant(pY, pX);
  while (1) {
    process_keypress();
    int64_t now = get_time_ms();
    if (now - last_ghost_move > GHOST_INTERVAL) {

      // int temp_p_quad = get_board_quadrant(pY, pX);
      // // if pacman in new quadrant
      // // new BFS_with_path
      // if (temp_p_quad != p_quad && (now - last_BFS_calc > BFS_INTERVAL)) {
      //   p_quad = temp_p_quad;
      //   pathLen = 0;
      //   BFS_with_path((POS){gX, gY}, (POS){pX, pY}, path, &pathLen);
      // } else {
      //   int temp_g_quad = get_board_quadrant(gY, gX);
      //   // otherwise
      //   // if ghost quadrant == pacman_quadrant && pacman has moved
      //   // new BFS_with_path
      //   if (temp_g_quad == temp_p_quad && DIRTY_PACMAN) {
      //     DIRTY_PACMAN = 0;
      //     pathLen = 0;
      //     BFS_with_path((POS){gX, gY}, (POS){pX, pY}, path, &pathLen);
      //   }
      // }

      DIRTY = 1;
      // process_BFS(path, &pathLen);
      process_a_star(path, &pathLen);
      last_ghost_move = get_time_ms();
    }

    if (check_collision()) {
      return;
    }
    if (DIRTY)
      draw();
    DIRTY = 0;
  }
  return;
}

int main(void) {

  while (1) {
    run_program();
    disable_raw_mode();
    if (!endgame()) {
      break;
    }
  }
  return 0;
}
