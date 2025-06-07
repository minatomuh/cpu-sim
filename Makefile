CC = g++
CFLAGS = -Wall -Wextra -pedantic -std=c++17
EXEC = simulator

all: $(EXEC)

$(EXEC): main.cpp CPU.cpp CPU.h Instruction.cpp Instruction.h Simulator.cpp Simulator.h
	$(CC) $(CFLAGS) -o $(EXEC) main.cpp CPU.cpp Instruction.cpp Simulator.cpp

clean:
	rm -f $(EXEC)