CFLAGS = -O
CC = g++
QMAKE_CXXFLAGS += -Wno-c++11-extensions

sat: main.o Solver.o Clause.o Node.o
	$(CC) $(CFLAGS) -o sat main.o Solver.o Clause.o Node.o

main.o: main.cpp Solver.h Clause.h Node.h
	$(CC) $(CFLAGS) -c main.cpp

Solver.o: Solver.cpp Solver.h Node.h
	$(CC) $(CFLAGS) -c Solver.cpp

Clause.o: Clause.cpp Clause.h Node.h
	$(CC) $(CFLAGS) -c Clause.cpp

Node.o: Node.cpp Node.h
	$(CC) $(CFLAGS) -c Node.cpp

clean:
	rm -f *.o
