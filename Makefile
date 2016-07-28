#
#
#

CC = g++
CFLAGS = -g

all: judge judge_html

judge: judge.cpp state_machine.cpp simulation.h state_machine.h
	$(CC) judge.cpp $(CFLAGS) -o judge state_machine.cpp

judge-html: judge_html.cpp state_machine.cpp simulation.h state_machine.h
	$(CC) judge_html.cpp $(CFLAGS) -o judge_html state_machine.cpp

clean:
	rm -f *.o judge judge-html
