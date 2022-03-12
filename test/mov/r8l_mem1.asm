.intel_syntax noprefix
.data

.text
.global _start
_start:
    xor rbp, rbp
    push rbp
    mov rbp, rsp
    
    mov DWORD PTR [rbp-4], 10
    mov dil, [rbp-4]
    
    mov eax, 60
    syscall
final:
    ret
    
