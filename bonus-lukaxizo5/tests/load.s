.text

li x5 286331153  # x5 = 0x11111111
addi, sp, sp, -4 # allocate stack 
sw x5 0(sp)      # store x5 on 0(sp)
lb x11 0(sp)     # load BYTE from x5 -> x11 = 0x11 = 17 
li x10 1         # x10 = 1 (print integer)
ecall            # ecall
li x10 10        # x10 = 10 (exit)
ecall            # ecall