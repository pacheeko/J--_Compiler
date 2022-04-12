	.globl main
	.data

	.text
main:
sub $sp, $sp, 0
li $t1, 1
neg $t0, $t1
move $a0, $t0
jal foo
end:
li $v0, 10
syscall
foo:
sub $sp, $sp, 4
sw $a0, 0($sp)
lw $t2, 0($sp)
lw $t3, 0($sp)
mul $t1, $t2, $t3
li $t2, 0
slt $t0, $t1, $t2
beq $0, $t0, label0
label0:
add $sp, $sp, 4
jr $ra
