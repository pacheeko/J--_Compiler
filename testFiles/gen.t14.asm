	.globl main
	.data
label4: .asciiz "minint is "
label5: .asciiz "\n"

	.text
main:
sub $sp, $sp, 8
li $t0, 0
sw $t0, 4($sp)
li $t0, 1000000
sw $t0, 0($sp)
label0:
lw $t1, 4($sp)
lw $t2, 4($sp)
lw $t3, 0($sp)
sub $t1, $t2, $t3
sgt $t0, $t1, $t2
beq $0, $t0, label1
lw $t1, 4($sp)
lw $t2, 0($sp)
add $t0, $t1, $t2
sw $t0, 4($sp)
b label0
label1:
lw $t1, 4($sp)
lw $t2, 0($sp)
sub $t0, $t1, $t2
sw $t0, 4($sp)
label2:
lw $t1, 4($sp)
lw $t2, 4($sp)
li $t3, 1
sub $t1, $t2, $t3
sgt $t0, $t1, $t2
beq $0, $t0, label3
lw $t1, 4($sp)
li $t2, 1
add $t0, $t1, $t2
sw $t0, 4($sp)
b label2
label3:
li $v0, 4
la $a0, label4
syscall
li $v0, 1
lw $a0, 4($sp)
syscall
li $v0, 4
la $a0, label5
syscall
end:
li $v0, 10
syscall
