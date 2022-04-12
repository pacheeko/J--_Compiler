	.globl main
	.data
label2: .asciiz "fib("
label3: .asciiz ") = "
label4: .asciiz "\n"

	.text
main:
sub $sp, $sp, 4
li $t0, 0
sw $t0, 0($sp)
label0:
lw $t1, 0($sp)
li $t2, 46
sle $t0, $t1, $t2
beq $0, $t0, label1
li $v0, 4
la $a0, label2
syscall
li $v0, 1
lw $a0, 0($sp)
syscall
li $v0, 4
la $a0, label3
syscall
lw $a0, 0($sp)
jal fib
move $a0, $v0
li $v0, 1

syscall
li $v0, 4
la $a0, label4
syscall
lw $t1, 0($sp)
li $t2, 1
add $t0, $t1, $t2
sw $t0, 0($sp)
b label0
label1:
end:
li $v0, 10
syscall
fib:
sub $sp, $sp, 4
sw $a0, 0($sp)
lw $t1, 0($sp)
li $t2, 0
seq $t0, $t1, $t2
beq $0, $t0, label5
label5:
lw $t1, 0($sp)
li $t2, 1
seq $t0, $t1, $t2
beq $0, $t0, label6
label6:
lw $t2, 0($sp)
li $t3, 1
sub $t1, $t2, $t3
move $a0, $t1
jal fib
lw $t3, 0($sp)
li $t4, 2
sub $t2, $t3, $t4
move $a0, $t2
jal fib
add $t0, $t1, $t2
jr $ra
add $sp, $sp, 4
jr $ra
