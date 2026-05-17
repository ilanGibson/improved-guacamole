#include "endgame.h"
#include <stdio.h>
#include <unistd.h>

void endgame(void) {
  write(STDOUT_FILENO, "\x1b[2J", 4);
  write(STDOUT_FILENO, "\x1b[H", 3);
  write(STDOUT_FILENO, "\x1b[?25h", 6);
  printf("bye");
}
