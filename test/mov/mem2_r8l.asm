.intel_syntax noprefix
.data

.text
.global _start
_start:
    xor rbp, rbp
    push rbp
    mov rbp, rsp
    
    mov dil, 10
    mov [rbp-1], dil
    mov di, [rbp-1]
    
    mov eax, 60
    syscall
final:
    ret
    
