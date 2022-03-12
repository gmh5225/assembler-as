.intel_syntax noprefix
.data

.text
.global _start
_start:
    mov di, 5
    and di, 10
    
    mov eax, 60
    syscall
done:
    ret
    
    
