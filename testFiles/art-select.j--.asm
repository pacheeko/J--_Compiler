	.globl main
	.data
label1546: .asciiz " "
label1549: .asciiz " "
label1552: .asciiz " "
label1555: .asciiz " "
label1558: .asciiz " "
label1561: .asciiz " "
label1564: .asciiz " "
label1567: .asciiz " "
label1570: .asciiz " "
label1573: .asciiz " "
label1576: .asciiz " "
label1579: .asciiz " "
label1582: .asciiz " "
label1585: .asciiz " "
label1588: .asciiz " "
label1603: .asciiz "\n"

	.text
main:
sub $sp, $sp, 88
li $a0, 1jal INTERNALseed
li $t0, 0
sw $t0, 12($sp)
label0:
lw $t1, 12($sp)
li $t2, 14
sle $t0, $t1, $t2
beq $0, $t0, label1
lw $t1, 12($sp)
li $t2, 0
seq $t0, $t1, $t2
beq $0, $t0, label2
b label3
label2:
label3:
lw $t1, 12($sp)
li $t2, 1
add $t0, $t1, $t2
sw $t0, 12($sp)
b label0
label1:
jal printarray
li $t0, 0
sw $t0, 12($sp)
label45:
lw $t1, 12($sp)
li $t2, 13
sle $t0, $t1, $t2
beq $0, $t0, label46
lw $t0, 12($sp)
sw $t0, 0($sp)
lw $t1, 12($sp)
li $t2, 1
add $t0, $t1, $t2
sw $t0, 8($sp)
label47:
lw $t1, 8($sp)
li $t2, 14
sle $t0, $t1, $t2
beq $0, $t0, label48
lw $t1, 8($sp)
li $t2, 1
seq $t0, $t1, $t2
beq $0, $t0, label49
b label108
label49:
label108:
lw $t1, 8($sp)
li $t2, 1
add $t0, $t1, $t2
sw $t0, 8($sp)
b label47
label48:
lw $t1, 12($sp)
li $t2, 0
seq $t0, $t1, $t2
beq $0, $t0, label901
b label945
label901:
label945:
jal printarray
lw $t1, 12($sp)
li $t2, 1
add $t0, $t1, $t2
sw $t0, 12($sp)
b label45
label46:
end:
li $v0, 10
syscall
printarray:
li $t0, 0
sw $t0, 16($sp)
label1543:
lw $t1, 16($sp)
li $t2, 14
sle $t0, $t1, $t2
beq $0, $t0, label1544
lw $t1, 16($sp)
li $t2, 0
seq $t0, $t1, $t2
beq $0, $t0, label1545
b label1547
label1545:
label1547:
lw $t1, 16($sp)
li $t2, 1
add $t0, $t1, $t2
sw $t0, 16($sp)
b label1543
label1544:
li $v0, 4
la $a0, label1603
syscall
jr $ra
INTERNALseed:
sub $sp, $sp, 4
sw $a0, 0($sp)
lw $t0, 0($sp)
sw $t0, 4($sp)
add $sp, $sp, 4
jr $ra
INTERNALrandom:
li $t3, 17
lw $t4, 0($sp)
mul $t2, $t3, $t4
li $t3, 13
add $t1, $t2, $t3
li $t2, 32768
rem $t0, $t1, $t2
sw $t0, 0($sp)
lw $v0, 0($sp)
jr $ra
jr $ra
