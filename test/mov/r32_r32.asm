.intel_syntax noprefix
.data

.text
.global _start
_start:
    mov ebx, 5
    mov edi, ebx
    
    mov eax, 60
    syscall
done:
    ret
    
    
