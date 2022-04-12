	.globl main
	.data
label599: .asciiz "2\n"
label604: .asciiz "\n"
label608: .asciiz "\n"
label612: .asciiz "\n"
label616: .asciiz "\n"
label620: .asciiz "\n"
label624: .asciiz "\n"
label628: .asciiz "\n"
label632: .asciiz "\n"
label636: .asciiz "\n"
label640: .asciiz "\n"
label644: .asciiz "\n"
label648: .asciiz "\n"
label652: .asciiz "\n"
label656: .asciiz "\n"
label660: .asciiz "\n"
label664: .asciiz "\n"
label668: .asciiz "\n"
label672: .asciiz "\n"
label676: .asciiz "\n"
label680: .asciiz "\n"
label684: .asciiz "\n"
label688: .asciiz "\n"
label692: .asciiz "\n"
label696: .asciiz "\n"
label700: .asciiz "\n"
label704: .asciiz "\n"
label708: .asciiz "\n"
label712: .asciiz "\n"
label716: .asciiz "\n"
label720: .asciiz "\n"
label724: .asciiz "\n"
label728: .asciiz "\n"
label732: .asciiz "\n"
label736: .asciiz "\n"
label740: .asciiz "\n"
label744: .asciiz "\n"
label748: .asciiz "\n"
label752: .asciiz "\n"
label756: .asciiz "\n"
label760: .asciiz "\n"
label764: .asciiz "\n"
label768: .asciiz "\n"
label772: .asciiz "\n"
label776: .asciiz "\n"
label780: .asciiz "\n"
label784: .asciiz "\n"
label788: .asciiz "\n"
label792: .asciiz "\n"
label796: .asciiz "\n"
label800: .asciiz "\n"
label804: .asciiz "\n"
label808: .asciiz "\n"
label812: .asciiz "\n"
label816: .asciiz "\n"
label820: .asciiz "\n"
label824: .asciiz "\n"
label828: .asciiz "\n"
label832: .asciiz "\n"
label836: .asciiz "\n"
label840: .asciiz "\n"
label844: .asciiz "\n"
label848: .asciiz "\n"
label852: .asciiz "\n"
label856: .asciiz "\n"
label860: .asciiz "\n"
label864: .asciiz "\n"
label868: .asciiz "\n"
label872: .asciiz "\n"
label876: .asciiz "\n"
label880: .asciiz "\n"
label884: .asciiz "\n"
label888: .asciiz "\n"
label892: .asciiz "\n"
label896: .asciiz "\n"
label900: .asciiz "\n"
label904: .asciiz "\n"
label908: .asciiz "\n"
label912: .asciiz "\n"
label916: .asciiz "\n"
label920: .asciiz "\n"
label924: .asciiz "\n"
label928: .asciiz "\n"
label932: .asciiz "\n"
label936: .asciiz "\n"
label940: .asciiz "\n"
label944: .asciiz "\n"
label948: .asciiz "\n"
label952: .asciiz "\n"
label956: .asciiz "\n"
label960: .asciiz "\n"
label964: .asciiz "\n"
label968: .asciiz "\n"
label972: .asciiz "\n"
label976: .asciiz "\n"
label980: .asciiz "\n"
label984: .asciiz "\n"
label988: .asciiz "\n"
label992: .asciiz "\n"
label1090: .asciiz " prime numbers <= "
label1091: .asciiz "\n"

	.text
main:
sub $sp, $sp, 424
li $t0, 1
sw $t0, 8($sp)
label0:
lw $t1, 8($sp)
li $t2, 100
sle $t0, $t1, $t2
beq $0, $t0, label1
lw $t1, 8($sp)
li $t2, 1
seq $t0, $t1, $t2
beq $0, $t0, label2
b label3
label2:
label3:
lw $t1, 8($sp)
li $t2, 1
add $t0, $t1, $t2
sw $t0, 8($sp)
b label0
label1:
li $t0, 2
sw $t0, 8($sp)
label300:
lw $t1, 8($sp)
li $t2, 10
sle $t0, $t1, $t2
beq $0, $t0, label301
lw $t1, 8($sp)
lw $t2, 8($sp)
add $t0, $t1, $t2
sw $t0, 4($sp)
label302:
lw $t1, 4($sp)
li $t2, 100
sle $t0, $t1, $t2
beq $0, $t0, label303
lw $t1, 4($sp)
li $t2, 2
seq $t0, $t1, $t2
beq $0, $t0, label304
b label305
label304:
label305:
b label302
label303:
lw $t1, 8($sp)
li $t2, 1
add $t0, $t1, $t2
sw $t0, 8($sp)
b label300
label301:
li $t0, 1
sw $t0, 0($sp)
li $v0, 4
la $a0, label599
syscall
li $t0, 3
sw $t0, 8($sp)
label600:
lw $t1, 8($sp)
li $t2, 100
sle $t0, $t1, $t2
beq $0, $t0, label601
lw $t1, 8($sp)
li $t2, 3
seq $t0, $t1, $t2
beq $0, $t0, label602
b label605
label602:
label605:
lw $t1, 8($sp)
li $t2, 1
add $t0, $t1, $t2
sw $t0, 8($sp)
b label600
label601:
li $v0, 1
lw $a0, 0($sp)
syscall
li $v0, 4
la $a0, label1090
syscall
li $v0, 1
li $a0, 100
syscall
li $v0, 4
la $a0, label1091
syscall
end:
li $v0, 10
syscall
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
