.intel_syntax noprefix
.data

.text
.global _start
_start:
    mov r15d, 20
    jmp part2
done1:
    mov edi, 10
    mov eax, 60
    syscall
done2:
    mov edi, 20
    mov eax, 60
    syscall
part2:
    mov r10d, 10
    mov r11d, 11
    mov r12d, 12
    jmp done2
final:
    ret
    
