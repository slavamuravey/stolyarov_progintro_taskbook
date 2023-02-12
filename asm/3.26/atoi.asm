%include "../include/stud_io.inc"
    global  ATOI_STATUS_CODE_OK
    global  ATOI_STATUS_CODE_ERROR_ZERO_LEN
    global  ATOI_STATUS_CODE_ERROR_NAN
    global  atoi

    section .text

ATOI_STATUS_CODE_OK: equ 0
ATOI_STATUS_CODE_ERROR_ZERO_LEN: equ 1
ATOI_STATUS_CODE_ERROR_NAN: equ 2

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
