%include "../include/stud_io.inc"
    global  _start

    section .data
the_longest_word_length:
    dd      0                                   ; define initial length of the longest word in a row
the_last_word_length:
    dd      0                                   ; define initial length of the last word in a row
current_word_length:
    dd      0

    section .text
print_stars_ecx:                                ; print stars; count of stars is stored in ecx register
    jecxz   .lp_quit                            ; if the result is zero, skip star printing loop
.lp:
    PUTCHAR "*"                                 ; print stars, using ecx register and loop construction
    loop    .lp
.lp_quit:
    ret

calculate_words_lengths:                        ; calculate lengths of the last and the longest words
    mov     eax, [current_word_length]
    test    eax, eax
    jz      .calculate_words_lengths_quit       ; skip calculation if counter equals zero
    mov     [the_last_word_length], eax         ; store the last word length
    mov     [current_word_length], dword 0      ; drop counter
    cmp     eax, [the_longest_word_length]      ; if greater word length reached, store the length as the longest
    jle     .if_new_the_longest_word_quit
    mov     eax, [the_last_word_length]
    mov     [the_longest_word_length], eax
.if_new_the_longest_word_quit:
.calculate_words_lengths_quit:
    ret

_start:
main:
read_line_lp:
    GETCHAR                                     ; read char
    cmp     eax, -1
    je      read_line_lp_quit                   ; quit loop if EOF
    cmp     al, 10
    jne     if_end_line_else

    call    calculate_words_lengths             ; recalculate lengths of the last and the longest words if the end of the line has reached

    PRINT   "The longest word length is: "
    mov     ecx, [the_longest_word_length]
    call    print_stars_ecx
    PUTCHAR 10

    PRINT   "The last word length is: "
    mov     ecx, [the_last_word_length]
    call    print_stars_ecx                     
    PUTCHAR 10

    mov     [the_longest_word_length], dword 0  ; drop the value for the new line
    mov     [the_last_word_length], dword 0     ; drop the value for the new line
    
    jmp     if_end_line_quit                    ; read next char
if_end_line_else:
    cmp     al, " "                             ; check end of a word
    jne     if_end_word_else
    call    calculate_words_lengths             ; if it is the end of the word, recalculate lengths of the last and the longest words
    jmp     if_end_word_quit
if_end_word_else:
    inc     dword [current_word_length]         ; increment counter of word length otherwise
if_end_word_quit:
if_end_line_quit:
    jmp     read_line_lp                        ; read next char
read_line_lp_quit:    
quit:                                           ; finish the program
    FINISH