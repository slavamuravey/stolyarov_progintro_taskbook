%include "../include/stud_io.inc"
    global  _start

    section .text
_start:
main:
read_line_lp:
    GETCHAR
    cmp     eax, -1
    je      read_line_lp_quit   ; quit loop if EOF
    PUTCHAR al                  ; print char
    jmp     read_line_lp        ; read next char
read_line_lp_quit:
quit:                           ; finish the program
    FINISH