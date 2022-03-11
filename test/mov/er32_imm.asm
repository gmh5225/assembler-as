.intel_syntax noprefix
.data

.text
.global _start
_start:
    mov r14d, 5
    mov edi, r14d
    
    mov eax, 60
    syscall
done:
    ret
    
    
