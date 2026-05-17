#ifndef GHOST_H
#define GHOST_H
#define MAX_Q 1000

typedef struct {
  int x;
  int y;
} POS;

POS addPosition(POS *curr, POS dir);
char isEqual(POS child, POS target);

typedef struct {
  int front;
  int rear;
  POS data[MAX_Q];
} Queue;

void initializeQ(Queue *q);
char isEmptyQ(Queue *q);
char isFullQ(Queue *q);
void enqueue(Queue *q, POS pos);
void dequeue(Queue *q);
POS peek(Queue *q);

void BFS_with_path(POS start, POS target, POS *path, int *pathLen);
void process_BFS(POS *path, int *pathLen);

#endif
