.intel_syntax noprefix
.data

.text
.global _start
_start:
    mov rdi, 45
    
    mov rax, 60
    syscall
done:
    ret
    
    
