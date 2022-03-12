.intel_syntax noprefix
.data

.text
.extern test1
.global _start
_start:
    call test1
    
    mov eax, 60
    syscall
done:
    ret
    
    
