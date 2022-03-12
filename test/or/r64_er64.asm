.intel_syntax noprefix
.data

.text
.global _start
_start:
    mov r15, 5
    mov rdi, 10
    or rdi, r15
    
    mov eax, 60
    syscall
done:
    ret
    
    
