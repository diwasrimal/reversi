#!/bin/sh

target=${1:-reversi}
cc -Wall -Wextra -std=c11 -o $target "$target.c"
