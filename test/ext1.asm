.intel_syntax noprefix
.data

.text
.global _start
_start:
    mov r10d, 6
    mov r11d, 15
    add r10d, r11d
    mov edi, r10d
    
    mov eax, 60
    syscall
    
    
