INCLUDE=.
LIBS=
CC=gcc
OFLAGS=-c
DFLAGS=TEST_LIST
CFLAGS=-g -Wall -Wextra -std=gnu99 -pedantic-errors -I../ -I$(INCLUDE) -D$(DFLAGS)

OBJECTS=list.o test_list.o

all: $(OBJECTS) Makefile
	$(CC) $(CFLAGS) -o test_list $(OBJECTS) $(LIBS)


%.o: %.c Makefile *.h
	$(CC) $(CFLAGS) $(OFLAGS) -o $@ $<

clean:
	rm $(OBJECTS) $(TARGET) test_list
