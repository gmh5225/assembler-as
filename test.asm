.intel_syntax noprefix
.data
    msg1: .string "Hello!\n"

.text
.global _start
.global x64syscall
.extern main

_start:
    xor ebp, ebp
    mov edi, DWORD PTR [rsp+0]
    lea rsi, [rsp+8]
    
    call main

    mov edi, eax
    mov eax, 60
    syscall
    
x64syscall:
    mov rax, rdi
    mov rdi, rsi
    mov rsi, rdx
    mov rdx, rcx
    mov r10, r8
    mov r8, r9
    xor r9, r9
    syscall
    ret
    
