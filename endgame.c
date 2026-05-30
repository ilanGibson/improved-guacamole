#include "endgame.h"
#include "pac_ansi.h"
#include <stdio.h>
#include <unistd.h>

int endgame(void) {
  WRITE_ESC(ESC_CLEAR_SCREEN);
  WRITE_ESC(ESC_MOVE_CURSOR_HOME);
  WRITE_ESC(ESC_SHOW_CURSOR);

  printf("play again?... Y/n ");
  char play = getchar();
  getchar();

  if (play == 'Y')
    return 1;
  WRITE_ESC(ESC_CLEAR_SCREEN);
  WRITE_ESC(ESC_MOVE_CURSOR_HOME);
  return 0;
}

void endgame_quit(void) {
  WRITE_ESC(ESC_CLEAR_SCREEN);
  WRITE_ESC(ESC_MOVE_CURSOR_HOME);
  WRITE_ESC(ESC_SHOW_CURSOR);

  printf("c --v is %ld\n", __STDC_VERSION__);
}
