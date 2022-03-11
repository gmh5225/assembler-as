.intel_syntax noprefix
.data

.text
.global _start
_start:
    mov r12d, 15
    mov r10d, 10
    or r10d, r12d
    mov edi, r10d
    
    mov eax, 60
    syscall
done:
    ret
    
    
