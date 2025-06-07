#include <iostream>
#include <vector>
#include <chrono>
#include "CPU.h"

CPU::CPU(CPU_WordSize* data_memory, Instruction* instr_memory)
    :   mode(0),
        halt(false),
        PC(data_memory[0]),
        SP(data_memory[1]),
        syscallResult(data_memory[2]),
        instrCount(data_memory[3]),
        R{ {    std::ref(data_memory[4]), std::ref(data_memory[5]), std::ref(data_memory[6]),
                std::ref(data_memory[7]), std::ref(data_memory[8]), std::ref(data_memory[9]),
                std::ref(data_memory[10]), std::ref(data_memory[11]), std::ref(data_memory[12]),
                std::ref(data_memory[13]), std::ref(data_memory[14]), std::ref(data_memory[15]),
                std::ref(data_memory[16]), std::ref(data_memory[17]), std::ref(data_memory[18]),
                std::ref(data_memory[19]), std::ref(data_memory[20]) } } 
{
    // Connect to memory
    this->data_memory = data_memory;
    this->instruction_memory = instr_memory;
}

CPU::~CPU() {
    // No need to cleanup
}

void CPU::step() {
    // No need to check for halt, simulator does it

    //DEBUG: Elapse runtime of each instruction
    //Instruction temp = instruction_memory[PC];
    //std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

    exec(instruction_memory[PC++]);

    //std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    //std::cout << "Elapsed time for instruction \"" << temp << "\": " << std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count() << " ns\n";

    ++instrCount;
}

// incomplete but never used
void CPU::reset() {
    mode = 0;
    halt = false;
}

bool CPU::isHalted() const {
    return halt;
}

void CPU::exec(Instruction instr) {
    switch(instr.type) {
        case InstructionSet::SET:
            if(mode && (instr.param2 < 1000 && instr.param2 > 20)) {
                std::cerr << "ERROR: execSET() address out of bounds" << std::endl;
                std::cerr << "Instruction: " << instr << std::endl;
                execSYSCALL(2, 0); // HLT the thread
                return;
            }
            execSET(instr.param1, instr.param2);
            break;
        case InstructionSet::CPY:
            if(mode && ((instr.param1 < 1000 && instr.param1 > 20) || (instr.param2 < 1000 && instr.param2 > 20))) {
                std::cerr << "ERROR: execCPY() address out of bounds" << std::endl;
                std::cerr << "Instruction: " << instr << std::endl;
                execSYSCALL(2, 0); // HLT the thread
                return;
            }
            execCPY(instr.param1, instr.param2);
            break;
        case InstructionSet::CPYI:
            if(mode 
                && ((instr.param1 < 1000 && instr.param1 > 20) || (instr.param2 < 1000 && instr.param2 > 20) // Addresses out of bounds
                || (data_memory[instr.param1] < 1000 && data_memory[instr.param1] > 20))) {                     // Indirect address out of bounds
                std::cerr << "ERROR: execCPYI() address out of bounds" << std::endl;
                std::cerr << "Instruction: " << instr << std::endl;
                execSYSCALL(2, 0); // HLT the thread
                return;
            }
            execCPYI(instr.param1, instr.param2);
            break;
        case InstructionSet::CPYI2:
            if(mode 
                && ((instr.param1 < 1000 && instr.param1 > 20) || (instr.param2 < 1000 && instr.param2 > 20) // Addresses out of bounds
                || (data_memory[instr.param1] < 1000 && data_memory[instr.param1] > 20) // Indirect address out of bounds
                || (data_memory[instr.param2] < 1000 && data_memory[instr.param2] > 20))) { // Indirect address out of bounds
                std::cerr << "ERROR: execCPYI2() address out of bounds" << std::endl;
                std::cerr << "Instruction: " << instr << std::endl;
                execSYSCALL(2, 0); // HLT the thread
                return;
            }
            execCPYI2(instr.param1, instr.param2);
            break;
        case InstructionSet::ADD:
            if(mode && (instr.param1 < 1000 && instr.param1 > 20)) {
                std::cerr << "ERROR: execADD() address out of bounds" << std::endl;
                std::cerr << "Instruction: " << instr << std::endl;
                execSYSCALL(2, 0); // HLT the thread
                return;
            }
            execADD(instr.param1, instr.param2);
            break;
        case InstructionSet::ADDI:
            if(mode && ((instr.param1 < 1000 && instr.param1 > 20) || (instr.param2 < 1000 && instr.param2 > 20))) {
                std::cerr << "ERROR: execADDI() address out of bounds" << std::endl;
                std::cerr << "Instruction: " << instr << std::endl;
                execSYSCALL(2, 0); // HLT the thread
                return;
            }
            execADDI(instr.param1, instr.param2);
            break;
        case InstructionSet::SUBI:
            if(mode && ((instr.param1 < 1000 && instr.param1 > 20) || (instr.param2 < 1000 && instr.param2 > 20))) {
                std::cerr << "ERROR: execSUBI() address out of bounds" << std::endl;
                std::cerr << "Instruction: " << instr << std::endl;
                execSYSCALL(2, 0); // HLT the thread
                return;
            }
            execSUBI(instr.param1, instr.param2);
            break;
        case InstructionSet::JIF:
            if(mode && (instr.param1 < 1000 && instr.param1 > 20)) {
                std::cerr << "ERROR: execJIF() address out of bounds\n";
                std::cerr << "Instruction: " << instr << std::endl;
                execSYSCALL(2, 0); // HLT the thread
                return;
            }
            execJIF(instr.param1, instr.param2);
            break;
        case InstructionSet::PUSH:
            if(mode && (instr.param1 < 1000 && instr.param1 > 20)) {
                std::cerr << "ERROR: execCPY() address out of bounds" << std::endl;
                std::cerr << "Instruction: " << instr << std::endl;
                execSYSCALL(2, 0); // HLT the thread
                return;
            }
            execPUSH(instr.param1);
            break;
        case InstructionSet::POP:
            if(mode && (instr.param1 < 1000 && instr.param1 > 20)) {
                std::cerr << "ERROR: execPOP() address out of bounds" << std::endl;
                std::cerr << "Instruction: " << instr << std::endl;
                execSYSCALL(2, 0); // HLT the thread
                return;
            }
            execPOP(instr.param1);
            break;
        case InstructionSet::CALL:
            execCALL(instr.param1);
            break;
        case InstructionSet::RET:
            execRET();
            break;
        case InstructionSet::HLT:
            execHLT();
            break;
        case InstructionSet::USER:
            execUSER(instr.param1);
            break;
        case InstructionSet::SYSCALL:
            execSYSCALL(instr.param1, instr.param2);
            break;
        case InstructionSet::NOP:
            execNOP();
            break;
        default:
            execNOP();
            std::cerr << "ERROR: exec() default case" << std::endl;
    }
}

void CPU::execSET(CPU_WordSize value, CPU_WordSize address) {
    data_memory[address] = value;
}

void CPU::execCPY(CPU_WordSize src, CPU_WordSize dest) {
    data_memory[dest] = data_memory[src];
}

void CPU::execCPYI(CPU_WordSize src, CPU_WordSize dest) {
    data_memory[dest] = data_memory[data_memory[src]];
}

void CPU::execCPYI2(CPU_WordSize src, CPU_WordSize dest) {
    data_memory[data_memory[dest]] = data_memory[data_memory[src]];
}

void CPU::execADD(CPU_WordSize address, CPU_WordSize value) {
    data_memory[address] += value;
}

void CPU::execADDI(CPU_WordSize address1, CPU_WordSize address2) {
    data_memory[address1] += data_memory[address2];
}

void CPU::execSUBI(CPU_WordSize address1, CPU_WordSize address2) {
    data_memory[address2] = data_memory[address1] - data_memory[address2];
}

void CPU::execJIF(CPU_WordSize address, CPU_WordSize target) {
    if(static_cast<CPU_WordSizeSigned>(data_memory[address]) <= 0) {
        PC = target;
    }
}

void CPU::execPUSH(CPU_WordSize address) {
    data_memory[SP--] = data_memory[address];
}

void CPU::execPOP(CPU_WordSize address) {
    data_memory[address] = data_memory[++SP];
}

void CPU::execCALL(CPU_WordSize address) {
    // Push return address
    data_memory[SP--] = PC;
    PC = address;
}

void CPU::execRET() {
    PC = data_memory[++SP];
}

void CPU::execHLT() {
    halt = true;
}

void CPU::execUSER(CPU_WordSize address) {
    mode = 1;
    // Set PC to the user mode address
    PC = address;
}

void CPU::execSYSCALL(CPU_WordSize type, CPU_WordSize addr) {
    int prev_mode = mode;
    // Switch to kernel mode
    mode = 0;

    switch(type) {
        case 1: // PRN
            std::cout << static_cast<CPU_WordSizeSigned>(data_memory[addr]) << std::endl;

            // Blocking only if thread was in user mode
            // Not implemented yet
            if(0 && prev_mode) { // Always false for now
                execCALL(0xA0000000); // OS Subroutine to block the thread for 100 instructions
            }
            mode = prev_mode; // Switch back to previous mode   
            break;
        case 2: // HLT
            // Shuts down the thread. Calls the OS subroutine
            if(prev_mode) {
                std::cout << "Thread halted by syscall." << std::endl;
                execCALL(200); // OS Subroutine to halt the thread
            }
            break;
        case 3: // YIELD
            // Yields the CPU so OS can schedule other threads. Calls the OS subroutine
            if(prev_mode) {
                std::cout << "Thread yielded by syscall." << std::endl;
                execCALL(300); // OS Subroutine to yield the thread
            }
                
            break;
        default:
            execNOP();
            std::cerr << "ERROR: execSYSCALL() default case" << std::endl;
    }

    // This is wrong, OS assembly handles the mode switch
    // Switch back to previous mode
    // mode = prev_mode;
}

void CPU::execNOP() {
    // No operation
}