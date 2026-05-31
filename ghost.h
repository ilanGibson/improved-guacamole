#ifndef GHOST_H
#define GHOST_H
#define MAX_Q 1000
#include "board.h"

typedef struct {
  int x;
  int y;
  int fn;
} POS;

POS addPosition(POS *curr, POS dir);
char isEqual(POS child, POS target);
void initializeParent(char visited[][BOARD_WIDTH]);

typedef struct {
  int front;
  int rear;
  POS data[MAX_Q];
} Queue;

typedef struct {
  POS data[MAX_Q];
  int size;
} PQ;

void initializeQ(Queue *q);
char isEmptyQ(Queue *q);
char isFullQ(Queue *q);
void enqueue(Queue *q, POS pos);
void dequeue(Queue *q);
POS peek(Queue *q);

void BFS_with_path(POS start, POS target, POS *path, int *pathLen);
void process_BFS(POS *path, int *pathLen);

void swap(POS *a, POS *b);
void headpUP(PQ *pq, int index);
void enqueuePQ(PQ *pq, POS pos);
void heapDOWN(PQ *pq, int index);
POS deququePQ(PQ *pq);
POS peekPQ(PQ *pq);
void initializePQ(PQ *pq);
void a_star_with_path(POS start, POS target, POS *path, int *pathLen);
void process_a_star(POS *path, int *pathLen);
int calc_manhattan(POS curr, POS target);

#endif
