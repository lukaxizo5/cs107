.text
li x11 0         # x11 = 0
li x5 1          # x5 = 1
li x6 100        # x6 = 10
LOOP:            # LOOP
bgt x5 x6 OUT    # if x5 > x6 jump to OUT
add x11 x11 x5   # x11 += x5
addi x5 x5 1     # x5++
j LOOP           # jump to LOOP again
OUT:             # jump out of LOOP
li x10 1         # x10 = 1 (print integer)
ecall            # ecall
li x10 10        # x10 = 10 (exit)
ecall            # ecall

# This program prints sum of numbers from 1 (x5) to 100 (x6)