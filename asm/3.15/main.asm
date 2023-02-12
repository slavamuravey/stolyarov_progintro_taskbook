%include "../include/stud_io.inc"
    global  _start

    section .data
opening_brackets_count:
    dd      0                                   ; define initial count of opening brackets
closing_brackets_count:
    dd      0                                   ; define initial count of closing brackets

_start:
main:
read_line_lp:
    GETCHAR                                     ; read char
    cmp     eax, -1
    je      read_line_lp_quit                   ; quit loop if EOF
    cmp     al, 10
    jne     if_end_line_else
    mov     eax, [opening_brackets_count]       ; check brackets balance
    cmp     eax, [closing_brackets_count]
    jne     if_brackets_balance_else
    PRINT   "YES"
    jmp     if_brackets_balance_quit
if_brackets_balance_else:
    PRINT   "NO"
if_brackets_balance_quit:
    PUTCHAR 10
    mov     [opening_brackets_count], dword 0   ; drop brackets counts
    mov     [closing_brackets_count], dword 0
    jmp     if_end_line_quit                    ; read next char
if_end_line_else:
    push    eax     
    mov     eax, [opening_brackets_count]       ; if closing brackets count greater than opening brackets count, skip following comparision
    cmp     eax, [closing_brackets_count]
    jl      if_cmp_brackets_quit
    pop     eax
    cmp     al, "("
    jne     if_opening_bracket_quit
    inc     dword [opening_brackets_count]      ; increment opening brackets count
if_opening_bracket_quit:
    cmp     al, ")"
    jne     if_closing_bracket_quit
    inc     dword [closing_brackets_count]      ; increment closing brackets count
if_closing_bracket_quit:
if_cmp_brackets_quit:
if_end_line_quit:
    jmp     read_line_lp                        ; read next char
read_line_lp_quit:    
quit:                                           ; finish the program
    FINISH