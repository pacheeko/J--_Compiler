	.globl main
	.data
label0: .asciiz "evaluated A\n"
label1: .asciiz "evaluated B\n"
label2: .asciiz "evaluated C\n"
label3: .asciiz "if ((A && B) || C) {...} else {...}, with A="
label4: .asciiz "true"
label5: .asciiz "false"
label8: .asciiz " B="
label9: .asciiz "true"
label10: .asciiz "false"
label13: .asciiz " C="
label14: .asciiz "true"
label15: .asciiz "false"
label18: .asciiz "\n"
label20: .asciiz "if-part executed\n"
label22: .asciiz "else-part executed\n"
label24: .asciiz "x = (A && !B) || C, with A="
label25: .asciiz "true"
label26: .asciiz "false"
label29: .asciiz " B="
label30: .asciiz "true"
label31: .asciiz "false"
label34: .asciiz " C="
label35: .asciiz "true"
label36: .asciiz "false"
label39: .asciiz "\n"
label40: .asciiz "x="
label41: .asciiz "true"
label42: .asciiz "false"
label45: .asciiz "\n"
label46: .asciiz "\n"
label47: .asciiz "\n"
label48: .asciiz "\n"
label49: .asciiz "\n"
label50: .asciiz "\n"
label51: .asciiz "\n"
label52: .asciiz "\n"
label53: .asciiz "\n"
label54: .asciiz "\n"
label55: .asciiz "\n"
label56: .asciiz "\n"
label57: .asciiz "\n"
label58: .asciiz "\n"
label59: .asciiz "\n"
label60: .asciiz "\n"

	.text
main:
sub $sp, $sp, 16
li $a2, 0li $a1, 0li $a0, 0jal eval
li $v0, 4
la $a0, label46
syscall
li $a2, 1li $a1, 0li $a0, 0jal eval
li $v0, 4
la $a0, label47
syscall
li $a2, 0li $a1, 1li $a0, 0jal eval
li $v0, 4
la $a0, label48
syscall
li $a2, 1li $a1, 1li $a0, 0jal eval
li $v0, 4
la $a0, label49
syscall
li $a2, 0li $a1, 0li $a0, 1jal eval
li $v0, 4
la $a0, label50
syscall
li $a2, 1li $a1, 0li $a0, 1jal eval
li $v0, 4
la $a0, label51
syscall
li $a2, 0li $a1, 1li $a0, 1jal eval
li $v0, 4
la $a0, label52
syscall
li $a2, 1li $a1, 1li $a0, 1jal eval
li $v0, 4
la $a0, label53
syscall
li $a2, 0li $a1, 0li $a0, 0jal assign
li $v0, 4
la $a0, label54
syscall
li $a2, 1li $a1, 0li $a0, 0jal assign
li $v0, 4
la $a0, label55
syscall
li $a2, 0li $a1, 1li $a0, 0jal assign
li $v0, 4
la $a0, label56
syscall
li $a2, 1li $a1, 1li $a0, 0jal assign
li $v0, 4
la $a0, label57
syscall
li $a2, 0li $a1, 0li $a0, 1jal assign
li $v0, 4
la $a0, label58
syscall
li $a2, 1li $a1, 0li $a0, 1jal assign
li $v0, 4
la $a0, label59
syscall
li $a2, 0li $a1, 1li $a0, 1jal assign
li $v0, 4
la $a0, label60
syscall
li $a2, 1li $a1, 1li $a0, 1jal assign
end:
li $v0, 10
syscall
A:
li $v0, 4
la $a0, label0
syscall
lw $v0, 8($sp)
jr $ra
jr $ra
B:
li $v0, 4
la $a0, label1
syscall
lw $v0, 4($sp)
jr $ra
jr $ra
C:
li $v0, 4
la $a0, label2
syscall
lw $v0, 0($sp)
jr $ra
jr $ra
eval:
sub $sp, $sp, 4
sw $a0, 0($sp)
sub $sp, $sp, 4
sw $a1, 0($sp)
sub $sp, $sp, 4
sw $a2, 0($sp)
lw $t0, 8($sp)
sw $t0, 20($sp)
lw $t0, 4($sp)
sw $t0, 16($sp)
lw $t0, 0($sp)
sw $t0, 12($sp)
li $v0, 4
la $a0, label3
syscall
li $v0, 4
lw $a0, 20($sp)
beq $0, $a0, label6
la $a0, label4
syscall
b label7
label6:
la $a0, label5
syscall
label7:
li $v0, 4
la $a0, label8
syscall
li $v0, 4
lw $a0, 16($sp)
beq $0, $a0, label11
la $a0, label9
syscall
b label12
label11:
la $a0, label10
syscall
label12:
li $v0, 4
la $a0, label13
syscall
li $v0, 4
lw $a0, 12($sp)
beq $0, $a0, label16
la $a0, label14
syscall
b label17
label16:
la $a0, label15
syscall
label17:
li $v0, 4
la $a0, label18
syscall
jal A
jal B
and $t1, $t2, $t3
jal C
or $t0, $t1, $t2
beq $0, $t0, label19
b label21
label19:
label21:
add $sp, $sp, 12
jr $ra
assign:
sub $sp, $sp, 4
sw $a0, 0($sp)
sub $sp, $sp, 4
sw $a1, 0($sp)
sub $sp, $sp, 4
sw $a2, 0($sp)
lw $t0, 8($sp)
sw $t0, 20($sp)
lw $t0, 4($sp)
sw $t0, 16($sp)
lw $t0, 0($sp)
sw $t0, 12($sp)
li $v0, 4
la $a0, label24
syscall
li $v0, 4
lw $a0, 20($sp)
beq $0, $a0, label27
la $a0, label25
syscall
b label28
label27:
la $a0, label26
syscall
label28:
li $v0, 4
la $a0, label29
syscall
li $v0, 4
lw $a0, 16($sp)
beq $0, $a0, label32
la $a0, label30
syscall
b label33
label32:
la $a0, label31
syscall
label33:
li $v0, 4
la $a0, label34
syscall
li $v0, 4
lw $a0, 12($sp)
beq $0, $a0, label37
la $a0, label35
syscall
b label38
label37:
la $a0, label36
syscall
label38:
li $v0, 4
la $a0, label39
syscall
jal A
jal B
not $t3, $t4
and $t1, $t2, $t3
jal C
or $t0, $t1, $t2
sw $t0, 0($sp)
li $v0, 4
la $a0, label40
syscall
li $v0, 4
lw $a0, 0($sp)
beq $0, $a0, label43
la $a0, label41
syscall
b label44
label43:
la $a0, label42
syscall
label44:
li $v0, 4
la $a0, label45
syscall
add $sp, $sp, 12
jr $ra
