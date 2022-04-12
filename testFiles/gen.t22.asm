	.globl main
	.data
label0: .asciiz "asdf"
label1: .asciiz "\b\t\n\f\r\"\'\\"
label2: .asciiz ""
label4: .asciiz "
label5: .asciiz ""

	.text
main:
sub $sp, $sp, 0
li $v0, 4
la $a0, label0
syscall
li $v0, 4
la $a0, label1
syscall
li $v0, 4
la $a0, label2
syscall
li $t0, 1
beq $0, $t0, label3
label3:
li $v0, 4
la $a0, label5
syscall
end:
li $v0, 10
syscall
