CC = gcc
FLAGS = -pedantic -Wall -Wextra

ifndef DEBUG
	FLAGS += -O2
else
	FLAGS += -Og -g
endif

PYTHON_VERSION = 3.10

all: rpython reverse

rpython: rpython.c
	$(CC) $(FLAGS) rpython.c -lpython$(PYTHON_VERSION) -I/usr/include/python$(PYTHON_VERSION) -o $@

reverse: reverse.c
	$(CC) $(FLAGS) reverse.c -o $@

clean:
	rm -f rpython reverse
