.intel_syntax noprefix
.data

.text
.global _start
_start:
    mov r12w, 5
    mov di, r12w
    
    mov eax, 60
    syscall
done:
    ret
    
    
