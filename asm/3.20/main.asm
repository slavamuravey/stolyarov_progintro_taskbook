%include "../include/stud_io.inc"
    global  _start

    section .data
number_detected:
    db      FALSE                                   ; define initial word detected flag

    section .bss
numbers:
    resd    NUMBERS_COUNT
digits:
    resb    DIGITS_COUNT

    section .text

; define constants
FALSE: equ 0
TRUE: equ 0xff
NUMBERS_COUNT: equ 2
DIGITS_COUNT: equ 10

; digits array address = esi
print_number: 
    mov     ecx, DIGITS_COUNT
.lp:          
    PUTCHAR [esi + ecx - 1]                         ; print digit characters from array of digits
    loop    .lp
    ret

; numbers array address = esi, numbers count = ecx, operation = ebx, result = eax
process_numbers:
    mov     eax, [esi + (NUMBERS_COUNT - 1) * 4]
    dec     ecx
    jecxz   .lp_quit
.lp:
    lea     edi, [esi + (ecx - 1) * 4]
    call    ebx
    loop    .lp
.lp_quit:
    ret

add:
    add     eax, [edi]
    ret
sub:
    sub     eax, [edi]
    ret
mul:
    mul     dword [edi]
    ret

; number value = ebx, result = [esi]
itoa:
    mov     ecx, DIGITS_COUNT
.clear_lp:
    mov     [esi + ecx - 1], byte 0
    loop    .clear_lp
    xor     edi, edi                                ; define initial index value to fill array of digits
.lp:
    xor     edx, edx                                ; drop edx after div
    mov     eax, ebx                                ; set dividend to eax as count of charactes stored in ebx
    mov     ecx, 10                                 ; div count of characters to 10 to get remainder of the division in edx
    div     ecx                                     ; execute division
    mov     ebx, eax                                ; update ebx with quotient
    lea     eax, [edx + '0']                        ; calculate char code
    mov     [esi + edi], al                         ; set char code to the array of digits
    inc     edi                                     ; increment array index
    test    ebx, ebx            
    jnz     .lp                                     ; break loop if digits are over
    ret

_start:
main:
    mov     ecx, NUMBERS_COUNT
    test    ecx, ecx                                ; jecxz can't be used because short jump is out of range
    jz      fill_numbers_lp_quit
fill_numbers_lp:
    xor     ebx, ebx
.read_char_lp:
    GETCHAR
    cmp     al, '0'                                 ; check the character is a digit
    jl      .if_digit_else
    cmp     al, '9'
    jg      .if_digit_else
    lea     esi, [eax - '0']                        ; if the first character is a digit, use lea instruction to calculate corresponding digit value
    mov     [number_detected], byte TRUE            ; set flag to true
    mov     eax, 10                                 ; multiply stored value to 10
    mul     ebx
    add     eax, esi                                ; add value of readed digit
    mov     ebx, eax                                ; save the result
    jmp     .if_digit_quit
.if_digit_else:
    cmp     al, " "
    je      .if_number_end
    cmp     al, 10
    je      .if_number_end
    jmp     fill_numbers_lp_quit
.if_number_end:
    push    eax
    mov     ah, [number_detected]                   ; if we met a number
    cmp     ah, TRUE
    jne     .read_char_lp
    mov     [number_detected], byte FALSE           ; if word ends, set word detected flag to false
    mov     [numbers + (ecx - 1) * 4], ebx
    pop     eax
    cmp     al, 10
    jne     .if_eol_else
    dec     ecx
    jmp     fill_numbers_lp_quit
.if_eol_else:
    loop    fill_numbers_lp
.if_number_end_else:
    jmp     fill_numbers_lp_quit
.if_digit_quit:
    jmp     .read_char_lp                           ; read next char
.read_char_lp_quit:
fill_numbers_lp_quit:

read_remaining_chars_lp:                            ; read remaining characters to avoid printing them after program will quit
    cmp     eax, -1                                 ; quit if EOF
    je      read_remaining_chars_lp_quit
    cmp     al, 10                                  ; quit if EOL
    je      read_remaining_chars_lp_quit
    GETCHAR
    jmp     read_remaining_chars_lp
read_remaining_chars_lp_quit:

    jecxz   if_error_else
    PRINT   "error"
    PUTCHAR 10
    jmp     if_error_quit
if_error_else:

    PRINT   "Sum: "
    mov     esi, numbers
    mov     ecx, NUMBERS_COUNT
    mov     ebx, add
    call    process_numbers
    mov     ebx, eax
    mov     esi, digits
    call    itoa
    call    print_number
    PUTCHAR 10

    PRINT   "Diff: "
    mov     esi, numbers
    mov     ecx, NUMBERS_COUNT
    mov     ebx, sub
    call    process_numbers
    mov     ebx, eax
    mov     esi, digits
    call    itoa
    call    print_number
    PUTCHAR 10

    PRINT   "Mul: "
    mov     esi, numbers
    mov     ecx, NUMBERS_COUNT
    mov     ebx, mul
    call    process_numbers
    mov     ebx, eax
    mov     esi, digits
    call    itoa
    call    print_number
    PUTCHAR 10
    
if_error_quit:
quit:                                           ; finish the program
    FINISH