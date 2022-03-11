.intel_syntax noprefix
.data

.text
.global _start
_start:
    mov r9w, 5
    mov r12w, r9w
    mov di, r12w
    
    mov eax, 60
    syscall
done:
    ret
    
    
