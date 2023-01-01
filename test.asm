.intel_syntax noprefix
.data
    msg1: .string "Hello!\n"

.text
.global _start
.extern main

_start:
    xor ebp, ebp
    mov edi, DWORD PTR [rsp+0]
    
    call main

    mov edi, eax
    mov eax, 60
    syscall
    
