	.globl main
	.data
label0: .asciiz "Enter an int now:"
label2: .asciiz ""

	.text
main:
sub $sp, $sp, 8
li $t0, 10
sw $t0, 0($sp)
jal reverse
end:
li $v0, 10
syscall
reverse:
li $v0, 4
la $a0, label0
syscall
li $v0, 5
syscall
move $t0, $v0
sw $t0, 0($sp)
lw $t1, 0($sp)
lw $t2, ($sp)
sne $t0, $t1, $t2
beq $0, $t0, label1
label1:
jr $ra
