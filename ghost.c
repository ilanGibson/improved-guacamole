void random_movement(void);
int ghost_process_direction(char key);

void random_movement(void) {
  static int i = 1;
  static int move_count = 0;
  char cmds[] = {'h', 'j', 'l', 'k'};
  int valid_move;

  if (move_count % 3 == 0) {
    int num = (rand() % 4) + 1;
    i += num;
  }

  valid_move = ghost_process_direction(cmds[i % 4]);
  printf("count: %d\t", move_count);
  while (!valid_move) {
    int num = (rand() % 4) + 1;
    i += num;
    valid_move = ghost_process_direction(cmds[i % 4]);
  }

  ++move_count;
  print_ghost_movements();
}

int ghost_process_direction(char key) {
  int valid_move = 1;
  old_gY = gY;
  old_gX = gX;
  switch (key) {
  case ARROW_UP:
    if (*get_cell((gY - 1), gX) == '.')
      gY--;
    else
      valid_move = 0;
    break;
  case ARROW_DOWN:
    if (*get_cell((gY + 1), gX) == '.')
      gY++;
    else
      valid_move = 0;
    break;
  case ARROW_RIGHT:
    if (*get_cell(gY, (gX + 1)) == '.')
      gX++;
    else
      valid_move = 0;
    break;
  case ARROW_LEFT:
    if (*get_cell(gY, (gX - 1)) == '.')
      gX--;
    else
      valid_move = 0;
    break;
  }
  return valid_move;
}
