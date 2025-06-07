#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <cstdint>
#include <array>
#include "CPU.h"
#include "Instruction.h"

constexpr size_t MEMORY_SIZE = 20000;

class Simulator {
    public:
        Simulator();
        ~Simulator();
        void run(size_t clockspeed); // in Hz
        void reset();
        void step();
        void loadProgram(const char* filename);
        void printMemoryHex();
        void printMemoryHexSkipZero();
        void printMemoryDec();
        void setDebugMode(uint8_t mode) { debugMode = mode; }
        void printThreadTable();
        void _printThreadTable();

    private:
        friend class CPU;
        CPU cpu;
        std::array<CPU_WordSize, MEMORY_SIZE> data_memory;
        std::array<Instruction, MEMORY_SIZE> instruction_memory;
        uint8_t debugMode;
};

#endif