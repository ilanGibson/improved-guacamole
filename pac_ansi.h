#ifndef ANSI_H
#define ANSI_H

#define WRITE_ESC(seq) write(STDOUT_FILENO, seq, sizeof(seq) - 1)

#define ESC_HIDE_CURSOR "\x1b[?25l"
#define ESC_SHOW_CURSOR "\x1b[?25h"

#define ESC_CLEAR_SCREEN "\x1b[2J"
#define ESC_MOVE_CURSOR_HOME "\x1b[H"
#define ESC_SAVE_CURSOR_POS "\x1b[s"

#endif
