.intel_syntax noprefix
.data

.text
.global _start
_start:
    xor rbp, rbp
    push rbp
    mov rbp, rsp
    
    mov al, 10
    mov [rbp-1], al
    mov di, [rbp-1]
    
    mov eax, 60
    syscall
final:
    ret
    
