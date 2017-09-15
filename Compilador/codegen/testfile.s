     .data
string_buffer:
     .space 128
sum: .word 0
     .text
     .globl main
main:
     li $t0, 4
     move $a0, $t0
     addi $sp, $sp, -4
     sw $ra, 0($sp)
     jal fnSum
     lw $ra, 0($sp)
     addi $sp, $sp, 4
     move $t0, $v0
     li $t1, 10
     ble $t0, $t1, L1
     li $t0, 1
     move $v0, $t0
     jr $ra
L1:
     li $t0, 0
     move $v0, $t0
     jr $ra
fnSum:
     li $t0, 1
     bne $a0, $t0, L2
     li $t0, 1
     move $v0, $t0
     jr $ra
L2:
     li $t0, 1
     sub $t0, $a0, $t0
     addi $sp, $sp, -4
     sw $a0, 0($sp)
     move $a0, $t0
     addi $sp, $sp, -4
     sw $ra, 0($sp)
     jal fnSum
     lw $ra, 0($sp)
     lw $a0, 4($sp)
     addi $sp, $sp, 8
     move $t0, $v0
     add $t0, $a0, $t0
     move $v0, $t0
     jr $ra
prints:
     la $a0, string_buffer
     li $v0, 4
     syscall
     jr $ra
