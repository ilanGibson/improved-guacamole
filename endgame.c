#include "endgame.h"
#include "pac_ansi.h"
#include <stdio.h>
#include <unistd.h>

void endgame(void) {
  WRITE_ESC(ESC_CLEAR_SCREEN);
  WRITE_ESC(ESC_MOVE_CURSOR_HOME);
  WRITE_ESC(ESC_SHOW_CURSOR);
  printf("bye");
}
