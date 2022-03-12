.intel_syntax noprefix
.data

.text
.global _start
_start:
    xor rbp, rbp
    push rbp
    mov rbp, rsp
    
    mov DWORD PTR [rbp-4], 10
    mov ah, [rbp-4]
    mov bl, ah
    mov dil, bl
    
    mov eax, 60
    syscall
final:
    ret
    
