# ===== Compact Countdown Program =====

Begin Data Section
0   0   # [0] PC
1   29  # [1] SP (stack starts at 29, grows downward)
2   0   # [2] Syscall result
3   0   # [3] Instruction counter
4   0   # [4] Counter (starts at 3)
5   -1  # [5] Decrement value
# 6-28 unused
29  0   # [29] Stack bottom
End Data Section

Begin Instruction Section
# Initialize (reuses memory)
0   SET 8 4       # [4] = 3 (counter)
1   SET -1 5      # [5] = -1 (decrement value)

# Main loop
2   SYSCALL PRN 4 # Print counter
3   ADDI 4 5       # counter += -1
4   JIF 4 6       # If counter <= 0, jump to print
5   SET 2 0       # Loop back (PC=2)

# Termination
6   SYSCALL PRN 4 # Print final 0
7   HLT           # Halt
End Instruction Section