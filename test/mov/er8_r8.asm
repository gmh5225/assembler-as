.intel_syntax noprefix
.data

.text
.global _start
_start:
    mov al, 12
    mov r11b, al
    mov dil, r11b
    
    mov eax, 60
    syscall
final:
    ret
    
