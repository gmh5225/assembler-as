.intel_syntax noprefix
.data

.text
.global _start
_start:
    mov edx, 5
    mov edi, 6
    add edi, edx
    
    mov eax, 60
    syscall
    
    
