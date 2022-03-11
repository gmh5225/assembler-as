.intel_syntax noprefix
.data

.text
.global _start
_start:
    mov ebx, 5
    mov r15d, ebx
    mov r14d, r15d
    mov edi, r14d
    
    mov eax, 60
    syscall
done:
    ret
    
    
