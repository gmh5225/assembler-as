.intel_syntax noprefix
.data

.text
.global _start
_start:
    mov r12d, 5
    add r12d, 10
    and r12d, 10
    sub r12d, 10
    or r12d, 10
    xor r12d, 10
    mov edi, r12d
    
    mov eax, 60
    syscall
done:
    ret
    
    
