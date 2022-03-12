.intel_syntax noprefix
.data

.text
.global _start
_start:
    mov r12d, 5
    sub r12d, 10
    mov edi, r12d
    
    mov eax, 60
    syscall
done:
    ret
    
    
