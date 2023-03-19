# Makefile

CC = gcc
CPPFLAGS = -MMD -g
CFLAGS = -Wall -Wextra -O3 -O0 -g `pkg-config --cflags sdl2 SDL2_image SDL2_ttf gtk+-3.0`
LDFLAGS =
LDLIBS = -lm `pkg-config --libs sdl2 SDL2_image SDL2_ttf gtk+-3.0`

SRC = set_up.c ../boid_sim/main.c ../boid_sim/mycomplex.c ../boid_sim/my_sdl.c ../paint/paint.c ../shared_queue/queue.c ../shared_queue/shared_queue.c
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
