.intel_syntax noprefix
.data

.text
.global _start
_start:
    mov ah, 5
    mov dh, 3
    and ah, dh
    mov al, ah
    mov dil, al
    
    mov eax, 60
    syscall
final:
    ret
    
