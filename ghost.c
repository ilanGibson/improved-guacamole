void manhattan_chase(void);
int ghost_process_direction(char key, char check_flag);
char calc_manhattan(int options[4][3]);

struct temp_ghost_cord {
  int y;
  int x;
};

struct temp_ghost_cord tgc1 = {0, 0};

static int previous_move = -1;

void manhattan_chase(void) {
  static int move_count = 0;
  char cmds[] = {'h', 'j', 'l', 'k'};
  /* array of int arrays
   * [i][0] == y
   * [i][1] == x
   * [i][2] == valid move flag */
  int optimal_paths[4][3];

  for (int i = 0; i < 4; i++) {
    /* for each turn check check which move are valid for ghost
     * if valid, set would be x and y
     * set valid move flag */
    if (ghost_process_direction(cmds[i], 1)) {
      optimal_paths[i][0] = tgc1.y;
      optimal_paths[i][1] = tgc1.x;
      optimal_paths[i][2] = 1;
    } else {
      optimal_paths[i][2] = 0;
    }
  }

  /* for testing */
  // for (int i = 0; i < 4; i++) {
  //   if (optimal_paths[i][2] == 1) {
  //     printf("option %d:\t", i);
  //     printf("y: %d, x: %d\n", optimal_paths[i][0], optimal_paths[i][1]);
  //   }
  // }

  /* process optimal move calculated from calc_manhattan */
  ghost_process_direction(cmds[calc_manhattan(optimal_paths)], 0);
  ++move_count;
  DIRTY = 1;
  printf("count: %d\t", move_count);
}

/* if check_flag
 * strictly checking which moves are valid
 * if NOT check_flag
 * actually moving ghost */
int ghost_process_direction(char key, char check_flag) {
  int valid_move = 0;

  old_gY = gY;
  old_gX = gX;

  tgc1.y = gY;
  tgc1.x = gX;
  switch (key) {
  case ARROW_UP:
    if (*get_cell((gY - 1), gX) == '.') {
      valid_move = 1;
      if (check_flag) {
        tgc1.y--;
        break;
      }
      gY--;
    }
    break;
  case ARROW_DOWN:
    if (*get_cell((gY + 1), gX) == '.') {
      valid_move = 1;
      if (check_flag) {
        tgc1.y++;
        break;
      }
      gY++;
    }
    break;
  case ARROW_RIGHT:
    if (*get_cell(gY, (gX + 1)) == '.') {
      valid_move = 1;
      if (check_flag) {
        tgc1.x++;
        break;
      }
      gX++;
    }
    break;
  case ARROW_LEFT:
    if (*get_cell(gY, (gX - 1)) == '.') {
      valid_move = 1;
      if (check_flag) {
        tgc1.x--;
        break;
      }
      gX--;
    }
    break;
  }
  return valid_move;
}

/* calculate optimal move where diagonal is not option */
char calc_manhattan(int options[4][3]) {
  int min = -1;
  char move;
  for (int i = 0; i < 4; i++) {
    /* if valid move flag set */
    if (options[i][2] == 1) {
      int temp_dist = (abs(options[i][1] - pX) + abs(options[i][0] - pY));
      /* if first check or temp_dist is new optimal */
      if (min == -1 || temp_dist < min) {
        min = temp_dist;
        move = i;
      }
    }
  }
  return move;
}
