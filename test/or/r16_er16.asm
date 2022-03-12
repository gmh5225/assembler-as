.intel_syntax noprefix
.data

.text
.global _start
_start:
    mov dx, 8
    mov r11w, 12
    or dx, r11w
    mov di, dx
    
    mov eax, 60
    syscall
final:
    ret
    
