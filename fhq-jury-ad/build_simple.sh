#!/bin/bash
# gcc -o daemon daemon.c

if [ ! -d tmp ]; then
	mkdir -p tmp
fi

cd tmp
cmake ..
make