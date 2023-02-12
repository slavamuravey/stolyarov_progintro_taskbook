%include "../include/stud_io.inc"
    global  _start

    section .text
_start:
main:
read_line_lp:
    GETCHAR
    cmp     eax, -1
    je      read_line_lp_quit
    cmp     al, 10
    jne     if_end_line_quit
    PRINT   "OK"
    PUTCHAR 10
if_end_line_quit:
    jmp     read_line_lp
read_line_lp_quit:
quit:                           ; finish the program
    FINISH