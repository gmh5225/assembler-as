.intel_syntax noprefix
.data

.text
.global _start
_start:
    mov r9w, 5
    mov di, r9w
    
    mov eax, 60
    syscall
done:
    ret
    
    
