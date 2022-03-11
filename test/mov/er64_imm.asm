.intel_syntax noprefix
.data

.text
.global _start
_start:
    mov r11, 45
    mov rdi, r11
    
    mov rax, 60
    syscall
done:
    ret
    
    
