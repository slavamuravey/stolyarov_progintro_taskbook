%include "../include/stud_io.inc"
    extern  atoi
    extern  itoa
    extern  get_number
    extern  GET_NUMBER_REASON_EOF
    extern  GET_NUMBER_REASON_ZERO_LEN
    extern  GET_NUMBER_REASON_CHARS_OVERFLOW
    extern  print_bytes
    global  _start

    section .bss
number1:
    resd    1
number2:
    resd    1
number_string1:
    resb    11
number_string2:
    resb    11
result_string:
    resb    11

    section .text

; read remaining characters to avoid printing them after program will quit
; input: last read char = [ebp + 8]
read_remaining_chars:
    push    ebp
    mov     ebp, esp

    mov     eax, [ebp + 8]
.lp:                          
    cmp     eax, -1                                 ; quit if EOF
    je      .lp_quit
    cmp     al, 10                                  ; quit if EOL
    je      .lp_quit
    GETCHAR
    jmp     .lp
.lp_quit:
.quit:

    mov     esp, ebp
    pop     ebp

    ret

_start:
main:
read_number_1:
    push    dword 10
    push    dword number_string1
    call    get_number
    add     esp, 8

    cmp     cl, GET_NUMBER_REASON_EOF
    je      print_eof_error
    cmp     cl, GET_NUMBER_REASON_ZERO_LEN
    je      print_zero_len_error
    cmp     cl, GET_NUMBER_REASON_CHARS_OVERFLOW
    je      print_chars_overflow_error
    
    test    al, al
    jnz     .if_not_empty_string_quit
    push    eax
    call    read_remaining_chars
    add     esp, 4
    jmp     print_parse_error
.if_not_empty_string_quit:

    cmp     cl, " "
    jne     .if_number_end_else
    jmp     .if_number_end_quit
.if_number_end_else:
    push    eax
    call    read_remaining_chars
    add     esp, 4

    jmp     print_parse_error
.if_number_end_quit:

    push    eax
    push    dword number_string1
    call    atoi
    add     esp, 8

    mov     [number1], eax

read_number_2:
    push    dword 10
    push    dword number_string2
    call    get_number
    add     esp, 8

    cmp     cl, GET_NUMBER_REASON_ZERO_LEN
    je      print_zero_len_error
    cmp     cl, GET_NUMBER_REASON_CHARS_OVERFLOW
    je      print_chars_overflow_error
    
    test    al, al
    jnz     .if_not_empty_string_quit
    push    eax
    call    read_remaining_chars
    add     esp, 4
    jmp     print_parse_error
.if_not_empty_string_quit:

    cmp     cl, " "
    jne     .if_number_end_else1

    push    eax
    call    read_remaining_chars
    add     esp, 4

    jmp     .if_number_end_quit
.if_number_end_else1:
    cmp     cl, GET_NUMBER_REASON_EOF
    jne     .if_number_end_else2
    jmp     .if_number_end_quit
.if_number_end_else2:
    cmp     cl, 10
    jne     .if_number_end_else3
    jmp     .if_number_end_quit
.if_number_end_else3:
    push    eax
    call    read_remaining_chars
    add     esp, 4

    jmp     print_parse_error
.if_number_end_quit:

    push    eax
    push    dword number_string2
    call    atoi
    add     esp, 8

    mov     [number2], eax

print_result:
    mov     ebx, [number2]
    mov     eax, [number1]
    add     eax, ebx

    push    dword result_string
    push    eax
    call    itoa
    add     esp, 8

    PRINT   "Sum: "

    push    dword result_string
    call    print_bytes
    add     esp, 4

    PUTCHAR 10

    mov     ebx, [number2]
    mov     eax, [number1]
    sub     eax, ebx
    
    push    dword result_string
    push    eax
    call    itoa
    add     esp, 8

    PRINT   "Diff: "

    push    dword result_string
    call    print_bytes
    add     esp, 4

    PUTCHAR 10

    mov     ebx, [number2]
    mov     eax, [number1]
    mul     ebx
    
    push    dword result_string
    push    eax
    call    itoa
    add     esp, 8

    PRINT   "Mul: "
    
    push    dword result_string
    call    print_bytes
    add     esp, 4
    
    PUTCHAR 10

    jmp     quit
print_eof_error:
    PRINT   "Not enough numbers"
    PUTCHAR 10
    jmp     quit
print_zero_len_error:
    PRINT   "Numbers should not be empty"
    PUTCHAR 10
    jmp     quit
print_chars_overflow_error:
    PRINT   "Chars overflow"
    PUTCHAR 10
    jmp     quit
print_parse_error:
    PRINT   "Parse numbers error"
    PUTCHAR 10
    jmp     quit
quit:                                           ; finish the program
    FINISH
