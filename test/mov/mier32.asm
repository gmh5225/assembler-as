.intel_syntax noprefix
.data

.text
.global _start
_start:
    xor ebp, ebp
    push rbp
    mov rbp, rsp
    
    mov DWORD PTR [rbp-4], 10
    mov r12d, [rbp-4]
    mov edi, r12d
    
    mov eax, 60
    syscall
done:
    ret
    
    
