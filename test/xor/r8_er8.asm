.intel_syntax noprefix
.data

.text
.global _start
_start:
    mov al, 5
    mov r12b, 3
    xor al, r12b
    mov dil, al
    
    mov eax, 60
    syscall
final:
    ret
    
