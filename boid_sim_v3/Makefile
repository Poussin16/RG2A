# Makefile

CC = gcc
CPPFLAGS = -MMD -g
CFLAGS = -Wall -Wextra -O3 -O0 -g `pkg-config --cflags sdl2 SDL2_image SDL2_ttf`
LDFLAGS =
LDLIBS = -lm `pkg-config --libs sdl2 SDL2_image SDL2_ttf`

SRC = main.c my_data.c
OBJ = ${SRC:.c=.o}
OBJ2 = ${SRC:.c=.d}
EXE = ${SRC:.c=}

all: main

main: ${OBJ}

.PHONY: clean

clean:
	${RM} ${OBJ}
	${RM} ${OBJ2}
	${RM} ${EXE}


# END
