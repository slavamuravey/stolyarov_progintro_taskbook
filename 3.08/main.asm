%include "../include/stud_io.inc"
    global  _start

    section .text
_start:
main:
    GETCHAR
    cmp     al, '0'             ; check the first character is a digit
    jl      if_digit_else
    cmp     al, '9'
    jg      if_digit_else
    lea     eax, [eax - 0x30]   ; if the first character is a digit, use lea instruction to calculate corresponding digit value
    push    eax                 ; save the value to the stack
    jmp     if_digit_quit
if_digit_else:
    push    0                   ; if the first number is not a digit, save zero as a value to the stack
if_digit_quit:    
read_remaining_chars:           ; read remaining characters to avoid printing them after program will quit
    cmp     eax, -1             ; quit if EOF
    je      print_result
    cmp     al, 10
    je      print_result
    GETCHAR
    jmp     read_remaining_chars
print_result:                   
    pop     ecx                 ; pop value from the stack, ecx register has count of stars now
    jecxz   lp_quit             ; if the value is zero, skip star printing loop
lp:
    PUTCHAR "*"                 ; print stars, using ecx register and loop construction
    loop    lp
    PUTCHAR 10
lp_quit:
quit:                           ; finish the program
    FINISH