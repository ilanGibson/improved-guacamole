#include "pacman.h"
#include "board.h"
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

char read_key(void) {
  int nread;
  char c;

  // ISSUE #16
  // while ((nread = read(STDIN_FILENO, &c, 1)) != 1) {
  //   if (nread == -1 && errno != EAGAIN)
  //     die("read");
  // }

  nread = read(STDIN_FILENO, &c, 1);
  if (nread == -1 && errno != EAGAIN)
    die("read");

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

    write(STDOUT_FILENO, "\x1b[?25h", 6);
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

void pacman_process_direction(char key) {
  old_pY = pY;
  old_pX = pX;
  char c;
  switch (key) {
  case ARROW_UP:
    if ((c = *get_cell((pY - 1), pX)) == '.' || c == ' ') {
      pY--;
    }
    break;
  case ARROW_DOWN:
    if ((c = *get_cell((pY + 1), pX)) == '.' || c == ' ') {
      pY++;
    }
    break;
  case ARROW_RIGHT:
    if ((c = *get_cell(pY, (pX + 1))) == '.' || c == ' ') {
      pX++;
    }
    break;
  case ARROW_LEFT:
    if ((c = *get_cell(pY, (pX - 1))) == '.' || c == ' ') {
      pX--;
    }
    break;
  }
  DIRTY = 1;
}
