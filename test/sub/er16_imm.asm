.intel_syntax noprefix
.data

.text
.global _start
_start:
    mov r12w, 5
    sub r12w, 10
    mov di, r12w
    
    mov eax, 60
    syscall
done:
    ret
    
    
