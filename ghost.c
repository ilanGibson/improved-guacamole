void manhattan_chase(void);
int ghost_process_direction(char key);

static int previous_move = -1;

void manhattan_chase(void) {
  static int move_count = 0;
  char cmds[] = {'h', 'j', 'l', 'k'};
  char optimal_paths[4];

  for (int i = 0; i < 4; i++) {
  }

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
