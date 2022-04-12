	.globl main
	.data
label1: .asciiz " = \n"
label2: .asciiz "\t"
label3: .asciiz " (base 2)\n"
label4: .asciiz "\t"
label5: .asciiz " (base 8)\n"
label6: .asciiz "\t"
label7: .asciiz " (base 10)\n"

	.text
main:
sub $sp, $sp, 0
li $a0, 0jal do
li $a0, 17jal do
li $a0, 42jal do
end:
li $v0, 10
syscall
convert:
sub $sp, $sp, 4
sw $a0, 0($sp)
sub $sp, $sp, 4
sw $a1, 0($sp)
lw $t1, 4($sp)
li $t2, 0
seq $t0, $t1, $t2
beq $0, $t0, label0
label0:
lw $a1, 0($sp)
lw $t1, 4($sp)
lw $t2, 0($sp)
div $t0, $t1, $t2
move $a0, $t0
jal convert
lw $t1, 4($sp)
lw $t2, 0($sp)
rem $t0, $t1, $t2
move $a0, $v0
li $v0, 1

syscall
add $sp, $sp, 8
jr $ra
do:
sub $sp, $sp, 4
sw $a0, 0($sp)
li $v0, 1
lw $a0, 0($sp)
syscall
li $v0, 4
la $a0, label1
syscall
li $v0, 4
la $a0, label2
syscall
li $a1, 2lw $a0, 0($sp)
jal convert
li $v0, 4
la $a0, label3
syscall
li $v0, 4
la $a0, label4
syscall
li $a1, 8lw $a0, 0($sp)
jal convert
li $v0, 4
la $a0, label5
syscall
li $v0, 4
la $a0, label6
syscall
li $a1, 10lw $a0, 0($sp)
jal convert
li $v0, 4
la $a0, label7
syscall
add $sp, $sp, 4
jr $ra
