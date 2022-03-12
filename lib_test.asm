.intel_syntax noprefix
.data
    STR1: .string "Hello!\n"
    
.text
.global test1

test1:
    mov rax, 1
    mov rdi, 1
    mov rsi, OFFSET FLAT:STR1
    mov rdx, 7
    syscall
    ret
