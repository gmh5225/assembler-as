.intel_syntax noprefix
.data

.text
.global _start
_start:
    mov r15, 5
    mov r11, 10
    or r15, r11
    mov rdi, r15
    
    mov eax, 60
    syscall
done:
    ret
    
    
