#!/bin/sh

cd ./build
rm -rf C* c*
cmake ..
make
