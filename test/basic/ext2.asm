.intel_syntax noprefix
.data

.text
.global _start
_start:
    mov r12, 6
    mov r15, 15
    add r12, r15
    mov rdi, r12
    
    mov rax, 60
    syscall
    
    
