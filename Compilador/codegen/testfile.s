     .data
string_buffer:
     .space 128
iMin: .word 0
iMax: .word 0
iRemainder: .word 0
gcd: .word 0
     .text
     .globl main
main:
     la $t0, gcd
     li $t1, 4
     li $t2, 10
     move $a1, $t2
     move $a0, $t1
     addi $sp, $sp, -8
     sw $ra, 0($sp)
     sw $t0, 4($sp)
     jal fnGCD
     lw $ra, 0($sp)
     lw $t0, 4($sp)
     addi $sp, $sp, 8
     move $t1, $v0
     sw $t1, 0($t0)
     lw $t0, gcd
     move $a0, $t0
     li $v0, 1
     syscall
     li $t1, 0
     move $v0, $t1
     jr $ra
fnMax:
     ble $a0, $a1, L1
     move $v0, $a0
     jr $ra
L1:
     move $v0, $a1
     jr $ra
fnMin:
     bge $a0, $a1, L2
     move $v0, $a0
     jr $ra
L2:
     move $v0, $a1
     jr $ra
fnGCD:
     la $t1, iMax
     addi $sp, $sp, -8
     sw $a0, 0($sp)
     sw $a1, 4($sp)
     addi $sp, $sp, -12
     sw $ra, 0($sp)
     sw $t0, 4($sp)
     sw $t1, 8($sp)
     jal fnMax
     lw $ra, 0($sp)
     lw $t0, 4($sp)
     lw $t1, 8($sp)
     lw $a0, 12($sp)
     lw $a1, 16($sp)
     addi $sp, $sp, 20
     move $t2, $v0
     sw $t2, 0($t1)
     la $t1, iMin
     addi $sp, $sp, -8
     sw $a0, 0($sp)
     sw $a1, 4($sp)
     addi $sp, $sp, -12
     sw $ra, 0($sp)
     sw $t0, 4($sp)
     sw $t1, 8($sp)
     jal fnMin
     lw $ra, 0($sp)
     lw $t0, 4($sp)
     lw $t1, 8($sp)
     lw $a0, 12($sp)
     lw $a1, 16($sp)
     addi $sp, $sp, 20
     move $t2, $v0
     sw $t2, 0($t1)
     lw $t1, iMin
     li $t2, 0
     bne $t1, $t2, L3
     lw $t1, iMax
     move $v0, $t1
     jr $ra
L3:
     la $t1, iRemainder
     lw $t2, iMax
     lw $t3, iMin
     div $t2, $t3
     mfhi $t2
     sw $t2, 0($t1)
L4:
     lw $t1, iRemainder
     li $t2, 0
     beq $t1, $t2, L5
     la $t1, iMax
     lw $t2, iMin
     sw $t2, 0($t1)
     la $t1, iMin
     lw $t2, iRemainder
     sw $t2, 0($t1)
     la $t1, iRemainder
     lw $t2, iMax
     lw $t3, iMin
     div $t2, $t3
     mfhi $t2
     sw $t2, 0($t1)
     j L4
L5:
     lw $t1, iMin
     move $v0, $t1
     jr $ra
prints:
     la $a0, string_buffer
     li $v0, 4
     syscall
     jr $ra
