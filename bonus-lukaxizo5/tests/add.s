.text
li x3 1        # x3 = 1
li x4 2        # x4 = 2
li x5 3        # x5 = 3
li x6 4        # x6 = 4
li x7 5        # x7 = 5
li x11 0       # x11 = 0 (result)
add x11 x11 x3 # x11 += x3
add x11 x11 x4 # x11 += x4
add x11 x11 x5 # x11 += x5
add x11 x11 x6 # x11 += x6
add x11 x11 x7 # x11 += x7
li x10 1       # x10 = 1 (print integer)
ecall          # ecall
li x10 10      # x10 = 10 (exit)
ecall          # ecall    

# This program prints sum of x3, x4, x5, x6, x7 registers