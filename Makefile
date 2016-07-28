#
#
#

CC = g++
CFLAGS = -g

all: judge

judge: judge.cpp state_machine.cpp simulation.h state_machine.h
	$(CC) judge.cpp $(CFLAGS) -o judge state_machine.cpp

clean:
	rm -f *.o judge *~
