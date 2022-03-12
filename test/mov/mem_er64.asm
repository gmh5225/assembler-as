.intel_syntax noprefix
.data

.text
.global _start
_start:
    xor rbp, rbp
    push rbp
    mov rbp, rsp
    
    mov r11, 20
    mov [rbp-8], r11
    mov rdi, [rbp-8]
    
    mov eax, 60
    syscall
final:
    ret
    
