.intel_syntax noprefix
.data

.text
.global _start
_start:
    mov r15, 5
    or r15, 10
    mov rdi, r15
    
    mov eax, 60
    syscall
done:
    ret
    
    
