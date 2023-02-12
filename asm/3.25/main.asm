%include "../include/stud_io.inc"
    global  _start

    section .data

    section .bss
number1:
    resd    1
number2:
    resd    1
number_string1:
    resb    NUMBER_STRING_LEN
number_string2:
    resb    NUMBER_STRING_LEN
result_string:
    resb    (NUMBER_STRING_LEN - 1) * 2 + 1

    section .text

; define constants
NUMBER_STRING_LEN: equ 11

ATOI_STATUS_CODE_OK: equ 0
ATOI_STATUS_CODE_ERROR_ZERO_LEN: equ 1
ATOI_STATUS_CODE_ERROR_NAN: equ 2

GET_NUMBER_REASON_EOF: equ -1
GET_NUMBER_REASON_ZERO_LEN: equ -2
GET_NUMBER_REASON_CHARS_OVERFLOW: equ -3

; >>> atoi

; input: string address = [ebp + 8], string length = [ebp + 12]
; output: result number = eax, status code = cl (0 - ok, 1 - zero length, 2 - string is NaN)
atoi:
    push    ebp
    mov     ebp, esp

    push    esi
    push    edi
    push    ebx

    mov     eax, [ebp + 8]
    mov     ecx, [ebp + 12]

    jecxz   .error_zero_len
    mov     esi, eax
    xor     edi, edi
.lp:
    xor     ebx, ebx
    mov     bl, [esi]
    cmp     bl, "0"                                 ; check the character is a digit
    jl      .if_digit_else
    cmp     bl, "9"
    jg      .if_digit_else
    sub     bl, "0"
    mov     eax, 10                                 ; multiply stored value to 10
    mul     edi
    add     eax, ebx                                ; add value of readed digit
    mov     edi, eax                                ; save the result
    jmp     .if_digit_quit
.if_digit_else:
    jmp     .error_nan
.if_digit_quit:
    inc     esi
    loop    .lp
    mov     cl, ATOI_STATUS_CODE_OK
    jmp     .quit
.error_zero_len:
    xor     eax, eax
    mov     cl, ATOI_STATUS_CODE_ERROR_ZERO_LEN
    jmp     .quit
.error_nan:
    xor     eax, eax
    mov     cl, ATOI_STATUS_CODE_ERROR_NAN
    jmp     .quit
.quit:
    pop    ebx
    pop    edi
    pop    esi

    mov     esp, ebp
    pop     ebp

    ret
; <<< atoi

; >>> itoa

; input: number value = [ebp + 8], result number string address = [ebp + 12]
itoa:
    push    ebp
    mov     ebp, esp

    push    ebx
    push    edi

    mov     eax, [ebp + 8]
    mov     ecx, [ebp + 12]

    mov     edi, NUMBER_STRING_LEN - 1
.clear_string_lp:
    mov     [ecx + edi], byte 0
    dec     edi
    jnz     .clear_string_lp
    xor     edi, edi
.read_digits_lp:
    xor     edx, edx                                
    mov     ebx, 10                                 
    div     ebx                                     
    add     edx, "0"
    push    edx   
    inc     edi                                  
    test    eax, eax            
    jnz     .read_digits_lp
.fill_string_lp:
    pop     eax
    mov     [ecx], al                               
    inc     ecx
    dec     edi
    jnz     .fill_string_lp
.quit:
    pop     edi
    pop     ebx

    mov     esp, ebp
    pop     ebp

    ret
; <<< itoa

; >>> print_bytes

; input: string address = [ebp + 8]
print_bytes:
    push    ebp
    mov     ebp, esp

    mov     eax, [ebp + 8]
.lp:
    mov     bl, [eax]
    test    bl, bl
    jz      .lp_quit
    PUTCHAR [eax]
    inc     eax
    jmp     .lp
.lp_quit:
.quit:
    mov     esp, ebp
    pop     ebp

    ret
; <<< print_bytes

; >>> get_number

; input: string address = [ebp + 8], bytes count = [ebp + 12]
; output: successfully readed digits count = eax, end read reason data = cl
get_number:
    push    ebp
    mov     ebp, esp

    push    ebx
    push    edi

    mov     edi, [ebp + 8]
    mov     ecx, [ebp + 12]

    mov     ebx, NUMBER_STRING_LEN - 1
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
; <<< get_number

; >>> read_remaining_chars

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
; <<< read_remaining_chars

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
