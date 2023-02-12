%include "../include/stud_io.inc"
    global  GET_NUMBER_REASON_EOF
    global  GET_NUMBER_REASON_ZERO_LEN
    global  GET_NUMBER_REASON_CHARS_OVERFLOW
    global  get_number

    section .text

GET_NUMBER_REASON_EOF: equ -1
GET_NUMBER_REASON_ZERO_LEN: equ -2
GET_NUMBER_REASON_CHARS_OVERFLOW: equ -3

; input: string address = [ebp + 8], bytes count = [ebp + 12]
; output: successfully readed digits count = eax, end read reason data = cl
get_number:
    push    ebp
    mov     ebp, esp

    push    ebx
    push    edi

    mov     edi, [ebp + 8]
    mov     ecx, [ebp + 12]

    mov     ebx, 10
.clear_string_lp:
    mov     [edi + ebx], byte 0
    dec     ebx
    jnz     .clear_string_lp
    xor     ebx, ebx
    jecxz   .zero_len_quit
.lp:    
    GETCHAR
    cmp     eax, -1
    je      .eof_quit
    cmp     al, "0"                                 ; check the character is a digit
    jl      .if_digit_else
    cmp     al, "9"
    jg      .if_digit_else
    mov     [edi + ebx], al
    inc     ebx
    jmp     .if_digit_quit
.if_digit_else:
    jmp     .nan_quit
.if_digit_quit:
    cmp     bl, cl
    jg      .chars_overflow_quit
    jmp     .lp
.eof_quit:
    mov     eax, ebx
    mov     cl, GET_NUMBER_REASON_EOF
    jmp     .quit
.nan_quit:
    mov     cl, al
    mov     eax, ebx
    jmp     .quit
.zero_len_quit:
    mov     eax, ebx
    mov     cl, GET_NUMBER_REASON_ZERO_LEN
    jmp     .quit
.chars_overflow_quit:
    mov     eax, ebx
    mov     cl, GET_NUMBER_REASON_CHARS_OVERFLOW
    jmp     .quit
.quit:
    pop     edi
    pop     ebx

    mov     esp, ebp
    pop     ebp

    ret
