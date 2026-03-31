#!/bin/bash
gcc -o out $1.c `sdl2-config --cflags --libs`
