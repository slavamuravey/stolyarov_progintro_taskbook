%include "../include/stud_io.inc"
    global  _start

    section .data
minus_count:
    dd      0                   ; define initial minus count
plus_count:
    dd      0                   ; define initial plus count

    section .text
_start:
main:
read_line_lp:
    GETCHAR                     ; read char
    cmp     eax, -1             
    je      read_line_lp_quit   ; quit loop if EOF
    cmp     al, 10
    je      read_line_lp_quit
    cmp     al, "-"             
    je      inc_minus_count     ; if char is "-", jump to inc minus count
    cmp     al, "+"
    je      inc_plus_count      ; if char is "+", jump to inc plus count
    jmp     read_line_lp
inc_minus_count:
    inc     dword [minus_count] ; inc minus count
    jmp     plus_minus_quit
inc_plus_count:
    inc     dword [plus_count]  ; inc plus count
    jmp     plus_minus_quit
plus_minus_quit:
    jmp     read_line_lp        ; read next char
read_line_lp_quit:
    mov     eax, [minus_count]  ; multiply minus count to plus count
    mul     dword [plus_count]
    mov     ecx, eax
    jecxz   lp_quit             ; if the result is zero, skip star printing loop
lp:
    PUTCHAR "*"                 ; print stars, using ecx register and loop construction
    loop    lp
    PUTCHAR 10
lp_quit:
quit:                           ; finish the program
    FINISH