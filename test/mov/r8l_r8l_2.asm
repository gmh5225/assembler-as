.intel_syntax noprefix
.data

.text
.global _start
_start:
    mov bl, 10
    mov dil, bl
    
    mov eax, 60
    syscall
final:
    ret
    
