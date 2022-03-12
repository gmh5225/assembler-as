.intel_syntax noprefix
.data

.text
.global _start
_start:
    mov r12b, 5
    add r12b, 1
    mov dil, r12b
    
    mov eax, 60
    syscall
done:
    ret
    
    
