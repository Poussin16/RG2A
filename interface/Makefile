# Makefile

CC = gcc
CPPFLAGS = -MMD -g
CFLAGS = -Wall -Wextra -O3 -std=c99 -O2 -O0 -g `pkg-config --cflags sdl2 SDL2_image SDL2_ttf gtk+-3.0`
LDFLAGS =
LDLIBS = -lm `pkg-config --libs sdl2 SDL2_image SDL2_ttf gtk+-3.0`

SRC = set_up.c ../Boid/main.c ../Boid/boid.c ../paint/paint.c ../algo/Algo_alpha/A_star.c ../algo/Algo_alpha/queue.c
OBJ = ${SRC:.c=.o}
OBJ2 = ${SRC:.c=.d}
EXE = ${SRC:.c=}

all: set_up

set_up: ${OBJ}

.PHONY: clean

clean:
	${RM} ${OBJ}
	${RM} ${OBJ2}
	${RM} ${EXE}


# END
