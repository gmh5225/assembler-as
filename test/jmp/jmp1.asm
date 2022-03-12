.intel_syntax noprefix
.data

.text
.global _start
_start:
    mov r15d, 20
    jmp done2
done1:
    mov edi, 10
    mov eax, 60
    syscall
done2:
    mov edi, 20
    mov eax, 60
    syscall
final:
    ret
    
