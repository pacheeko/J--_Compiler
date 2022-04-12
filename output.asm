	.globl main
	.data
label0: .asciiz "true"
label1: .asciiz "false"

	.text
main:
sub $sp, $sp, 4
li $t2, 5
li $t3, 3
sgt $t1, $t2, $t3
li $t3, 1
not $t2, $t3
and $t0, $t1, $t2
sw $t0, 0($sp)
li $v0, 4
lw $a0, 0($sp)
beq $0, $a0, label2
la $a0, label0
syscall
b label3
label2:
la $a0, label1
syscall
label3:
end:
li $v0, 10
syscall
