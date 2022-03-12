.intel_syntax noprefix
.data

.text
.global _start
_start:
    xor rbp, rbp
    push rbp
    mov rbp, rsp
    
    mov rax, 20
    mov [rbp-8], rax
    mov rdi, [rbp-8]
    
    mov eax, 60
    syscall
final:
    ret
    
