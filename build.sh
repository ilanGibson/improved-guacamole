#!/bin/bash

clang -O0 -g -fsanitize=address,undefined -Wall -Wextra -Wpedantic main.c board.c ghost.c pacman.c endgame.c
