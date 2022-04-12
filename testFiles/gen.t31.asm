	.globl main
	.data

	.text
main:
sub $sp, $sp, 8
li $t0, 12
sw $t0, 0($sp)
jal foo
lw $t1, 0($sp)
lw $t2, ($sp)
li $t3, 2
sub $t1, $t2, $t3
div $t0, $t1, $t2
move $a0, $v0
li $v0, 1

syscall
end:
li $v0, 10
syscall
foo:
li $t0, 2
sw $t0, 0($sp)
jr $ra
