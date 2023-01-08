%include "../include/stud_io.inc"
    global  _start

    section .text
_start:
main:
read_line_lp:
    GETCHAR                     ; read char
    cmp     eax, -1
    je      read_line_lp_quit   ; quit loop if EOF
    cmp     al, 10
    jne     if_end_line_quit
    jecxz   lp_quit             ; if the result is zero, skip star printing loop
lp:
    PUTCHAR "*"                 ; print stars, using ecx register and loop construction
    loop    lp
    PUTCHAR 10
lp_quit:
    jmp     read_line_lp        ; read next char
if_end_line_quit:
    inc     ecx                 ; increase count of chars in line
    jmp     read_line_lp        ; read next char
read_line_lp_quit:
quit:                           ; finish the program
    FINISH