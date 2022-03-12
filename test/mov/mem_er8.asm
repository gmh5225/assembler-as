.intel_syntax noprefix
.data

.text
.global _start
_start:
    xor rbp, rbp
    push rbp
    mov rbp, rsp
    
    mov r9b, 10
    mov [rbp-1], r9b
    mov di, [rbp-1]
    
    mov eax, 60
    syscall
final:
    ret
    
