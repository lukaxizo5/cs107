.text

li x7 10        # x7 = 10
li x8 20        # x8 = 20
addi sp sp -8   # allocate stack
sw x7 0(sp)     # store x7 on 0(sp)
sw x8 4(sp)     # store x8 on 4(sp)
li x7 0         # x7 = 0
li x8 0         # x8 = 0
lw x11 0(sp)    # download value from stack (10)
lw x12 4(sp)    # download value from stack (20)
mul x11 x11 x12 # x11 = x11 * x12 (10 * 20 = 200)
li x10 1        # x10 = 1 (print integer)
ecall           # ecall
li x10 10       # x10 = 10 (exit)
ecall           # ecall

# This program tests sw/lw and mul  functions and stack pointer management