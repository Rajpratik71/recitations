CC = gcc
CXX = g++ -O0

INCLUDES = -DBASIC3TRACE

CFLAGS   = -g -Wall $(INCLUDES)
CXXFLAGS = -g -Wall $(INCLUDES)

LDFLAGS = -g
LDLIBS  =

.PHONY: valgrind
valgrind: default
	valgrind --leak-check=yes ./arraylist-test

default: box-test readfile arraylist-test

box-test: box.o

arraylist-test: arraylist.h

readfile:

box.o: box.h

.PHONY: clean
clean:
	rm -rf *.o box-test *~ core a.out readfile arraylist-test

.PHONY: all
all: clean default
