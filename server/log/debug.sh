#!/bin/sh

g++ -o out main.cc log.cc ../utils/utils.cc -std=c++11 -g
gdb ./out