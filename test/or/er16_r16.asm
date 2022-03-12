.intel_syntax noprefix
.data

.text
.global _start
_start:
    mov dx, 8
    mov r11w, 12
    or r11w, dx
    mov di, r11w
    
    mov eax, 60
    syscall
final:
    ret
    
