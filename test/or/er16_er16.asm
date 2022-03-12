.intel_syntax noprefix
.data

.text
.global _start
_start:
    mov r10w, 8
    mov r11w, 12
    or r10w, r11w
    mov di, r10w
    
    mov eax, 60
    syscall
final:
    ret
    
