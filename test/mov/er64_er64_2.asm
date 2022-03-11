.intel_syntax noprefix
.data

.text
.global _start
_start:
    mov rbx, 45
    mov r15, rbx
    mov r12, r15
    mov rdi, r12
    
    mov rax, 60
    syscall
done:
    ret
    
    
