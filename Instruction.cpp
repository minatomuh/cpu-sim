#include <sstream>
#include <iostream>
#include <algorithm>
#include <cstdint>
#include "Instruction.h"

// Parse an instruction string into an Instruction object
Instruction Instruction::parse(const std::string& instructionStr) {
    std::istringstream iss(instructionStr);
    std::string opcode;
    
    // Skip empty lines or comment lines
    if(instructionStr.empty() || instructionStr[0] == '#') {
        this->type = InstructionSet::NOP;
        return *this;
    }
    
    // Extract operation code
    iss >> opcode;
    
    // Convert opcode to uppercase for case-insensitive comparison
    std::transform(opcode.begin(), opcode.end(), opcode.begin(), 
        [](unsigned char c) { return std::toupper(c); });
    
    // Parse instruction based on opcode
    // PS: this code gives wrong results if CPU_WordSize is uint8_t, since iss reads param only 1 character, not whole integer
    if (opcode == "SET") {
        this->type = InstructionSet::SET;
        iss >> this->param1 >> this->param2;
    } 
    else if (opcode == "CPY") {
        this->type = InstructionSet::CPY;
        iss >> this->param1 >> this->param2;
    } 
    else if (opcode == "CPYI") {
        this->type = InstructionSet::CPYI;
        iss >> this->param1 >> this->param2;
    }
    else if (opcode == "CPYI2") {
        this->type = InstructionSet::CPYI2;
        iss >> this->param1 >> this->param2;
    }
    else if (opcode == "ADD") {
        this->type = InstructionSet::ADD;
        iss >> this->param1 >> this->param2;
    } 
    else if (opcode == "ADDI") {
        this->type = InstructionSet::ADDI;
        iss >> this->param1 >> this->param2;
    } 
    else if (opcode == "SUBI") {
        this->type = InstructionSet::SUBI;
        iss >> this->param1 >> this->param2;
    } 
    else if (opcode == "JIF") {
        this->type = InstructionSet::JIF;
        iss >> this->param1 >> this->param2;
    } 
    else if (opcode == "PUSH") {
        this->type = InstructionSet::PUSH;
        iss >> this->param1;
    } 
    else if (opcode == "POP") {
        this->type = InstructionSet::POP;
        iss >> this->param1;
    } 
    else if (opcode == "CALL") {
        this->type = InstructionSet::CALL;
        iss >> this->param1;
    } 
    else if (opcode == "RET") {
        this->type = InstructionSet::RET;
    } 
    else if (opcode == "HLT") {
        this->type = InstructionSet::HLT;
    } 
    else if (opcode == "USER") {
        this->type = InstructionSet::USER;
        iss >> this->param1;
    } 
    else if (opcode == "SYSCALL") {
        this->type = InstructionSet::SYSCALL;
        
        // Parse system call type
        std::string syscallType;
        iss >> syscallType;
        
        // Convert syscall type to uppercase for case-insensitive comparison
        std::transform(syscallType.begin(), syscallType.end(), syscallType.begin(), 
            [](unsigned char c) { return std::toupper(c); });
        
        if (syscallType == "PRN") {
            this->param1 = 1;
            iss >> this->param2; // A from "SYSCALL PRN A"
        } 
        else if (syscallType == "HLT") {
            this->param1 = 2;
        } 
        else if (syscallType == "YIELD") {
            this->param1 = 3;
        } 
        else {
            std::cerr << "Error: Unknown syscall type: " << syscallType << std::endl;
            this->type = InstructionSet::HLT;  // Default to HLT on error
        }
    }
    else if (opcode == "NOP") {
        this->type = InstructionSet::NOP;
    } 
    else {
        std::cerr << "Error: Unknown instruction: " << opcode << std::endl;
        this->type = InstructionSet::HLT;  // Default to HLT on error
    }
    
    return *this;
}

std::ostream &operator<<(std::ostream &os, const Instruction &instr) {
    switch(instr.type) {
        case InstructionSet::SET: os << "SET " << static_cast<CPU_WordSizeSigned>(instr.param1) << " " << instr.param2; break;
        case InstructionSet::CPY: os << "CPY " << instr.param1 << " " << instr.param2; break;
        case InstructionSet::CPYI: os << "CPYI " << instr.param1 << " " << instr.param2; break;
        case InstructionSet::CPYI2: os << "CPYI2 " << instr.param1 << " " << instr.param2; break;
        case InstructionSet::ADD: os << "ADD " << instr.param1 << " " << static_cast<CPU_WordSizeSigned>(instr.param2); break;
        case InstructionSet::ADDI: os << "ADDI " << instr.param1 << " " << instr.param2; break;
        case InstructionSet::SUBI: os << "SUBI " << instr.param1 << " " << instr.param2; break;
        case InstructionSet::JIF: os << "JIF " << instr.param1 << " " << instr.param2; break;
        case InstructionSet::PUSH: os << "PUSH " << instr.param1; break;
        case InstructionSet::POP: os << "POP " << instr.param1; break;
        case InstructionSet::CALL: os << "CALL " << instr.param1; break;
        case InstructionSet::RET: os << "RET"; break;
        case InstructionSet::HLT: os << "HLT"; break;
        case InstructionSet::USER: os << "USER " << instr.param1; break;
        case InstructionSet::SYSCALL: 
            os << "SYSCALL";
            if(instr.param1 == 1)
                os << " PRN " << instr.param2;
            else if(instr.param1 == 2)
                os << " HLT";
            else if(instr.param1 == 3)
                os << " YIELD";
            break;
        case InstructionSet::NOP: os << "NOP"; break;
        default: 
            os << "ERROR: OPERATOR << DEFAULT CASE";
    }
    return os;
}
