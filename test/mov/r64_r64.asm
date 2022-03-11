.intel_syntax noprefix
.data

.text
.global _start
_start:
    mov rbx, 45
    mov rdi, rbx
    
    mov rax, 60
    syscall
done:
    ret
    
    
