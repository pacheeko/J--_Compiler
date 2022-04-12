	.globl main
	.data
label0: .asciiz "Hello, world!\n"

	.text
main:
sub $sp, $sp, 0
li $v0, 4
la $a0, label0
syscall
end:
li $v0, 10
syscall
