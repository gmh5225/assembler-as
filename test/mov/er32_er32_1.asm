.intel_syntax noprefix
.data

.text
.global _start
_start:
    mov ebx, 5
    mov r15d, ebx
    mov edi, r15d
    
    mov eax, 60
    syscall
done:
    ret
    
    
