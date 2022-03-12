.intel_syntax noprefix
.data

.text
.global _start
_start:
    mov r11b, 5
    mov r12b, 3
    add r11b, r12b
    mov dil, r11b
    
    mov eax, 60
    syscall
final:
    ret
    
