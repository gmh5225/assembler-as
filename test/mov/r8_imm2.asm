.intel_syntax noprefix
.data

.text
.global _start
_start:
    mov dil, 5
    
    mov eax, 60
    syscall
done:
    ret
    
    
