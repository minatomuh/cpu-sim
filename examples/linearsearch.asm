# Memory Location / Memory mapped Register 
# 0 - Program Counter 
# 1 - Stack pointer 
# 2 - System call result 
# 3 - Number of instructions executed so far 
# 4-20 - Reserved for other uses

# Linear Search
Begin Data Section
0 2000 # [0] Program starts from 2000
1 2999 # [1] SP (stack starts at 2999, grows downward)

2000 10 # N = 10
2001 91 # Key to search
2002 10 # Array[0]
2003 15 # Array[1]
2004 42 # Array[2]
2005 88 # Array[3]
2006 99 # Array[4]
2007 54 # Array[5]
2008 91 # Array[6] - match
2009 14 # Array[7]
2010 68 # Array[8]
2011 33 # Array[9]

2999 0
End Data Section

Begin Instruction Section

# Simple Linear Search Implementation

# REGISTERS:
# 4: len
# 5: array pointer
# 6: i
# 7: key
# 8: result
# 9: temp
# 10: temp2
# 11: temp3
# 12: temp4
# 13: constant 0

# for(i = 0; i < len; ++i)
#    if(arr[i] - key <= 0) {
#        if(key - arr[i] <= 0) {
#            // found
#            print i;
#            halt;
#        }
#    }
# }
# print -1; halt;

2000 CPY 2000 4 # len = mem[2000] (N)
2001 SET 2002 5 # arr = 2002 (array start address)
2002 CPY 2001 7 # key = mem[2001] (key to search)
2003 SET -1 8 # result = -1

2004 SET 0 6 # i = 0
# printing loop
    #loop condition: (i < len) == (0 < len - i)
    2005 CPY 4 9 # temp = len
    2006 CPY 6 10 # temp2 = i
    2007 SUBI 13 10 # temp2 = -i
    2008 ADDI 9 10 # temp = len - i
    2009 JIF 9 2022 # if temp <= 0, exit loop

        2010 CPY 5 9 # temp = arr
        2011 ADDI 9 6 # temp = arr + i
        2012 CPYI 9 10 # temp2 = mem[arr + i]
        2013 CPY 10 11 # temp3 = mem[arr + i]

        # check if key - arr[i] <= 0
        2014 SUBI 7 10 # temp2 = key - mem[arr + i]
        2015 JIF 10 2017 # go check reverse
        2016 SET 2020 0 # break if statement, not found

        # check if arr[i] - key <= 0
        2017 CPY 7 12 # temp4 = key
        2018 SUBI 11 12 # temp4 = arr[i] - key
        2019 JIF 12 2024 # if arr[i] - key <= 0, found

    2020 ADD 6 1 # ++i
    2021 SET 2005 0 # continue loop

#loop_end
2022 SYSCALL PRN 8 # print -1, not found
2023 HLT # program end

#found
2024 SYSCALL PRN 6 # Print found index
2025 HLT

End Instruction Section
