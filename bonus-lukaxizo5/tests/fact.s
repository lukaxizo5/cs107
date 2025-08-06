.text

li x10, 10
addi sp, sp, -4
sw x10, 0(sp)
call fact
addi sp, sp, 4
li x10, 1
mv x11, x20
ecall
li x10, 10
ecall

fact:
	lw x10, 0(sp) # download n from the stack 
	bne x10, x0, abovezero # if x10 != 0 jump to abovezero
	addi x20, x0, 1 # x20 = 1 and return that
	ret
abovezero:
	lw x10, 0(sp) # load 0(sp) in x10 so x10 = n
	addi x10, x10, -1 # x10 = x10 - 1
	addi sp, sp, -8 # sp chamovwiot 8it
	sw x10, 0(sp) # shevinaxot x10 spze
    sw ra, 4(sp) # shevinaxot ra 4spze
	call fact #factoriali x10ze rac daabrunebs values x10shi
    lw ra, 4(sp) #agvadginot ra
    addi sp, sp, 8 #avkecot stack
	lw x10, 0(sp) #load 0(sp) in x10 so x10 = n
	mul x20, x20, x10 # mul x20 x20 x10 anu x20 = x20 * n da
	# x20 storavda fact(n-1)s
	ret

# This program prints factorial of x10 (10)