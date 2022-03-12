.intel_syntax noprefix
.data

.text
.global _start
_start:
    mov al, 5
    mov r12b, 3
    and r12b, al
    mov dil, r12b
    
    mov eax, 60
    syscall
final:
    ret
    
