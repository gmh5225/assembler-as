.intel_syntax noprefix
.data

.text
.global _start
_start:
    mov rax, 6
    mov rbx, 15
    add rax, rbx
    mov rdi, rax
    
    mov rax, 60
    syscall
    
    
