.intel_syntax noprefix
.data

.text
.global _start
_start:
    mov bl, 5
    xor bl, 1
    mov dil, bl
    
    mov eax, 60
    syscall
done:
    ret
    
    