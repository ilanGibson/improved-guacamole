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
  int optimal_paths[4][3];

  for (int i = 0; i < 4; i++) {
    if (ghost_process_direction(cmds[i], 1)) {
      optimal_paths[i][0] = tgc1.y;
      optimal_paths[i][1] = tgc1.x;
      optimal_paths[i][2] = 1;
    } else {
      optimal_paths[i][2] = 0;
    }
  }

  // for (int i = 0; i < 4; i++) {
  //   if (optimal_paths[i][2] == 1) {
  //     printf("option %d:\t", i);
  //     printf("y: %d, x: %d\n", optimal_paths[i][0], optimal_paths[i][1]);
  //   }
  // }

  printf("count: %d\t", move_count);
  ghost_process_direction(cmds[calc_manhattan(optimal_paths)], 0);
  // char t = calc_manhattan(optimal_paths);
  // printf("t: %d", t);
  // return;
  ++move_count;
  print_ghost_movements();
}

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

char calc_manhattan(int options[4][3]) {
  int min = -1;
  char move;
  for (int i = 0; i < 4; i++) {
    if (options[i][2] == 1) {
      int temp_dist = (abs(options[i][1] - pX) + abs(options[i][0] - pY));
      if (min == -1 || temp_dist < min) {
        min = temp_dist;
        move = i;
      }
    }
  }
  return move;
}
