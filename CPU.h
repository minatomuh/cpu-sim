#ifndef CPU_H
#define CPU_H

#include <cstdint>
#include <array>
#include "Instruction.h"

class CPU {
    public:
        CPU(CPU_WordSize* data_memory, Instruction* instr_memory);
        ~CPU();
        void step();
        void reset();
        bool isHalted() const;
        void exec(Instruction instr);

    private:
        bool mode; // 0 for kernel mode, 1 for user mode
        bool halt;

        // memory mapped CPU registers
        CPU_WordSize& PC; 
        CPU_WordSize& SP;
        CPU_WordSize& syscallResult;
        CPU_WordSize& instrCount;
        std::array<std::reference_wrapper<CPU_WordSize>, 17> R; // 17 general purpose registers

        CPU_WordSize* data_memory; // direct access to memory 
        Instruction* instruction_memory;

        void execSET(CPU_WordSize value, CPU_WordSize address);
        void execCPY(CPU_WordSize src, CPU_WordSize dest);
        void execCPYI(CPU_WordSize src, CPU_WordSize dest);
        void execCPYI2(CPU_WordSize src, CPU_WordSize dest);
        void execADD(CPU_WordSize address, CPU_WordSize value);
        void execADDI(CPU_WordSize address1, CPU_WordSize address2);
        void execSUBI(CPU_WordSize address1, CPU_WordSize address2);
        void execJIF(CPU_WordSize address, CPU_WordSize target);
        void execPUSH(CPU_WordSize address);
        void execPOP(CPU_WordSize address);
        void execCALL(CPU_WordSize address);
        void execRET();
        void execHLT();
        void execUSER(CPU_WordSize address);
        void execSYSCALL(CPU_WordSize type, CPU_WordSize addr);
        void execNOP();
};

#endif