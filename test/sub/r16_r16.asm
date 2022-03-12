.intel_syntax noprefix
.data

.text
.global _start
_start:
    mov ax, 5
    mov bx, 10
    sub ax, bx
    mov di, ax
    
    mov eax, 60
    syscall
final:
    ret
    
