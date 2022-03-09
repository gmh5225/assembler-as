.intel_syntax noprefix
.data

.text
.global _start
_start:
    mov DWORD PTR [rbp-4], 10
    mov edi, [rbp-4]
    syscall
    
    
