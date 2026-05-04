#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h> /* for sleep() */

#include <errno.h>
#include <termios.h>
#include <unistd.h>

struct termios orig_termios;

void die(const char *s);
void disable_raw_mode(void);
void enable_raw_mode(void);

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

int main() {
  srand(time(NULL));
  enable_raw_mode();

  print_board();
  while (1) {
    // process_keypress();
    random_movement();
    usleep(050000);
    if (check_collision()) {
      endgame();
      return 0;
    }
  }
  return 0;
}
