.intel_syntax noprefix
.data

.text
.global _start
_start:
    xor rbp, rbp
    push rbp
    mov rbp, rsp
    
    mov r12w, 20
    mov [rbp-2], r12w
    mov di, [rbp-2]
    
    mov eax, 60
    syscall
final:
    ret
    
