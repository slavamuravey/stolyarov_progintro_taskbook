%include "../include/stud_io.inc"
    global  _start

    section .text
_start:
main:
read_line_lp:
    GETCHAR
    cmp     eax, -1
    je      read_line_lp_quit       ; quit loop if EOF
    cmp     al, 10
    jne     if_eol_else
    jecxz   reverse_print_lp_quit
reverse_print_lp:
    pop     eax                     ; pop a char from the stack
    PUTCHAR al                      ; print the char
    loop    reverse_print_lp        
reverse_print_lp_quit:
    PUTCHAR 10
    xor     ecx, ecx                ; reset counter
    jmp     if_eol_quit
if_eol_else:
    push    eax                     ; push a char to the stack
    inc     ecx                     ; increment counter
if_eol_quit:
    jmp     read_line_lp            ; read next char
read_line_lp_quit:
quit:                               ; finish the program
    FINISH