# GTU-C312 CPU Simulator

A simple CPU simulator with a custom instruction set and cooperative operating system implementation.

## Overview

This project simulates a hypothetical CPU with a minimal instruction set (C312).

### Memory Layout
| Address Range | Purpose | Access |
|---------------|---------|--------|
| 0-20 | System Registers | All modes |
| 21-999 | OS Data & Instructions | Kernel only |
| 1000-1999 | Thread #1 | User mode |
| 2000-2999 | Thread #2 | User mode |
| ... | Threads #3-9 | User mode |
| 10000-10999 | Thread #10 | User mode |

### System Registers
| Address | Register | Description |
|---------|----------|-------------|
| 0 | Program Counter | Current instruction pointer |
| 1 | Stack Pointer | Stack memory pointer |
| 2 | System Call Result | Result of system calls |
| 3 | Instruction Counter | Total instructions executed |
| 4-20 | Reserved | For future use |

## Instruction Set

| Instruction | Semantic | Description |
|-------------|----------|-------------|
| `SET B A` | `mem[A] = B` | Sets number B to memory address A |
| `CPY A1 A2` | `mem[A2] = mem[A1]` | Copy the value in memory address A1 to memory address A2 |
| `CPYI A1 A2` | `mem[A2] = mem[mem[A1]]` | Copy the memory address indexed by A1 to memory address A2 |
| `CPYI2 A1 A2` | `mem[mem[A2]] = mem[mem[A1]]` | Copy the memory address indexed by A1 to memory address indexed by A2 |
| `ADD A B` | `mem[A] = mem[A] + B` | Adds number B to memory address A |
| `ADDI A1 A2` | `mem[A1] = mem[A1] + mem[A2]` | Adds the value in memory address A2 to memory address A1 |
| `SUBI A1 A2` | `mem[A2] = mem[A1] - mem[A2]` | Subtracts the value in memory address A2 from memory address A1 |
| `JIF A C` | `PC = if mem[A] <= 0 then C else PC+1` | Jump if the value in memory address A is less than or equal to 0 |
| `PUSH A` | `mem[SP] = mem[A]; SP = SP - 1` | Pushes the value in memory address A to the stack |
| `POP A` | `mem[A] = mem[SP]; SP = SP + 1` | Pops the value from the stack to memory address A |
| `CALL C` | `PC = C; PUSH PC` | Calls the subroutine at address C and pushes the current PC to the stack |
| `RET` | `POP SP` | Returns from subroutine |
| `HLT` | Halts CPU | Halt CPU execution |
| `USER A` | | Switches to user mode and jumps to address A |
| `SYSCALL PRN A` | | Prints mem[A] |
| `SYSCALL HLT` | | Halts the thread |
| `SYSCALL YIELD` | | Thread yields the CPU |

## Implementation

### Files Structure
```
├── CPU.cpp             
├── CPU.h               
├── Instruction.cpp     
├── Instruction.h       
├── Simulator.cpp       
├── Simulator.h         
├── main.cpp            
├── Makefile            
└── examples/           # Assembly examples
    ├── bubblesort.asm      
    ├── countdown.asm       
    ├── example.asm        
    ├── linearsearch.asm   
    └── reverse.asm         
```

### Program Format
```
#Program File Format
Begin Data Section
0 0     # Program Counter
1 0     # Stack Pointer
2 0     # System Call Result
3 0     # Instruction Counter
...
End Data Section

Begin Instruction Section
0 SET 10 50    # Instructions start here
1 ADDI 50 51   # Comments after #
...
End Instruction Section
```

## Usage

### Running the Simulator
```bash
# Normal execution
./simulator program.gtuc312

# Debug mode 1 - Memory dump after each instruction
./simulator program.gtuc312 -D 1

# Debug mode 2 - Step-by-step execution
./simulator program.gtuc312 -D 2

# Debug mode 3 - Thread table monitoring
./simulator program.gtuc312 -D 3
```

### Debug Modes
- **Mode 0**: Memory output after program completion
- **Mode 1**: Memory output after each instruction
- **Mode 2**: Interactive step-by-step execution
- **Mode 3**: Thread table output after syscalls

