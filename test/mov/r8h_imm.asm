.intel_syntax noprefix
.data

.text
.global _start
_start:
    mov ah, 1
    mov di, ax
    
    mov eax, 60
    syscall
final:
    ret
    
