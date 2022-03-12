.intel_syntax noprefix
.data

.text
.global _start
_start:
    mov r11b, 10
    mov r10b, r11b
    mov dil, r10b
    
    mov eax, 60
    syscall
final:
    ret
    
