.intel_syntax noprefix
.data

.text
.global _start
_start:
    mov r11b, 10
    mov dil, r11b
    
    mov eax, 60
    syscall
final:
    ret
    
