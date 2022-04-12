	.globl main
	.data
label0: .asciiz "fib(0) = 0\n"
label1: .asciiz "fib(1) = 1\n"
label4: .asciiz "fib("
label5: .asciiz ") = "
label6: .asciiz "\n"

	.text
main:
sub $sp, $sp, 16
li $t0, 2
sw $t0, 12($sp)
li $t0, 1
sw $t0, 4($sp)
li $t0, 0
sw $t0, 0($sp)
li $v0, 4
la $a0, label0
syscall
li $v0, 4
la $a0, label1
syscall
label2:
lw $t1, 12($sp)
li $t2, 47
slt $t0, $t1, $t2
beq $0, $t0, label3
li $v0, 4
la $a0, label4
syscall
li $v0, 1
lw $a0, 12($sp)
syscall
li $v0, 4
la $a0, label5
syscall
lw $t1, 4($sp)
lw $t2, 0($sp)
add $t0, $t1, $t2
sw $t0, 8($sp)
li $v0, 1
lw $a0, 8($sp)
syscall
lw $t0, 4($sp)
sw $t0, 0($sp)
lw $t0, 8($sp)
sw $t0, 4($sp)
li $v0, 4
la $a0, label6
syscall
lw $t1, 12($sp)
li $t2, 1
add $t0, $t1, $t2
sw $t0, 12($sp)
b label2
label3:
end:
li $v0, 10
syscall
