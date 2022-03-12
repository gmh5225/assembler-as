.intel_syntax noprefix
.data

.text
.global _start
_start:
    xor rbp, rbp
    push rbp
    mov rbp, rsp
    
    mov eax, 10
    mov [rbp-4], eax
    mov edi, [rbp-4]
    
    mov eax, 60
    syscall
final:
    ret
    
