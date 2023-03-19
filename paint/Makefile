# Makefile

CC = gcc
CPPFLAGS = -MMD -g
CFLAGS = -Wall -Wextra -O3 `pkg-config --cflags sdl2`
LDFLAGS =
LDLIBS = -lm `pkg-config --libs sdl2`

SRC = main.c paint.c
OBJ = ${SRC:.c=.o}
EXE = ${SRC:.c=}

all: main

main: ${OBJ}

.PHONY: clean

clean:
	${RM} ${OBJ}
	${RM} ${EXE}
	find . -mindepth 1 -iname "*.d" -type f -delete

# END
