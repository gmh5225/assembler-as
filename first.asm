.intel_syntax noprefix
.data

.text
.global _start
_start:
    mov ecx, 11
    mov eax, 60
    mov edi, ecx
    syscall
done:
    ret
    
