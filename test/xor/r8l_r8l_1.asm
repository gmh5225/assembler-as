.intel_syntax noprefix
.data

.text
.global _start
_start:
    mov al, 7
    mov cl, 2
    xor al, cl
    mov dil, al
    
    mov eax, 60
    syscall
final:
    ret
    
