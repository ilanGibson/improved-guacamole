#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h> /* for sleep() */

#include <errno.h>
#include <termios.h>
#include <unistd.h>

struct termios orig_termios;
int DIRTY = 0;
int GHOST_INTERVAL = 200;

void die(const char *s);
void disable_raw_mode(void);
void enable_raw_mode(void);
time_t get_time(void);

#include "board.c"
#include "endgame.c"
#include "ghost.c"
#include "pacman.c"

void die(const char *s) {
  write(STDOUT_FILENO, "\x1b[2J", 4);

  write(STDOUT_FILENO, "\x1b[H", 3);
  perror(s);
  exit(1);
}

void disable_raw_mode() {
  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1)
    die("tcsetattr");
}

void enable_raw_mode() {
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

int main() {
  srand(time(NULL));
  enable_raw_mode();

  int64_t last_ghost_move = get_time_ms();
  print_board();
  while (1) {
    process_keypress();

    int64_t now = get_time_ms();
    if (now - last_ghost_move > GHOST_INTERVAL) {
      manhattan_chase();
      last_ghost_move = get_time_ms();
    }

    if (check_collision()) {
      endgame();
      return 0;
    }

    if (DIRTY)
      draw();
    DIRTY = 0;

    // dont know is sleep() is needed
    // continue testing
    // usleep(200000);
  }
  return 0;
}
