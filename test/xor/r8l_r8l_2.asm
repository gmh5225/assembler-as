.intel_syntax noprefix
.data

.text
.global _start
_start:
    mov dil, 7
    mov cl, 2
    xor dil, cl
    
    mov eax, 60
    syscall
final:
    ret
    
