.intel_syntax noprefix
.data

.text
.global _start
_start:
    mov bh, 5
    or bh, 1
    mov bl, bh
    mov dil, bl
    
    mov eax, 60
    syscall
done:
    ret
    
    
