.intel_syntax noprefix
.data

.text
.global _start
_start:
    mov r11b, 8
    mov dil, r11b
    
    mov eax, 60
    syscall
final:
    ret
    
