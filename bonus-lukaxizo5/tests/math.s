.text

start:
# Test SUB (x10 = x11 - x12)
li x11, 100
li x12, 40
sub x10, x11, x12  # x10 = 100 - 40 = 60
# Test DIV (x13 = x14 / x15)
li x14, 50
li x15, 7
div x13, x14, x15  # x13 = 50 / 7 = 7 (integer division)
# Test REM (x16 = x17 % x18)
li x17, 50
li x18, 7
rem x16, x17, x18  # x16 = 50 % 7 = 1 (remainder)
# Test SLL (x19 = x20 << x21)
li x20, 5
li x21, 3
sll x19, x20, x21  # x19 = 5 << 3 = 5 * 2^3 = 40
# Test SRL (x22 = x23 >> x24)
li x23, 64
li x24, 2
srl x22, x23, x24  # x22 = 64 >> 2 = 64 / 2^2 = 16

# PRINT ALL CALCULATED VALUES
mv x11 x10
li x10 1
ecall
mv x11 x13
ecall
mv x11 x16
ecall
mv x11 x19
ecall
mv x11 x22
ecall
li x10 10
ecall

# This program tests sub, div, rem, srl, sll functions and prints all results