#include "../ghost.h"
#include <assert.h>
#include <stdio.h>

void BFS_test() {
  POS start = {26, 21};
  POS target = {2, 2};
  POS BFS_path[100];
  int BFS_pathLen = 0;

  POS path[] = {{1, 1},   {2, 1},   {3, 1},   {4, 1},   {5, 1},   {6, 1},
                {7, 1},   {8, 1},   {9, 1},   {10, 1},  {11, 1},  {12, 1},
                {13, 1},  {14, 1},  {15, 1},  {16, 1},  {17, 1},  {18, 1},
                {19, 1},  {20, 1},  {21, 1},  {22, 1},  {23, 1},  {24, 1},
                {25, 1},  {25, 2},  {25, 3},  {25, 4},  {25, 5},  {25, 6},
                {25, 7},  {25, 8},  {25, 9},  {25, 10}, {25, 11}, {25, 12},
                {25, 13}, {25, 14}, {25, 15}, {25, 16}, {25, 17}, {25, 18},
                {25, 19}, {25, 20}};

  BFS_with_path(start, target, BFS_path, &BFS_pathLen);

  for (int i = 0; i < BFS_pathLen; i++) {
    assert(path[i].x == BFS_path[i].x);
    assert(path[i].y == BFS_path[i].y);
  }

  POS start2 = {14, 10};
  BFS_pathLen = 0;
  POS target2 = {2, 2};

  POS path2[] = {{1, 1}, {2, 1},  {3, 1},  {4, 1},  {5, 1},  {6, 1},  {7, 1},
                 {8, 1}, {9, 1},  {9, 2},  {9, 3},  {9, 4},  {9, 5},  {9, 6},
                 {9, 7}, {10, 7}, {11, 7}, {12, 7}, {13, 7}, {13, 8}, {13, 9}};

  BFS_with_path(start2, target2, BFS_path, &BFS_pathLen);

  for (int i = 0; i < BFS_pathLen; i++) {
    assert(path2[i].x == BFS_path[i].x);
    assert(path2[i].y == BFS_path[i].y);
  }

  puts("BFS_test passed (2/2)\n");
}

int main() { BFS_test(); }
