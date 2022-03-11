.intel_syntax noprefix
.data

.text
.global _start
_start:
    mov ax, 5
    mov r12w, ax
    mov di, r12w
    
    mov eax, 60
    syscall
done:
    ret
    
    
