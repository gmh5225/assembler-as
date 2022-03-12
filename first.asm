.intel_syntax noprefix
.data
    STR1: .string "Hello!\n"
    STR2: .string "Hi!\n"

.text
.global _start
_start:
    mov rax, 1
    mov rdi, 1
    ;mov rsi, OFFSET FLAT:STR1
    mov rdx, 7
    syscall
    
    mov rax, 1
    mov rdi, 1
    ;mov rsi, OFFSET FLAT:STR2
    mov rdx, 4
    syscall
    
    mov eax, 60
    syscall
done:
    ret
    
    
