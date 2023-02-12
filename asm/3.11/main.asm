%include "../include/stud_io.inc"
    global  _start

    section .data
digits_sum:
    dd      0                   ; define initial sum of digits

    section .text
_start:
main:
read_line_lp:
    GETCHAR                     ; read char
    cmp     eax, -1             
    je      read_line_lp_quit   ; quit loop if EOF
    cmp     al, 10
    je      read_line_lp_quit
    cmp     al, '0'             ; check the character is a digit
    jl      if_digit_quit
    cmp     al, '9'
    jg      if_digit_quit
    lea     eax, [eax - 0x30]   ; if the character is a digit, use lea instruction to calculate corresponding digit value
    add     [digits_sum], eax
if_digit_quit:
    jmp     read_line_lp        ; read next char
read_line_lp_quit:
    mov     ecx, [digits_sum]
    jecxz   lp_quit             ; if the result is zero, skip star printing loop
lp:
    PUTCHAR "*"                 ; print stars, using ecx register and loop construction
    loop    lp
    PUTCHAR 10
lp_quit:
quit:                           ; finish the program
    FINISH