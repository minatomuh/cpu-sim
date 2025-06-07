#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include <string>
#include <fstream>
#include <iostream>

// Can be changed, sets the architecture
using CPU_WordSize = uint32_t;
using CPU_WordSizeSigned = int32_t;
// used for proper signed value printing and comparison
// must match CPU_WordSize

enum class InstructionSet {
    SET,
    CPY,
    CPYI,
    CPYI2,
    ADD,
    ADDI,
    SUBI,
    JIF,
    PUSH,
    POP,
    CALL,
    RET,
    HLT,
    USER,
    SYSCALL, // PRN A / HLT / YIELD
    NOP
};

struct Instruction {
    InstructionSet type;
    CPU_WordSize param1;
    CPU_WordSize param2;
    
    Instruction() : type(InstructionSet::NOP), param1(0), param2(0) {}
    Instruction(const std::string& instructionStr) {
        parse(instructionStr);
    }
    
    Instruction parse(const std::string& instructionStr);

    friend std::ostream& operator<<(std::ostream& os, const Instruction& instr);
};

#endif