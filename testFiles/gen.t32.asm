	.globl main
	.data
label2: .asciiz "\n"
label5: .asciiz "\n"

	.text
main:
sub $sp, $sp, 8
li $t0, 0
sw $t0, 4($sp)
label0:
lw $t1, 4($sp)
li $t2, 10
slt $t0, $t1, $t2
beq $0, $t0, label1
li $v0, 1
lw $a0, 4($sp)
syscall
li $v0, 4
la $a0, label2
syscall
li $t0, 100
sw $t0, 0($sp)
label3:
lw $t1, 0($sp)
li $t2, 110
slt $t0, $t1, $t2
beq $0, $t0, label4
li $v0, 1
lw $a0, 0($sp)
syscall
li $v0, 4
la $a0, label5
syscall
lw $t1, 0($sp)
li $t2, 105
seq $t0, $t1, $t2
beq $0, $t0, label6
label6:
lw $t1, 0($sp)
li $t2, 1
add $t0, $t1, $t2
sw $t0, 0($sp)
b label3
label4:
lw $t1, 4($sp)
li $t2, 5
seq $t0, $t1, $t2
beq $0, $t0, label7
label7:
lw $t1, 4($sp)
li $t2, 1
add $t0, $t1, $t2
sw $t0, 4($sp)
b label0
label1:
end:
li $v0, 10
syscall
