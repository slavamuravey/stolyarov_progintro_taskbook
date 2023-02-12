%include "../include/stud_io.inc"
    global  _start

    section .text
print_stars_ecx:                                ; print stars; count of stars is stored in ecx register
    jecxz   .lp_quit                            ; if the result is zero, skip star printing loop
.lp:
    PUTCHAR "*"                                 ; print stars, using ecx register and loop construction
    loop    .lp
.lp_quit:
    ret

_start:
main:
    xor ebx, ebx
read_line_lp:
    GETCHAR
    cmp     al, '0'                             ; check the character is a digit
    jl      read_line_lp_quit
    cmp     al, '9'
    jg      read_line_lp_quit
    lea     esi, [eax - '0']                    ; if the first character is a digit, use lea instruction to calculate corresponding digit value
    mov     eax, 10                             ; multiply stored value to 10
    mul     ebx
    add     eax, esi                            ; add value of readed digit
    mov     ebx, eax                            ; save the result
    jmp     read_line_lp                        ; read next char
read_line_lp_quit:
read_remaining_chars_lp:                        ; read remaining characters to avoid printing them after program will quit
    cmp     eax, -1                             ; quit if EOF
    je      read_remaining_chars_lp_quit
    cmp     al, 10                              ; quit if EOL
    je      read_remaining_chars_lp_quit
    GETCHAR
    jmp     read_remaining_chars_lp
read_remaining_chars_lp_quit:
    mov     ecx, ebx                            ; print stars based on ebx register value
    call    print_stars_ecx
    PUTCHAR 10
quit:                                           ; finish the program
    FINISH