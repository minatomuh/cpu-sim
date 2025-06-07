Begin Data Section
0 1000 # [0] Program starts from 1000
1 1999 # [1] SP (stack starts at 1999, grows downward)

1000 10     # N = 10 (number of elements to sort)
1001 65     # Element 0
1002 34     # Element 1
1003 25     # Element 2
1004 12     # Element 3
1005 22     # Element 4
1006 90     # Element 5
1007 88     # Element 6
1008 76     # Element 7
1009 50     # Element 8
1010 42     # Element 9
1999 0
End Data Section

Begin Instruction Section

#swap(int& a, int& b) {
#    if(a <= b) return;
#    int temp = a;
#    a = b;
#    b = temp;
#    return;
#}

#for (int j = 0; j < len - 1; j++) {
#    for (int i = 0; i < len - 1 - j; i++) {       
#        swap(arr[i], arr[i + 1]);
#    }
#}

# REGISTERS:
# 4: len
# 5: array pointer
# 6 = len - 1
# 7: i
# 8: j
# 9: temp
# 10: temp2
# 11: constant 0
# Bubble Sort
1000 CPY 1000 4 # len = mem[1000]

1001 SET 1001 5 # array pointer (points to the first element)
1002 CPY 4 6
1003 ADD 6 -1 # len - 1 in reg6
1004 SET 0 11 # constant 0 in reg11


1005 SET 0 8 # j = 0
#outer loop
    #Loop condition: 0 < temp(len - 1 - j)
    1006 CPY 6 9 # temp = len - 1
    1007 CPY 8 10 # temp2 = j
    1008 SUBI 11 10 # temp2 = -j
    1009 ADDI 9 10 # temp = len - 1 - j
    1010 JIF 9 1032 # if temp <= 0, end loop
    
    1011 SET 0 7 # i = 0
    #inner loop
        #Loop condition: 0 < temp(len - 1 - j - i)
        # temp = len - 1 - j
        1012 CPY 6 9 # temp = len - 1
        1013 CPY 8 10 # temp2 = j
        1014 SUBI 11 10 # temp2 = -j
        1015 ADDI 9 10 # temp = len - 1 - j
        1016 CPY 7 10 # temp2 = i
        1017 SUBI 11 10 # temp2 = -i
        1018 ADDI 9 10 # temp = len - 1 - j - i
        1019 JIF 9 1030 # if temp <= 0, exit loop

        # put &arr + i in reg19
        # put &arr + i + 1 in reg20
        # call swap
        1020 CPY 5 9 # temp = arr
        1021 ADDI 9 7 # temp = arr + i
        1022 CPY 9 19 # &arr + i in reg19
        1023 CPY 5 9 # temp = arr
        1024 ADDI 9 7 # temp = arr + i
        1025 ADD 9 1 # temp = arr + i + 1
        1026 CPY 9 20 # &arr + i + 1 in reg20
        1027 CALL 1200 # Call swap subroutine

        1028 ADD 7 1 # ++i
        1029 SET 1012 0 # continue inner loop

    # end of inner loop

    1030 ADD 8 1 # ++j
    1031 SET 1006 0 # continue outer loop
# end of outer loop

# Sorting ended, start printing
1032 SET 0 7 # i = 0
# printing loop
    #loop condition: (i < len) == (0 < len - i)
    1033 CPY 4 9 # temp = len
    1034 CPY 7 10 # temp2 = i
    1035 SUBI 11 10 # temp2 = -i
    1036 ADDI 9 10 # temp = len - i
    1037 JIF 9 1044 # if temp <= 0, exit loop

    1038 CPY 5 9 # temp = arr
    1039 ADDI 9 7 # temp = arr + i
    1040 CPYI 9 10 # temp2 = mem[arr + i]
    1041 SYSCALL PRN 10 # Print temp2 = arr + i

    1042 ADD 7 1 # ++i
    1043 SET 1033 0 # continue loop

1044 HLT # program end



# swap subroutine / starts from 1200.
# before calling this subroutine, make sure that:
# A's address is in register 19
# B's address is in register 20

# What it does is:
# if(mem[A] <= mem[B]) swap(A, B)

# approach: temp = mem[a] - mem[b]
# JIF temp swap
# RET
1200 CPYI 20 18 # valB(18) = mem[B]
1201 CPYI 19 17 # valA(17) = mem[A]
1202 SUBI 17 18 # valB = valA - valB
# now valB contains the difference
1203 JIF 18 1208 # if diff <= 0, no swap needed, jump to return

# temp = A
# A = B
# B = temp
1204 CPYI 19 16     # temp = *A     - *A is at reg 16
1205 CPYI2 20 19    # *A = *B       - *A is now *B
1206 SET 16 15      # Put address of temp which is 16 into register 15
1207 CPYI2 15 20    # *B = temp     - mem[mem[20]] = (mem[mem[15]] === mem[16])
1208 RET # Return to caller


End Instruction Section
