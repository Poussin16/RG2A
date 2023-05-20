.PHONY: all clean

CC = gcc
CFLAGS = -Wall -Wextra -g `pkg-config --cflags sdl2`
LDFLAGS = -lm `pkg-config --libs sdl2`
SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)
EXEC = prog

all : $(EXEC)

$(EXEC) : $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o : %.c
	$(CC) $(CFLAGS) -o $@ -c $<

clean :
	rm -rf *.o
	rm -rf $(EXEC)
