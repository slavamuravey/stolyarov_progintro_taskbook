%include "../include/stud_io.inc"
    global  _start

    section .text
_start:
main:

; define constants
FALSE:      equ  0
TRUE:       equ  0xff

    GETCHAR                             ; get the first char
    cmp     al, "A"                 
    jne     first_char_capital_a_else   ; if the first char is "A"
    push    TRUE                        ; save "true" to the stack
    jmp     first_char_capital_a_quit
first_char_capital_a_else:            
    push    FALSE                       ; else save "false" to the stack
first_char_capital_a_quit:
read_remaining_chars:                   ; read remaining characters to avoid printing them after program will quit
    cmp     eax, -1                     ; quit if EOF
    je      print_result
    cmp     al, 10
    je      print_result
    GETCHAR
    jmp     read_remaining_chars
print_result:
    pop     eax                         ; pop result from the stack
    test    eax, eax
    jnz     print_yes                   ; if result is zero
    PRINT   "NO"                        ; print "NO"
    jmp     print_result_quit
print_yes:
    PRINT   "YES"                       ; else print "YES"
print_result_quit:
    PUTCHAR 10
quit:                                   ; finish the program
    FINISH