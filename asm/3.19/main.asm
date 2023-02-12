%include "../include/stud_io.inc"
    global  _start

    section .bss
digits:
    resb    DIGITS_COUNT

    section .text

; define constants
DIGITS_COUNT: equ 10

_start:
main:
    xor     ebx, ebx            ; set zero as initial value for chars count
read_line_lp:
    GETCHAR
    cmp     eax, -1
    je      read_line_lp_quit   ; quit loop if EOF
    inc     ebx                 
    jmp     read_line_lp        ; read next char
read_line_lp_quit:
    xor     edi, edi            ; define initial index value to fill array of digits
itoa_lp:
    xor     edx, edx            ; drop edx after div
    mov     eax, ebx            ; set dividend to eax as count of charactes stored in ebx
    mov     ecx, 10             ; div count of characters to 10 to get remainder of the division in edx
    div     ecx                 ; execute division
    mov     ebx, eax            ; update ebx with quotient
    lea     eax, [edx + '0']    ; calculate char code
    mov     [digits + edi], al  ; set char code to the array of digits
    inc     edi                 ; increment array index
    test    ebx, ebx            
    jnz     itoa_lp             ; break loop if digits are over

    mov     ecx, DIGITS_COUNT
print_chars_count_lp:           
    PUTCHAR [digits + ecx - 1]  ; print digit characters from array of digits
    loop    print_chars_count_lp
    PUTCHAR 10
quit:                           ; finish the program
    FINISH