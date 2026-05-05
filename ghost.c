void random_movement(void);
int ghost_process_direction(char key);

static int previous_move = -1;

void random_movement(void) {
  static int i = 1;
  static int move_count = 0;
  char cmds[] = {'h', 'j', 'l', 'k'};
  int failed_moves = 0;
  int valid_move;

  if (move_count % 3 == 0) {
    do {
      int num = (rand() % 4) + 1;
      i += num;
    } while ((i % 4) == ((previous_move + 2) % 4) && failed_moves++ < 4);
  }

  valid_move = ghost_process_direction(cmds[i % 4]);
  while (!valid_move) {
    failed_moves++;
    do {
      int num = (rand() % 4) + 1;
      i += num;
    } while ((i % 4) == ((previous_move + 2) % 4) && failed_moves < 4);
    valid_move = ghost_process_direction(cmds[i % 4]);
  }

  /* for testing */
  // if ((i % 4) == ((previous_move + 2) % 4))
  //   printf("i: %d prev: %d fails: %d\t", i, previous_move, failed_moves);
  // printf("count: %d\t", move_count);

  previous_move = i % 4;
  ++move_count;
  print_ghost_movements();
}

int ghost_process_direction(char key) {
  int valid_move = 0;

  old_gY = gY;
  old_gX = gX;
  switch (key) {
  case ARROW_UP:
    if (*get_cell((gY - 1), gX) == '.') {
      gY--;
      valid_move = 1;
    }
    break;
  case ARROW_DOWN:
    if (*get_cell((gY + 1), gX) == '.') {
      gY++;
      valid_move = 1;
    }
    break;
  case ARROW_RIGHT:
    if (*get_cell(gY, (gX + 1)) == '.') {
      gX++;
      valid_move = 1;
    }
    break;
  case ARROW_LEFT:
    if (*get_cell(gY, (gX - 1)) == '.') {
      gX--;
      valid_move = 1;
    }
    break;
  }
  return valid_move;
}
