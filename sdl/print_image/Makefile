#DFLAGS=
INCLUDE=.
LIBS=-lSDL2
CC=gcc
OFLAGS=-c
CFLAGS=-g -Wall -Wextra -std=c99 -pedantic-errors -I$(INCLUDE)

TARGET=print_image
OBJECTS=print_image.o

all: $(OBJECTS) Makefile
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS) $(LIBS)

%o: %.c Makefile *.h
	$(CC) $(CFLAGS) $(OFLAGS) -o $@ $<
clean:
	rm $(OBJECTS) $(TARGET)
