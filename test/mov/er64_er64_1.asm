.intel_syntax noprefix
.data

.text
.global _start
_start:
    mov rbx, 45
    mov r15, rbx
    mov rdi, r15
    
    mov rax, 60
    syscall
done:
    ret
    
    
