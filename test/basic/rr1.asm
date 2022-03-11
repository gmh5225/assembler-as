.intel_syntax noprefix
.data

.text
.global _start
_start:
    mov eax, 5
    mov ebx, eax
    mov eax, ebx
    mov ecx, eax
    mov edx, ecx
    mov esp, edx
    mov ebp, esp
    mov esi, ebp
    
    mov eax, 60
    mov edi, esi
    syscall
done:
    ret
    
