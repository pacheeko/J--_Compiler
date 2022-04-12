    .data
msg1: .asciiz "\nHello, World! My new friend\n"
msg2: .asciiz "\nAnother message\n"

    .text
main:
li $t0, 5
beq $t0, 3, label1

li $v0, 4
la $a0, msg2
syscall

label1:
li $v0, 4
la $a0, msg1
syscall
li $v0, 10
syscall