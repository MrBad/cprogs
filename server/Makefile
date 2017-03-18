INCLUDE=.
LIBS=
CC=gcc
OFLAGS=-c
DFLAGS=EST_TREE
CFLAGS=-g -Wall -Wextra -std=gnu99 -pedantic-errors -I$(INCLUDE) -D$(DFLAGS)

TARGET=server
OBJECTS=server.o

all: $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS) $(LIBS)

%.o: %.c Makefile *.h
	$(CC) $(CFLAGS) $(OFLAGS) -o $@ $<

clean:
	rm $(OBJECTS) $(TARGET)
distclean:
	make clean
