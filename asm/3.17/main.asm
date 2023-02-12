%include "../include/stud_io.inc"
    global  _start

    section .data
word_detected:
    db      FALSE                               ; define initial word detected flag

    section .text

; define constants
FALSE:      equ  0
TRUE:       equ  0xff

_start:
main:
read_line_lp:
    GETCHAR                                     ; read char
    push    eax                                 ; remember a char
    cmp     eax, -1
    je      read_line_lp_quit                   ; quit loop if EOF

    cmp     al, " "                             ; check end of a word
    jne     if_end_word_else
    mov     ah, [word_detected]
    cmp     ah, TRUE                            ; if a word ends
    jne     if_word_detected_end_quit
    PUTCHAR ")"                                 ; print ")"
if_word_detected_end_quit:
    mov     [word_detected], byte FALSE         ; if word ends, set word detected flag to false
    jmp     if_end_word_quit
if_end_word_else:
    mov     ah, [word_detected]                 ; if we met the start of a word
    cmp     ah, FALSE
    jne     if_word_detected_start_quit         ; and the char is not "\n"
    cmp     al, 10
    je      if_new_line_char_quit
    PUTCHAR "("                                 ; print "("
    mov     [word_detected], byte TRUE          ; and set flag to true
if_new_line_char_quit:
if_word_detected_start_quit:
if_end_word_quit:
    pop     eax                                 ; recall a char
    PUTCHAR al                                  ; print a char
    jmp     read_line_lp                        ; read next char
read_line_lp_quit:
quit:                                           ; finish the program
    FINISH