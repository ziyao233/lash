#!/bin/sh

CC=cc

"$CC" -o lash.so -shared -fPIC $(pkg-config --cflags lua5.4) lash.c \
	-Wall -Werror -g -O0 -pedantic -Wextra -std=gnu99
