.intel_syntax noprefix
.data

.text
.global _start
_start:
    mov edi, 15
    mov ebx, 10
    and edi, ebx
    
    mov eax, 60
    syscall
done:
    ret
    
    
