# Memory Location / Memory mapped Register 
# 0 - Program Counter 
# 1 - Stack pointer 
# 2 - System call result 
# 3 - Number of instructions executed so far 
# 4-20 - Reserved for other uses

Begin Data Section
0 3000 # [0] Program starts from 1000
1 3999 # [1] SP (stack starts at 1999, grows downward)

3000 10     # N = 10 (number of elements)
3001 1     # Element 0
3002 2     # Element 1
3003 3     # Element 2
3004 4     # Element 3
3005 5     # Element 4
3006 6     # Element 5
3007 7     # Element 6
3008 8     # Element 7
3009 9     # Element 8
3010 10     # Element 9
3999 0
End Data Section

Begin Instruction Section

# REGISTERS:
# 4: len
# 5: array pointer
# 6 = len - 1
# 7: i
# 8: j
# 9: temp
# 10: temp2
# 11: constant 0

# Reverse Array
3000 CPY 3000 4 # len = mem[1000]
3001 SET 3001 5 # array pointer (points to the first element)
3002 CPY 4 6
3003 ADD 6 -1 # len - 1 in reg6
3004 SET 0 11 # constant 0 in reg11
3005 SET 0 8 # j = 0
#outer loop
    #Loop condition: 0 < temp(len - 1 - j)
    3006 CPY 6 9 # temp = len - 1
    3007 CPY 8 10 # temp2 = j
    3008 SUBI 11 10 # temp2 = -j
    3009 ADDI 9 10 # temp = len - 1 - j
    3010 JIF 9 3032 # if temp <= 0, end loop
    
    3011 SET 0 7 # i = 0
    #inner loop
        #Loop condition: 0 < temp(len - 1 - j - i)
        # temp = len - 1 - j
        3012 CPY 6 9 # temp = len - 1
        3013 CPY 8 10 # temp2 = j
        3014 SUBI 11 10 # temp2 = -j
        3015 ADDI 9 10 # temp = len - 1 - j
        3016 CPY 7 10 # temp2 = i
        3017 SUBI 11 10 # temp2 = -i
        3018 ADDI 9 10 # temp = len - 1 - j - i
        3019 JIF 9 3030 # if temp <= 0, exit loop

        # put &arr + i in reg19
        # put &arr + i + 1 in reg20
        # call swap(i, i+1)
        3020 CPY 5 9 # temp = arr
        3021 ADDI 9 7 # temp = arr + i
        3022 CPY 9 19 # &arr + i in reg19
        3023 CPY 5 9 # temp = arr
        3024 ADDI 9 7 # temp = arr + i
        3025 ADD 9 1 # temp = arr + i + 1
        3026 CPY 9 20 # &arr + i + 1 in reg20
        3027 CALL 3200 # Call swap subroutine

        3028 ADD 7 1 # ++i
        3029 SET 3012 0 # continue inner loop

    # end of inner loop

    3030 ADD 8 1 # ++j
    3031 SET 3006 0 # continue outer loop
# end of outer loop


# Reverse ended, start printing
# Print the reversed array

3032 SET 0 7 # i = 0
# printing loop
    #loop condition: (i < len) == (0 < len - i)
    3033 CPY 4 9 # temp = len
    3034 CPY 7 10 # temp2 = i
    3035 SUBI 11 10 # temp2 = -i
    3036 ADDI 9 10 # temp = len - i
    3037 JIF 9 3044 # if temp <= 0, exit loop

    3038 CPY 5 9 # temp = arr
    3039 ADDI 9 7 # temp = arr + i
    3040 CPYI 9 10 # temp2 = mem[arr + i]
    3041 SYSCALL PRN 10 # Print temp2 = arr + i

    3042 ADD 7 1 # ++i
    3043 SET 3033 0 # continue loop

3044 HLT # program end

# swap subroutine / starts from 1200.
# before calling this subroutine, make sure that:
# A's address is in register 19
# B's address is in register 20

# What it does is:
# swap(A, B)

# temp = A
# A = B
# B = temp
3200 CPYI 19 16     # temp = *A     - *A is at reg 16
3201 CPYI2 20 19    # *A = *B       - *A is now *B
3202 SET 16 15      # Put address of temp which is 16 into register 15
3203 CPYI2 15 20    # *B = temp     - mem[mem[20]] = (mem[mem[15]] === mem[16])
3204 RET # Return to caller


End Instruction Section
