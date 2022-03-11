.intel_syntax noprefix
.data

.text
.global _start
_start:
    mov edi, 5
    
    mov eax, 60
    syscall
done:
    ret
    
