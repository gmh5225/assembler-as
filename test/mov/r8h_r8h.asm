.intel_syntax noprefix
.data

.text
.global _start
_start:
    mov bh, 1
    mov ah, bh
    mov di, ax
    
    mov eax, 60
    syscall
final:
    ret
    
