INCLUDE=.
LIBS=
CC=gcc
OFLAGS=-c
DFLAGS=TEST_TREE
CFLAGS=-g -Wall -Wextra -std=gnu99 -pedantic-errors -I../ -I$(INCLUDE) -D$(DFLAGS)

OBJECTS=tree.o

all: $(OBJECTS) Makefile
	$(CC) $(CFLAGS) -o tree $(OBJECTS) $(LIBS)


%.o: %.c Makefile *.h
	$(CC) $(CFLAGS) $(OFLAGS) -o $@ $<

clean:
	rm $(OBJECTS) $(TARGET) tree
