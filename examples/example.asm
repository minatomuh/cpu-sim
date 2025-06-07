#Program sample 
Begin Data Section 
0 0 # PC
1 63 # SP
2 0 # syscall result
3 0 # instructions executed
63 0
End Data Section 
Begin Instruction Section 
0 SET 10 50    # i = 10 
1 SET 0 51   # sum = 0 
2 ADDI 51 50  # sum = sum + i 
3 ADD 50 -1  # i = i – 1
4 JIF 50 7   # Go to 6 if i <= 0 
5 SYSCALL PRN 51  # print the sum so far 
6 SET 2 0    # Go to 2 – remember address 0 is the program counter 
7 HLT        # end of program, the result is in memory address 51 (sum) 
End Instruction Section