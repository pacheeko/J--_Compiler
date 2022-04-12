	.globl main
	.data
label0: .asciiz "\n(it's ok if the following aren't 0/false)\n"
label1: .asciiz "\tlocal int default value: "
label2: .asciiz "\n"
label3: .asciiz "\tlocal boolean default value: "
label4: .asciiz "true"
label5: .asciiz "false"
label8: .asciiz "\n"
label9: .asciiz "global int default value: "
label10: .asciiz "\n"
label11: .asciiz "global boolean default value: "
label12: .asciiz "true"
label13: .asciiz "false"
label16: .asciiz "\n"

	.text
main:
sub $sp, $sp, 16
li $v0, 4
la $a0, label9
syscall
li $v0, 1
lw $a0, 12($sp)
syscall
li $v0, 4
la $a0, label10
syscall
li $v0, 4
la $a0, label11
syscall
li $v0, 4
lw $a0, 8($sp)
beq $0, $a0, label14
la $a0, label12
syscall
b label15
label14:
la $a0, label13
syscall
label15:
li $v0, 4
la $a0, label16
syscall
jal foo
jal foo
end:
li $v0, 10
syscall
foo:
li $v0, 4
la $a0, label0
syscall
li $v0, 4
la $a0, label1
syscall
li $v0, 1
lw $a0, 4($sp)
syscall
li $v0, 4
la $a0, label2
syscall
li $v0, 4
la $a0, label3
syscall
li $v0, 4
lw $a0, 0($sp)
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
li $t0, 123
sw $t0, 4($sp)
li $t0, 1
sw $t0, 0($sp)
jr $ra
