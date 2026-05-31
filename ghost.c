#include "ghost.h"
#include "board.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

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

void initializeParent(char closed[][BOARD_WIDTH]) {
  for (int i = 0; i < BOARD_HEIGHT; i++) {
    for (int j = 0; j < BOARD_WIDTH; j++) {
      char *tempCell = get_cell(i + 1, j + 1);
      if (*tempCell == '#' || *tempCell == '|' || *tempCell == '-' ||
          *tempCell == '_') {
        closed[i][j] = 1;
        continue;
      }
      closed[i][j] = 0;
    }
  }
}

void initializeGn(double(g_n[BOARD_HEIGHT][BOARD_WIDTH])) {
  for (int i = 0; i < BOARD_HEIGHT; i++) {
    for (int j = 0; j < BOARD_WIDTH; j++) {
      g_n[i][j] = INFINITY;
    }
  }
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
  POS parent[BOARD_HEIGHT][BOARD_WIDTH];
  initializeParent(visited);

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
      // avoid stack-buffer overflow of visited
      if (temp.y < 0 || temp.x < 0 || temp.y > 21 || temp.x > 27) {
        continue;
      }
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

void swap(POS *a, POS *b) {
  POS temp = *a;
  *a = *b;
  *b = temp;
}

void heapUP(PQ *pq, int index) {
  if (index && pq->data[(index - 1) / 2].fn > pq->data[index].fn) {
    swap(&pq->data[(index - 1) / 2], &pq->data[index]);
    heapUP(pq, (index - 1) / 2);
  }
}

void enqueuePQ(PQ *pq, POS pos) {
  if (pq->size == MAX_Q) {
    printf("priority queue is full\n");
    return;
  }

  pq->data[pq->size++] = pos;
  heapUP(pq, pq->size - 1);
}

void heapDOWN(PQ *pq, int index) {
  int smallest = index;
  int left = 2 * index + 1;
  int right = 2 * index + 2;

  if (left < pq->size && pq->data[left].fn < pq->data[smallest].fn)
    smallest = left;

  if (right < pq->size && pq->data[right].fn < pq->data[smallest].fn) {
    smallest = right;
  }

  if (smallest != index) {
    swap(&pq->data[index], &pq->data[smallest]);
    heapDOWN(pq, smallest);
  }
}

POS deququePQ(PQ *pq) {
  if (!pq->size) {
    printf("priority queue is empty\n");
    return (POS){-1, -1};
  }

  POS item = pq->data[0];
  pq->data[0] = pq->data[--pq->size];
  heapDOWN(pq, 0);
  return item;
}

POS peekPQ(PQ *pq) {
  if (!pq->size) {
    printf("priority is emtpy\n");
    return (POS){-1, -1};
  }
  return pq->data[0];
}

void initializePQ(PQ *pq) { pq->size = 0; }

void print_point(POS pos) {
  printf("point x: %d, point y: %d, fn: %d\n", pos.x, pos.y, pos.fn);
}

void a_star_with_path(POS start, POS target, POS *path, int *pathLen) {
  // initialize priority queue
  POS start2 = addPositions(&start, (POS){-1, -1});
  POS target2 = addPositions(&target, (POS){-1, -1});
  PQ pq;
  initializePQ(&pq);

  // closed means node has beed removed from PQ and fully processed
  char closed[BOARD_HEIGHT][BOARD_WIDTH];
  POS parent[BOARD_HEIGHT][BOARD_WIDTH];
  initializeParent(closed);
  double g_n[BOARD_HEIGHT][BOARD_WIDTH];
  initializeGn(g_n);

  parent[start2.y][start2.x] = (POS){-1, -1};
  g_n[start2.y][start2.x] = 0;
  start2.fn = 0;
  enqueuePQ(&pq, start2);

  while (pq.size) {
    POS curr = deququePQ(&pq);

    if (closed[curr.y][curr.x]) {
      continue;
    }
    closed[curr.y][curr.x] = 1;

    if (isEqual(curr, target2)) {
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
      char temp_cell = *get_cell(temp.y + 1, temp.x + 1);
      if (temp_cell != ' ' && temp_cell != '.') {
        continue;
      }
      // ISSUE #31
      // if (temp.y < 0 || temp.x < 0 || temp.y > (BOARD_HEIGHT - 1) ||
      //     temp.x > (BOARD_WIDTH - 1)) {
      //   continue;
      // }
      int tenative_g = g_n[curr.y][curr.x] + 1;
      if (!(tenative_g < g_n[temp.y][temp.x])) {
        continue;
      }
      // update g(n)
      g_n[temp.y][temp.x] = tenative_g;
      int temp_h = calc_manhattan(temp, target2);
      temp.fn = tenative_g + temp_h;
      parent[temp.y][temp.x] = curr;
      enqueuePQ(&pq, temp);
    }
  }
}

void process_a_star(POS *path, int *pathLen) {
  if (*pathLen > 0) {
    (*pathLen)--;
    old_gY = gY;
    old_gX = gX;

    gY = path[*pathLen].y + 1;
    gX = path[*pathLen].x + 1;
  }
}

int calc_manhattan(POS curr, POS target) {
  int temp_dist = (abs(curr.x - target.x) + abs(curr.y - target.y));
  return temp_dist;
}
