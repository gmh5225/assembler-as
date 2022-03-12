.intel_syntax noprefix
.data

.text
.global _start
_start:
    mov rdi, 5
    xor rdi, 10
    
    mov eax, 60
    syscall
done:
    ret
    
    
