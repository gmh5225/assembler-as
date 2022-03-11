.intel_syntax noprefix
.data

.text
.global _start
_start:
    mov al, 5
    mov di, ax
    
    mov eax, 60
    syscall
done:
    ret
    
    
