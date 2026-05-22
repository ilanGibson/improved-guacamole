#ifndef BOARD_H
#define BOARD_H

#define GHOST_INTERVAL 100
#define BFS_INTERVAL 200
enum { BOARD_HEIGHT = 22, BOARD_WIDTH = 28 };

enum direction_key {
  ARROW_LEFT = 'h',
  ARROW_RIGHT = 'l',
  ARROW_UP = 'k',
  ARROW_DOWN = 'j'
};

extern const char PACMAN;
extern int pY;
extern int pX;
extern int old_pY;
extern int old_pX;

extern const char GHOST;
extern int gY;
extern int gX;
extern int old_gY;
extern int old_gX;

extern int DIRTY;
extern int DIRTY_PACMAN;

char *get_cell(int row, int col);
int get_board_quadrant(int row, int col);
char *get_curr_cell(int row, int col);
void set_curr_cell(int row, int col, char value);
void increase_score(void);
void print_board(void);
void draw(void);
int check_collision(void);
void die(const char *);

#endif
