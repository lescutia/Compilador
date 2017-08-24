     .data
string_buffer:
     .space 128
n: .word 0
i: .word 0
     .text
     .globl main
main:
     la $t0, i
     li $t1, 1
     sw $t1, 0($t0)
     la $t0, n
     li $t1, 15
     sw $t1, 0($t0)
     la $t0, string_buffer
     li $t1, 1835627088
     sw $t1, 0($t0)
     la $t0, string_buffer
     li $t1, 1936683621
     sw $t1, 4($t0)
     la $t0, string_buffer
     li $t1, 32
     sw $t1, 8($t0)
     addi $sp, $sp, -4
     sw $ra, 0($sp)
     la $t0, string_buffer
     sw $zero, 12($t0)
     jal prints
     lw $ra, 0($sp)
     addi $sp, $sp, 4
     lw $t0, n
     li $t1, 1
     sub $t0, $t0, $t1
     move $a0, $t0
     li $v0, 1
     syscall
     la $t1, string_buffer
     li $t2, 1836412448
     sw $t2, 0($t1)
     la $t1, string_buffer
     li $t2, 1936683621
     sw $t2, 4($t1)
     la $t1, string_buffer
     li $t2, 543515680
     sw $t2, 8($t1)
     la $t1, string_buffer
     li $t2, 1931501932
     sw $t2, 12($t1)
     la $t1, string_buffer
     li $t2, 1936024437
     sw $t2, 16($t1)
     la $t1, string_buffer
     li $t2, 544108393
     sw $t2, 20($t1)
     la $t1, string_buffer
     li $t2, 1713399140
     sw $t2, 24($t1)
     la $t1, string_buffer
     li $t2, 1852793449
     sw $t2, 28($t1)
     la $t1, string_buffer
     li $t2, 1768121185
     sw $t2, 32($t1)
     la $t1, string_buffer
     li $t2, 58
     sw $t2, 36($t1)
     addi $sp, $sp, -4
     sw $ra, 0($sp)
     la $t1, string_buffer
     sw $zero, 40($t1)
     jal prints
     lw $ra, 0($sp)
     addi $sp, $sp, 4
     li $a0, '\n'
     li $v0, 11
     syscall
L1:
     lw $t1, i
     lw $t2, n
     bge $t1, $t2, L2
     lw $t1, i
     move $a0, $t1
     addi $sp, $sp, -8
     sw $ra, 0($sp)
     sw $t0, 4($sp)
     jal fnFibonacci
     lw $ra, 0($sp)
     lw $t0, 4($sp)
     addi $sp, $sp, 8
     move $t1, $v0
     move $a0, $t1
     li $v0, 1
     syscall
     la $t2, i
     lw $t3, i
     li $t4, 1
     add $t3, $t3, $t4
     sw $t3, 0($t2)
     lw $t2, i
     lw $t3, n
     bge $t2, $t3, L3
     la $t2, string_buffer
     li $t3, 8236
     sw $t3, 0($t2)
     addi $sp, $sp, -4
     sw $ra, 0($sp)
     la $t2, string_buffer
     sw $zero, 4($t2)
     jal prints
     lw $ra, 0($sp)
     addi $sp, $sp, 4
L3:
     j L1
L2:
     li $t2, 0
     move $v0, $t2
     jr $ra
fnFibonacci:
     li $t2, 0
     bne $a0, $t2, L4
     li $t2, 0
     move $v0, $t2
     jr $ra
L4:
     li $t2, 1
     bne $a0, $t2, L5
     li $t2, 1
     move $v0, $t2
     jr $ra
L5:
     li $t2, 1
     sub $t2, $a0, $t2
     addi $sp, $sp, -4
     sw $a0, 0($sp)
     move $a0, $t2
     addi $sp, $sp, -12
     sw $ra, 0($sp)
     sw $t0, 4($sp)
     sw $t1, 8($sp)
     jal fnFibonacci
     lw $ra, 0($sp)
     lw $t0, 4($sp)
     lw $t1, 8($sp)
     lw $a0, 12($sp)
     addi $sp, $sp, 16
     move $t2, $v0
     li $t3, 2
     sub $t3, $a0, $t3
     addi $sp, $sp, -4
     sw $a0, 0($sp)
     move $a0, $t3
     addi $sp, $sp, -16
     sw $ra, 0($sp)
     sw $t0, 4($sp)
     sw $t1, 8($sp)
     sw $t2, 12($sp)
     jal fnFibonacci
     lw $ra, 0($sp)
     lw $t0, 4($sp)
     lw $t1, 8($sp)
     lw $t2, 12($sp)
     lw $a0, 16($sp)
     addi $sp, $sp, 20
     move $t3, $v0
     add $t2, $t2, $t3
     move $v0, $t2
     jr $ra
prints:
     la $a0, string_buffer
     li $v0, 4
     syscall
     jr $ra
