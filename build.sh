#!/bin/bash

clang -O0 -g -fsanitize=address,undefined -Wall -Wextra -Wpedantic main.c board.c ghost.c pacman.c endgame.c
# find . -type f \( -name "*.c" -o -name "*.h" \) -print0 | xargs -0 wc -l
