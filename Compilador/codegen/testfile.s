     .data
string_buffer:
     .space 128
a: .word 0
     .text
     .globl main
main:
     la $t0, a
     li $t1, 1
     li $t2, 2
     add $t1, $t1, $t2
     sw $t1, 0($t0)
     li $t0, 0
     move $v0, $t0
     jr $ra
prints:
     la $a0, string_buffer
     li $v0, 4
     syscall
     jr $ra
