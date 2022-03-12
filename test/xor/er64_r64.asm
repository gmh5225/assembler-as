.intel_syntax noprefix
.data

.text
.global _start
_start:
    mov r15, 5
    mov rax, 10
    xor r15, rax
    mov rdi, r15
    
    mov eax, 60
    syscall
done:
    ret
    
    
