#include <thread>
#include <chrono>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>
#include <vector>
#include "Simulator.h"
#include "CPU.h"

Simulator::Simulator() : cpu(data_memory.data(), instruction_memory.data()), data_memory{}, instruction_memory{}, debugMode(0){
}

Simulator::~Simulator() {
}

void Simulator::run(size_t clockspeed) {
    std::cout << "-------------PROGRAM START-------------\n";
    switch(debugMode) {
        default:
        case 0:
            while(!cpu.isHalted()) {
                step();                
                std::this_thread::sleep_for(std::chrono::milliseconds(1000 / clockspeed));
            }
            std::cout << "-------------PROGRAM END-------------\n";
            printMemoryHexSkipZero();
            break;
        case 1:
            while(!cpu.isHalted()) {
                step();
                printMemoryHexSkipZero();
                std::this_thread::sleep_for(std::chrono::milliseconds(1000 / clockspeed));
            }
            std::cout << "-------------PROGRAM END-------------\n";
            break;
        case 2:
            while(!cpu.isHalted()) {
                step();
                printMemoryHexSkipZero();
                std::this_thread::sleep_for(std::chrono::milliseconds(1000 / clockspeed));
                std::cout << "Press any key to step one tick...\n";
                std::cin.get();
            }
            std::cout << "-------------PROGRAM END-------------\n";
            break;
        case 3:
            while(!cpu.isHalted()) {
                Instruction current_instr = instruction_memory[data_memory[0]]; // instr[PC]
                step();
                //printMemoryHexSkipZero();
                std::this_thread::sleep_for(std::chrono::milliseconds(1000 / clockspeed));
                if(current_instr.type == InstructionSet::SYSCALL) { 
                    // After each context switch and syscall, print thread table
                    _printThreadTable();
                }
            }
            printMemoryHexSkipZero();
            std::cout << "-------------PROGRAM END-------------\n";
            break;
    }
    
}

void Simulator::reset() {
    cpu.reset();
}

void Simulator::step() {
    cpu.step();
}

void Simulator::loadProgram(const char *filename) {
    // open file and start reading
    std::ifstream file(filename);
    if(!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }

    std::string line;
    bool isDataSection = false;
    bool isInstructionSection = false;

    while(std::getline(file, line)) {
        // Trim until comment
        size_t commentPos = line.find('#');
        if(commentPos != std::string::npos) {
            line = line.substr(0, commentPos);
        }
        
        // Trim whitespace and empty line
        line.erase(0, line.find_first_not_of(" \t"));
        if(line.empty()) 
            continue;
        
        // Check for section markers
        if(line.find("Begin Data Section") != std::string::npos) {
            isDataSection = true;
            isInstructionSection = false;
            continue;
        }
        else if(line.find("End Data Section") != std::string::npos) {
            isDataSection = false;
            continue;
        }
        else if(line.find("Begin Instruction Section") != std::string::npos) {
            isInstructionSection = true;
            isDataSection = false;
            continue;
        }
        else if(line.find("End Instruction Section") != std::string::npos) {
            isInstructionSection = false;
            continue;
        }
        
        // Parse data section entries
        if(isDataSection) {
            size_t address;
            CPU_WordSize value;
            std::istringstream iss(line);
            if(iss >> address >> value) {
                data_memory[address] = value;
            }
        }
        // Parse instruction section entries
        else if(isInstructionSection) {
            size_t address;
            std::istringstream iss(line);
            if(iss >> address) {
                std::string instructionStr;
                std::getline(iss, instructionStr);
                instruction_memory[address].parse(instructionStr);
            }
        }
    }

    file.close();

    // to show if the program loaded correctly
    std::cout << "-------------INSTRUCTIONS-------------\n";
    for(auto i : instruction_memory) {
        if(i.type != InstructionSet::NOP)
            std::cout << i << "\n";
    }
}

void Simulator::printMemoryHex() {
    const uint8_t* bytes = reinterpret_cast<const uint8_t*>(data_memory.data());
    const size_t byte_count = MEMORY_SIZE * sizeof(CPU_WordSize);
    const size_t bytes_per_line = 16;

    for(size_t i = 0; i < byte_count; i += bytes_per_line) {
        // Print the memory offset
        std::cout << std::setw(8) << std::setfill('0') << std::hex << i << ": ";

        // Print hex bytes with spacing between 32-bit words (every 4 bytes)
        for(size_t j = 0; j < bytes_per_line; ++j) {
            if(i + j < byte_count) {
                std::cout << std::setw(2) << std::setfill('0') << static_cast<int>(bytes[i + j]) << " ";
            } else {
                std::cout << "   ";
            }

            // Extra space after every 4 bytes
            if((j + 1) % 4 == 0) {
                std::cout << " ";
            }
        }

        // Optional: print ASCII representation
        std::cout << " ";
        for(size_t j = 0; j < bytes_per_line; ++j) {
            if(i + j < byte_count) {
                char c = static_cast<char>(bytes[i + j]);
                std::cout << (std::isprint(c) ? c : '.');
            }
        }

        std::cout << "\n";
    }
}

void Simulator::printMemoryHexSkipZero() {
    const uint8_t* bytes = reinterpret_cast<const uint8_t*>(data_memory.data());
    const size_t byte_count = MEMORY_SIZE * sizeof(CPU_WordSize);
    const size_t bytes_per_line = 16;

    for(size_t i = 0; i < byte_count; i += bytes_per_line) {
        // Check if the entire line is zero
        bool all_zero = true;
        for(size_t j = 0; j < bytes_per_line && (i + j) < byte_count; ++j) {
            if(bytes[i + j] != 0) {
                all_zero = false;
                break;
            }
        }
        
        if(all_zero) {
            continue;  // Skip printing this line
        }

        // Print the memory offset
        std::cerr << std::setw(8) << std::setfill('0') << std::hex << i << ": ";

        // Print hex bytes with spacing between 32-bit words (every 4 bytes)
        for(size_t j = 0; j < bytes_per_line; ++j) {
            if(i + j < byte_count) {
                std::cerr << std::setw(2) << std::setfill('0') << static_cast<int>(bytes[i + j]) << " ";
            } else {
                std::cerr << "   ";
            }

            // Extra space after every 4 bytes
            if((j + 1) % 4 == 0) {
                std::cerr << " ";
            }
        }

        // Optional: print ASCII representation
        std::cerr << " ";
        for(size_t j = 0; j < bytes_per_line; ++j) {
            if(i + j < byte_count) {
                char c = static_cast<char>(bytes[i + j]);
                std::cerr << (std::isprint(c) ? c : '.');
            }
        }

        std::cerr << "\n";
    }
}

void Simulator::printMemoryDec() {
    constexpr size_t WORDS_PER_LINE = 4;
    constexpr size_t BYTES_PER_WORD = sizeof(CPU_WordSize);

    for(size_t i = 0; i < data_memory.size(); i += WORDS_PER_LINE) {
        std::cout << std::setw(8) << std::setfill('0') << std::hex << i * BYTES_PER_WORD << ": ";
        for(size_t j = 0; j < WORDS_PER_LINE; ++j) {
            if(i + j < data_memory.size()) {
                std::cout << std::setw(8) << std::setfill('0') << std::hex << data_memory[i + j] << " ";
            } else {
                std::cout << "         "; // For incomplete lines
            }
        }
        std::cout << std::dec << "\n";
    }
}

void Simulator::printThreadTable() {
    std::cerr << "---------------------Thread Table---------------------\n";
    int idx = 100;
    for(int j = 0; j < 4; ++j) {
        std::cerr << "Thread " << j << ": \n";
        std::cerr << std::setw(15) << std::setfill(' ') << "Thread ID: " << std::setw(10) << data_memory[idx++] << "\n";
        std::cerr << std::setw(15) << std::setfill(' ') << "Start time: " << std::setw(10) << data_memory[idx++] << "\n";
        std::cerr << std::setw(15) << std::setfill(' ') << "Executions: " << std::setw(10) << data_memory[idx++] << "\n";
        std::cerr << std::setw(15) << std::setfill(' ') << "State: " << std::setw(10) << data_memory[idx++] << "\n";
        std::cerr << std::setw(15) << std::setfill(' ') << "PC: " << std::setw(10) << data_memory[idx++] << "\n";
        std::cerr << std::setw(15) << std::setfill(' ') << "SP: " << std::setw(10) << data_memory[idx++] << "\n";
        std::cerr << std::setw(15) << std::setfill(' ') << "syscallResult: " << std::setw(10) << data_memory[idx++] << "\n";
        std::cerr << std::setw(15) << std::setfill(' ') << "instrCount: " << std::setw(10) << data_memory[idx++] << "\n";
        for (int k = 0; k < 17; ++k) {
            std::string regName = "R" + std::to_string(k + 4);
            std::cerr << std::setw(13) << std::setfill(' ') << regName << ": " << std::setw(10) << data_memory[idx++] << "\n";
        }
        std::cerr << "\n";
        //idx += 25;
    }
    std::cerr << "---------------------Thread Table End---------------------\n";
}

void Simulator::_printThreadTable() {
    // Hardcoded for OS + 3 Threads
    std::vector<std::vector<std::string>> thread_lines(4);
    int idx = 100;

    std::vector<std::pair<std::string, size_t>> fields = {
        {"Thread ID", 10},
        {"Start time", 10},
        {"Executions", 10},
        {"State", 10},
        {"PC", 10},
        {"SP", 10},
        {"syscallResult", 13},
        {"instrCount", 10}
    };

    // Calculate maximum field name width
    size_t max_field_width = 0;
    for(const auto& field : fields) {
        if(field.first.size() > max_field_width) {
            max_field_width = field.first.size();
        }
    }

    max_field_width += 2; // For the ": " part

    for(int j = 0; j < 4; ++j) {
        //int idx = idx_base;
        thread_lines[j].push_back("| Thread " + std::to_string(j) + ":");
        
        // Add all fields to this thread's lines
        for(const auto& field : fields) {
            std::string line = "| " + field.first + ": ";
            line += std::string(max_field_width - line.size() + 2, ' '); // +2 for "| "
            line += std::to_string(static_cast<CPU_WordSizeSigned>(data_memory[idx++]));
            thread_lines[j].push_back(line);
        }
        
        // Add registers
        for(int k = 0; k < 17; ++k) {
            std::string regName = "R" + std::to_string(k + 4) + ":";
            std::string line = "| " + regName;
            line += std::string(15 - regName.size(), ' '); // Align register values
            line += std::to_string(static_cast<CPU_WordSizeSigned>(data_memory[idx++]));
            thread_lines[j].push_back(line);
        }
        
        //idx_base += 25; // Skip the padding for next thread
    }

    // Print horizontal separator line
    auto print_separator = [](int columns, int width) {
        for(int j = 0; j < columns; ++j) {
            std::cerr << "+" << std::string(width - 1, '-');
        }
        std::cerr << "+\n";
    };

    const int column_width = 25; // Adjust as needed

    // Print all threads side by side with borders
    for(size_t i = 0; i < 26; ++i) {
        // Print horizontal separator for section headers
        if(i == 0 || i == 1 || i == 9) {
            print_separator(4, column_width);
        }
        
        for(int j = 0; j < 4; ++j) {
            if(i < thread_lines[j].size()) {
                std::cerr << std::left << std::setw(column_width) << thread_lines[j][i];
            } else {
                std::cerr << "|" << std::setw(column_width - 1) << "";
            }
        }
        std::cerr << "|\n";
    }

    // Final separator
    print_separator(4, column_width);
}
