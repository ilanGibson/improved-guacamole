#include "ghost.h"
#include "board.h"
#include <stdio.h>

POS direction[4] = {
    {0, -1}, // up
    {-1, 0}, // left
    {0, 1},  // down
    {1, 0}   // right
};

POS addPositions(POS *curr, POS dir) {
  POS p;
  p.x = curr->x + dir.x;
  p.y = curr->y + dir.y;

  return p;
}

char isEqual(POS child, POS target) {
  return ((child.x == target.x) && (child.y == target.y));
}

void initializeQ(Queue *q) {
  q->front = -1;
  q->rear = 0;
}

char isEmptyQ(Queue *q) { return (q->front == q->rear - 1); }
char isFullQ(Queue *q) { return (q->rear == MAX_Q); }

void enqueue(Queue *q, POS pos) {
  if (isFullQ(q)) {
    printf("enqueue is full\n");
    return;
  }

  q->data[q->rear] = pos;
  q->rear++;
}

void dequeue(Queue *q) {
  if (isEmptyQ(q)) {
    printf("queue is empty\n");
    return;
  }

  q->front++;
}

POS peek(Queue *q) {
  if (isEmptyQ(q)) {
    printf("queue is empty\n");
    return (POS){-1, -1};
  }

  return (q->data[q->front + 1]);
}

void printQ(Queue *q) {
  if (isEmptyQ(q)) {
    printf("queue is empty\n");
    return;
  }

  printf("current queue:\n");
  for (int i = q->front + 1; i < q->rear; i++) {
    printf("queue[%d]: x:%d y:%d\n", i, q->data[i].x, q->data[i].y);
  }
  printf("end\n");
}

void BFS_with_path(POS start, POS target, POS *path, int *pathLen) {
  POS start2 = addPositions(&start, (POS){-1, -1});
  POS target2 = addPositions(&target, (POS){-1, -1});
  Queue q;
  initializeQ(&q);
  // the traversal across graph is implicit
  char visited[BOARD_HEIGHT][BOARD_WIDTH];

  // initialize parent
  POS parent[BOARD_HEIGHT][BOARD_WIDTH];
  for (int i = 0; i < BOARD_HEIGHT; i++) {
    for (int j = 0; j < BOARD_WIDTH; j++) {
      char *tempCell = get_cell(i + 1, j + 1);
      if (*tempCell == '#' || *tempCell == '|' || *tempCell == '-' ||
          *tempCell == '_') {
        visited[i][j] = 1;
        continue;
      }
      visited[i][j] = 0;
    }
  }

  visited[start2.y][start2.x] = 1;
  parent[start2.y][start2.x] = (POS){-1, -1};
  enqueue(&q, start2);

  while (!isEmptyQ(&q)) {
    POS curr = peek(&q);
    dequeue(&q);

    if (visited[target2.y][target2.x]) {
      POS temp = target2;
      *(path + *pathLen) = (POS){temp.x, temp.y};
      (*pathLen)++;
      while (!isEqual(parent[temp.y][temp.x], (POS){-1, -1})) {
        POS temp2 = parent[temp.y][temp.x];
        temp = temp2;
        *(path + *pathLen) = temp;
        (*pathLen)++;
      }
      return;
    }

    for (int dir = 0; dir < 4; dir++) {
      POS temp = addPositions(&curr, direction[dir]);
      // ISSUE #23
      // if (temp.y < 0 || temp.x < 0)
      //   continue;
      if (!visited[temp.y][temp.x]) {
        visited[temp.y][temp.x] = 1;
        parent[temp.y][temp.x] = curr;
        enqueue(&q, temp);
      }
    }
  }
}

void process_BFS(POS *path, int *pathLen) {
  if (*pathLen > 0) {
    (*pathLen)--;
    old_gY = gY;
    old_gX = gX;

    gY = path[*pathLen].y + 1;
    gX = path[*pathLen].x + 1;
  }
}
