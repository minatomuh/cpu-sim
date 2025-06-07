#include <iostream>
#include <string>
#include <cstdlib>
#include "Simulator.h"

int main(int argc, char* argv[]) {
    const char* programFile = nullptr;
    int debugMode = 0;
    if(argc == 2) {
        programFile = argv[1];
    } else if(argc == 4 && std::string(argv[2]) == "-D") {
        programFile = argv[1];
        debugMode = std::atoi(argv[3]);

        if(debugMode < 0 || debugMode > 3) {
            std::cerr << "Error: Debug mode must be 0, 1, 2, or 3." << std::endl;
            return 1;
        }
    } else {
        std::cerr << "Usage:\n"
                  << "  " << argv[0] << " <program_file>\n"
                  << "  " << argv[0] << " <program_file> -D <debug_mode (0-3)>" << std::endl;
        return 1;
    }

    Simulator simulator;
    // 0: print memory at the end,
    // 1: print memory each instruction, 
    // 2: press key step by step, 
    // 3: print thread table each syscall
    simulator.setDebugMode(debugMode); 
    simulator.loadProgram(programFile);
    simulator.run(1000000); // 1 MHz

    return 0;

}