#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#include <errno.h>
#include <termios.h>
#include <unistd.h>

#include "board.c"

#define HEIGHT 10
#define WIDTH 20

struct termios orig_termios;
enum direction_key {
  ARROW_LEFT = 'h',
  ARROW_RIGHT = 'l',
  ARROW_UP = 'k',
  ARROW_DOWN = 'j'
};

void die(const char *s);
void disable_raw_mode(void);
void enable_raw_mode(void);
char read_key(void);
void process_keypress(void);
void print_board(void);

void print_movements(void);
void pacman_process_direction(char key);

int main() {
  enable_raw_mode();

  print_board();
  while (1) {
    process_keypress();
  }
  return 0;
}

void print_board(void) {
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
  write(STDOUT_FILENO, "\x1b[s", 3);
  char buf[32];
  int len = snprintf(buf, sizeof(buf), "\x1b[%d;%dH", pY, pX);
  write(STDOUT_FILENO, buf, len);
}

void print_movements(void) {
  char buf[32];
  write(STDOUT_FILENO, get_cell(pY, pX), 1);
  int len = snprintf(buf, sizeof(buf), "\x1b[%d;%dH", pY, pX);
  write(STDOUT_FILENO, buf, len);
  write(STDOUT_FILENO, &PACMAN, 1);
  write(STDOUT_FILENO, buf, len);

  /* for testing; prints coordinates */
  write(STDOUT_FILENO, "\x1b[u", 3); /* put cursor after board */
  write(STDOUT_FILENO, "\x1b[2K", 4);

  len = snprintf(buf, sizeof(buf), "p: %d;%d\n", pY, pX);
  write(STDOUT_FILENO, buf, len);

  /* puts cursor back in proper position */
  len = snprintf(buf, sizeof(buf), "\x1b[%d;%dH", pY, pX);
  write(STDOUT_FILENO, buf, len);
}

void pacman_process_direction(char key) {
  switch (key) {
  case ARROW_UP:
    if (*get_cell((pY - 1), pX) == '.') {
      pY--;
    }
    break;
  case ARROW_DOWN:
    if (*get_cell((pY + 1), pX) == '.') {
      pY++;
    }
    break;
  case ARROW_RIGHT:
    if (*get_cell(pY, (pX + 1)) == '.') {
      pX++;
    }
    break;
  case ARROW_LEFT:
    if (*get_cell(pY, (pX - 1)) == '.') {
      pX--;
    }
    break;
  }
  print_movements();
}

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

char read_key(void) {
  int nread;
  char c;
  while ((nread = read(STDIN_FILENO, &c, 1)) != 1) {
    if (nread == -1 && errno != EAGAIN)
      die("read");
  }
  if (c == '\x1b') {
    char seq[3];

    if (read(STDOUT_FILENO, &seq[0], 1) != 1)
      return '\x1b';
    if (read(STDOUT_FILENO, &seq[1], 1) != 1)
      return '\x1b';

    if (seq[0] == '[') {
      switch (seq[1]) {
      case 'A':
        return ARROW_UP;
      case 'B':
        return ARROW_DOWN;
      case 'C':
        return ARROW_RIGHT;
      case 'D':
        return ARROW_LEFT;
      }
    }
    return '\x1b';
  } else {
    return c;
  }
}

void process_keypress(void) {
  char c = read_key();

  switch (c) {
  case ('q'):
    write(STDOUT_FILENO, "\x1b[2J", 4);
    write(STDOUT_FILENO, "\x1b[H", 3);

    exit(0);
    break;

  case ARROW_UP:
  case ARROW_DOWN:
  case ARROW_RIGHT:
  case ARROW_LEFT:
    pacman_process_direction(c);
    break;
  }
}
