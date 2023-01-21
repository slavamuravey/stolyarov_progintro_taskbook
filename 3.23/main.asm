%include "../include/stud_io.inc"
    global  _start

    section .text
_start:
main:
read_line_lp:
    GETCHAR
    push    eax                     ; push a char to the stack
    inc     ecx                     ; increment counter
    cmp     eax, -1
    je      read_line_lp_quit       ; quit loop if EOF
    jmp     read_line_lp            ; read next char
read_line_lp_quit:
    test    ecx, ecx
    jz      lp_quit
    mov     esi, esp
lp:
    lea     ebx, [esi + (ecx - 1) * 4]
    mov     eax, [ebx]
    cmp     al, 10
    je      if_eol
    cmp     eax, -1
    je      if_eol
if_eol_else:
    push    eax
    jmp     if_eol_quit
if_eol:
    push    10
    cmp     esp, esi
    je      reverse_print_lp_quit
reverse_print_lp:
    pop     eax                     ; pop a char from the stack
    PUTCHAR al                      ; print the char
    cmp     esp, esi
    je      reverse_print_lp_quit
    jmp     reverse_print_lp        
reverse_print_lp_quit:
    jmp     if_eol_quit
if_eol_quit:
    loop    lp
lp_quit:
quit:                               ; finish the program
    FINISH